/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2019-2023 Maxar (http://www.maxar.com/)
 */

#include "ParallelBoundedApiReader.h"

//  Hootenanny
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/io/HootNetworkRequest.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/HootNetworkUtils.h>
#include <hoot/core/util/StringUtils.h>

//  Qt
#include <QUrlQuery>

//  Geos
#include <geos/geom/GeometryFactory.h>

//  Standard
#include <map>

namespace hoot
{

ParallelBoundedApiReader::ParallelBoundedApiReader(bool useOsmApiBboxFormat, bool addProjection)
  : _dataType(DataType::PlainText),
    _coordGridSize(ConfigOptions().getReaderHttpBboxMaxSize()),
    _threadCount(ConfigOptions().getReaderHttpBboxThreadCount()),
    _totalResults(0),
    _totalWork(0),
    _maxGridSize(ConfigOptions().getReaderHttpBboxMaxDownloadSize()),
    _fatalError(false),
    _useOsmApiBboxFormat(useOsmApiBboxFormat),
    _addProjection(addProjection),
    _continueRunning(true),
    _filenumber(0),
    _timeout(ConfigOptions().getApiTimeout()),
    _isPolygon(false)
{
}

ParallelBoundedApiReader::~ParallelBoundedApiReader()
{
  stop();
}

void ParallelBoundedApiReader::beginRead(const QUrl& endpoint, const geos::geom::Envelope& envelope)
{
  //  Validate the size of the envelope, in square degrees, before beginning
  //  Don't allow the whole earth to be downloaded!
  if (envelope.getWidth() * envelope.getHeight() > _maxGridSize)
    throw UnsupportedException(QString("Cannot request areas larger than %1 square degrees.").arg(QString::number(_maxGridSize, 'f', 4)));
  //  Save the endpoint URL to query
  _sourceUrl = endpoint;
  //  Load the query from a file if requested
  QUrlQuery urlQuery(_sourceUrl);
  if (!urlQuery.hasQueryItem("data") && !_queryFilepath.isEmpty())
  {
    QString query = _readOverpassQueryFile(_queryFilepath);
    //  Should the query be validated here?
    urlQuery.addQueryItem("data", query);
  }
  _sourceUrl.setQuery(urlQuery);

  std::vector<std::shared_ptr<ParallelApiJob>> splits = std::make_shared<BoundedParallelApiJob>(_sourceUrl.toString(), envelope, _coordGridSize)->CreateInitialJob();
  //  No need to lock the mutex here because the other threads that access these member variables haven't started up yet
  _totalWork = static_cast<int>(splits.size());
  for (const auto& j : splits)
    _workQueue.emplace(j);
  //  Start up the processing threads
  for (int i = 0; i < _threadCount; ++i)
    _threads.push_back(std::thread(&ParallelBoundedApiReader::_process, this));
}

void ParallelBoundedApiReader::beginRead(const QUrl& endpoint)
{
  //  Save the endpoint URL to query
  _sourceUrl = endpoint;
  //  Load the query from a file if requested
  QUrlQuery urlQuery(_sourceUrl);
  if (!urlQuery.hasQueryItem("data") && !_queryFilepath.isEmpty())
  {
    QString query = _readOverpassQueryFile(_queryFilepath);
    //  Should the query be validated here?
    urlQuery.addQueryItem("data", query);
  }
  _sourceUrl.setQuery(urlQuery);

  std::vector<std::shared_ptr<ParallelApiJob>> queries;
  while (hasMoreQueries())
    queries.push_back(std::make_shared<QueryParallelApiJob>(getNextQuery(_sourceUrl.toString())));
  //  No need to lock the mutex here because the other threads that access these member variables haven't started up yet
  _totalWork = static_cast<int>(queries.size());
  for (const auto& j : queries)
    _workQueue.emplace(j);
  //  Start up the processing threads
  for (int i = 0; i < _threadCount; ++i)
    _threads.push_back(std::thread(&ParallelBoundedApiReader::_process, this));
}


bool ParallelBoundedApiReader::isComplete()
{
  //  Get the total number of jobs
  int jobs = 0;
  {
    std::lock_guard<std::mutex> work_lock(_workMutex);
    jobs = _totalWork;
  }
  //  Get the total number of results
  int results = 0;
  {
    std::lock_guard<std::mutex> results_lock(_resultsMutex);
    results = _totalResults;
  }
  //  Done means there is one result for each job
  return jobs == results && results > 0;
}

bool ParallelBoundedApiReader::getSingleResult(QString& result)
{
  bool success = true;
  std::lock_guard<std::mutex> results_lock(_resultsMutex);
  //  takeFirst() pops the first element and returns it
  if (!_resultsList.empty())
    result = _resultsList.takeFirst();
  else
    success = false;
  //  Return the result found
  return success;
}

bool ParallelBoundedApiReader::hasMoreResults()
{
  bool more = false;
  {
    std::lock_guard<std::mutex> results_lock(_resultsMutex);
    more = !_resultsList.empty();
  }
  bool done = isComplete();
  //  There are more results when the queue contains results
  //  or the threads are still processing envelopes
  //  and there isn't an error
  return (more || !done) && !_fatalError;
}

void ParallelBoundedApiReader::wait()
{
  //  Wait on the threads to complete
  for (std::size_t i = 0; i < _threads.size(); ++i)
    _threads[i].join();
}

void ParallelBoundedApiReader::stop()
{
  //  Stop the threads
  _continueRunning = false;
  //  Wait for all threads to stop
  wait();
}

void ParallelBoundedApiReader::_sleep(long milliseconds) const
{
  //  Sleep (default for 10 milliseconds)
  std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void ParallelBoundedApiReader::_setBoundingPoly(const std::shared_ptr<geos::geom::Geometry>& poly)
{
  _boundingPoly = poly;
  //  Let all parallel bounded jobs know of the bounding poly
  BoundedParallelApiJob::setBoundingPolygon(poly);
}

void ParallelBoundedApiReader::_setIsBoundsPoly(bool isPoly)
{
  _isPolygon = isPoly;
  //  Let all parallel bounded jobs know if the bounds is a polygon
  BoundedParallelApiJob::setBoundsIsPolygon(isPoly);
}

void ParallelBoundedApiReader::_process()
{
  int timeout = 0;
  int max_timeout = 3;
  //  Continue working until all of the results are back
  while (!isComplete() && _continueRunning && !_fatalError)
  {
    //  Don't allow the results list to get too large, i.e larger than the thread count
    bool isFull = false;
    {
      std::lock_guard<std::mutex> results_lock(_resultsMutex);
      isFull = _resultsList.size() >= _threadCount;
    }
    if (isFull)
    {
      //  The results buffer is full, so there is no work to be done yet
      _sleep();
      continue;
    }
    //  Try to grab the next job on the queue
    ParallelApiJobPtr job;
    {
      std::lock_guard<std::mutex> work_lock(_workMutex);
      if (!_workQueue.empty())
      {
        job = _workQueue.front();
        _workQueue.pop();
      }
    }
    //  Make sure that we got a valid envelope
    if (job)
    {
      QUrl url(job->getQuery());
      if (job->isBounded())
      {
        //  Add the bbox to the query string
        QUrlQuery query(url);
        bool add_bbox = true;
        //  Remove any bbox that comes in so it can be updated
        if (query.hasQueryItem("bbox"))
          query.removeQueryItem("bbox");
        //  Use the correct type of bbox for this query
        QString bboxQuery;
        if (_useOsmApiBboxFormat)
          bboxQuery = GeometryUtils::toLonLatString(job->getBounds());
        else
          bboxQuery = GeometryUtils::toMinMaxString(job->getBounds());
        //  Overpass queries operate a little differently with the bbox
        if (_isOverpass)
        {
          //  Get the data parameter
          QString data = query.queryItemValue("data");
          //  Search the data parameter for {{bbox}} and replace it with the actual bounds
          if (data.contains("{{bbox}}"))
          {
            //  Embedded {{bbox}} data requires a Lat/Lon string
            bboxQuery = GeometryUtils::toLatLonString(job->getBounds());
            //  Remove the old 'data' query
            query.removeQueryItem("data");
            //  Replace the bbox place holder
            data = data.replace("{{bbox}}", QString("%1").arg(bboxQuery));
            //  Set the data query item back to the query
            query.addQueryItem("data", data);
            //  Don't add the bbox query item later because it is embedded in the data query item
            add_bbox = false;
          }
        }
        else if (_addProjection)
          bboxQuery += ",EPSG:4326";  //  Some APIs require the bounding box's projection, add it here
        //  Add the bbox query item if necessary
        if (add_bbox)
          query.addQueryItem("bbox", bboxQuery);
        url.setQuery(query);
      }
      else if (url.toString().isEmpty())  //  Ignore empty URLs
        continue;

      HootNetworkRequest request;
      LOG_DEBUG(url);
      request.networkRequest(url, _timeout);
      //  Check the HTTP status code and result
      int status = request.getHttpStatus();
      switch (status)
      {
      case HttpResponseCode::HTTP_OK:
        try
        {
          //  Store the result and increment the number of results received
          QString result = QString::fromUtf8(request.getResponseContent().data());
          //  Check for an error
          QString error;
          if (_isQueryError(result, error))
          {
            if (error.contains("Cannot allocate memory"))
            {
              std::lock_guard<std::mutex> error_lock(_errorMutex);
              LOG_ERROR("API Failure: " << error);
              _fatalError = true;
            }
            else
            {
              //  Log the error
              LOG_DEBUG(error);
              //  Split the job and retry
              _splitJob(job);
            }
          }
          else
          {
            std::lock_guard<std::mutex> results_lock(_resultsMutex);
            _resultsList.append(result);
            _totalResults++;
            //  Write out a "debug map" for each result that comes in
            _writeDebugMap(result, "bounded-reader-result");
            //  Reset the timeout because this thread has successfully received a response
            timeout = 0;
            //  Update the user status
            if (job->isBounded())
            {
              LOG_STATUS("Downloaded area (" << GeometryUtils::toLonLatString(job->getBounds()) << ")");
            }
            else
            {
              LOG_STATUS("Downloaded elements: " << FileUtils::toLogFormat(url.toString()));
            }
          }
        }
        catch(const std::bad_alloc&)
        {
          //  It is possible that the response is so big that Qt cannot handle it,
          //  split the job and try again
          _splitJob(job);
        }
        break;
      case HttpResponseCode::HTTP_BAD_REQUEST:
        if (_isOverpass)
        {
          //  Overpass API 400 Bad Request means the query didn't parse correctly and the error is in the response
          std::lock_guard<std::mutex> error_lock(_errorMutex);
          LOG_ERROR("Overpass Error: " + parseOverpassError(QString::fromUtf8(request.getResponseContent().data())));
          LOG_VARD(url);
          _fatalError = true;
        }
        else
        {
          //  OSM API 400 Bad Request means the node/way/relation limit has been exceeded
          _splitJob(job);
        }
        break;
      case HttpResponseCode::HTTP_BANDWIDTH_EXCEEDED:        //  Bandwidth for downloads has been exceeded, fail immediately
      {
        std::lock_guard<std::mutex> error_lock(_errorMutex);
        LOG_ERROR(request.getErrorString());
        _fatalError = true;
        break;
      }
      case HttpResponseCode::HTTP_GATEWAY_TIMEOUT:
      case HttpResponseCode::HTTP_SERVICE_UNAVAILABLE:
      case HttpResponseCode::HTTP_BAD_GATEWAY:
        timeout++;
        if (timeout <= max_timeout)
        {
          //  Sleep before retrying, inside of the mutex so that other threads wait too
          {
            std::lock_guard<std::mutex> work_lock(_workMutex);
            _sleep();
          }
          //  Retry, splitting the job for overpass timeouts
          if (_isOverpass)
            _splitJob(job);
          else
          {
            //  The gateway timed out, retry the job
            std::lock_guard<std::mutex> job_lock(_workMutex);
            _workQueue.push(job);
          }
          LOG_WARN("Gateway error (" << status << ") while communicating with API, retrying.");
        }
        else
        {
          //  Log the error after max_time out number of errors in this thread
          LOG_ERROR(request.getErrorString());
        }
        break;
      case HttpResponseCode::HTTP_TOO_MANY_REQUESTS:
      {
        const std::map<QString, QString>& headers = request.getResponseHeaders();
        if (headers.find("Retry-After") != headers.end())
        {
          QString retry = headers.at("Retry-After");
          int seconds = retry.toInt();
          if (seconds <= 120)
          {
            LOG_WARN("Server responded: Too many requests.  Trying again in " << retry << " seconds.");
            std::lock_guard<std::mutex> work_lock(_workMutex);
            _sleep(seconds * 1000);
            _workQueue.push(job);
          }
          else
          {
            //  Log the unrecoverable error
            std::lock_guard<std::mutex> error_lock(_errorMutex);
            LOG_ERROR(request.getErrorString());
            _fatalError = true;
          }
        }
        else
        {
          //  Log the unrecoverable error
          std::lock_guard<std::mutex> error_lock(_errorMutex);
          LOG_ERROR(request.getErrorString());
          _fatalError = true;
        }
        break;
      }
      default:
        _logNetworkError(request);
        break;
      }
    }
    else
    {
      //  Give-up the timeslice since there is no work to be done currently
      _sleep();
    }
  }
}

void ParallelBoundedApiReader::_writeDebugMap(const QString& data, const QString& name)
{
  if (ConfigOptions().getDebugMapsWrite())
  {
    //  Get the file extension based on the data type downloaded
    QString ext;
    switch (_dataType)
    {
    default:
    case DataType::PlainText: ext = "txt";      break;
    case DataType::OsmXml:    ext = "osm";      break;
    case DataType::Json:      ext = "json";     break;
    case DataType::GeoJson:   ext = "geojson";  break;
    }
    //  Get the unique file number and increment it
    QString filenumber;
    {
      std::lock_guard<std::mutex> filenumber_lock(_filenumberMutex);
      filenumber = StringUtils::padFrontOfNumberStringWithZeroes(_filenumber++, 3);
    }
    //  Write out the text to a uniquely named file
    FileUtils::writeFully(QString("tmp/%1-%2.%3").arg(name, filenumber, ext), data);
  }
}

void ParallelBoundedApiReader::_logNetworkError(const HootNetworkRequest& request)
{
  std::lock_guard<std::mutex> error_lock(_errorMutex);
  request.logConnectionError();
  _fatalError = true;
}

bool ParallelBoundedApiReader::_isQueryError(const QString& result, QString& error) const
{
  //  XML handling regular expression
  // <remark> runtime error: Query ran out of memory in "query" at line 10. It would need at least 0 MB of RAM to continue. </remark>
  static QRegularExpression regexXml("<remark> (.*) </remark>", QRegularExpression::OptimizeOnFirstUsageOption);
  //  JSON handling regular expression
  //  "remark": "runtime error: Query ran out of memory in \"query\" at line 10. It would need at least 0 MB of RAM to continue."
  static QRegularExpression regexJson("[\"|\']remark[\"|\'|>]: *[\"|\']?(runtime error.*)[\"|\']", QRegularExpression::OptimizeOnFirstUsageOption);
  static std::vector<QRegularExpression> regexes({regexXml, regexJson});

  for (const auto& regex : regexes)
  {
    QRegularExpressionMatch match = regex.match(result);
    if (match.hasMatch())
    {
      error = match.captured(1);
      return true;
    }
  }
  return false;
}

void ParallelBoundedApiReader::_splitJob(const ParallelApiJobPtr& job)
{
  //  Split the job
  std::vector<ParallelApiJobPtr> splits = job->SplitJob();
  //  Lock down the _workQueue
  std::lock_guard<std::mutex> bbox_lock(_workMutex);
  //  Add the work to the queue and update the total work
  for (const auto& split : splits)
    _workQueue.push(split);
  _totalWork += static_cast<int>(splits.size() - 1);
}

}
