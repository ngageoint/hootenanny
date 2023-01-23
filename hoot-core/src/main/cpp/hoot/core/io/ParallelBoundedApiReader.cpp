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
 * @copyright Copyright (C) 2019, 2020, 2021, 2022, 2023 Maxar (http://www.maxar.com/)
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
    _isPolygon(false),
    _totalResults(0),
    _totalEnvelopes(0),
    _maxGridSize(ConfigOptions().getReaderHttpBboxMaxDownloadSize()),
    _fatalError(false),
    _useOsmApiBboxFormat(useOsmApiBboxFormat),
    _addProjection(addProjection),
    _continueRunning(true),
    _filenumber(0),
    _timeout(ConfigOptions().getApiTimeout())
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
    QString query = FileUtils::readFully(_queryFilepath).replace("\r", "").replace("\n", "");
    //  Should the query be validated here?
    urlQuery.addQueryItem("data", query);
  }
  _sourceUrl.setQuery(urlQuery);
  //  Split the envelope if it is bigger than the prescribed max
  int lon_div = 1;
  int lat_div = 1;
  if (envelope.getWidth() > _coordGridSize)
    lon_div = (int)std::ceil(envelope.getWidth() / _coordGridSize);
  if (envelope.getHeight() > _coordGridSize)
    lat_div = (int)std::ceil(envelope.getHeight() / _coordGridSize);
  //  Record the number of envelopes to start with
  _totalEnvelopes = lat_div * lon_div;
  //  Setup the envelopes to query in a grid
  for (int i = 0; i < lon_div; ++i)
  {
    double lon = envelope.getMinX() + _coordGridSize * i;
    double lon2 = std::min(lon + _coordGridSize, envelope.getMaxX());
    for (int j = 0; j < lat_div; ++j)
    {
      double lat = envelope.getMinY() + _coordGridSize * j;
      double lat2 = std::min(lat + _coordGridSize, envelope.getMaxY());
      //  Start at the upper right corner and create boxes left to right, top to bottom
      std::lock_guard<std::mutex> bbox_lock(_bboxMutex);
      _bboxes.emplace(lon, lon2, lat, lat2);
    }
  }
  //  Start up the processing threads
  for (int i = 0; i < _threadCount; ++i)
    _threads.push_back(std::thread(&ParallelBoundedApiReader::_process, this));
}

bool ParallelBoundedApiReader::isComplete()
{
  //  Get the total number of envelopes
  int envelopes = 0;
  {
    std::lock_guard<std::mutex> bbox_lock(_bboxMutex);
    envelopes = _totalEnvelopes;
  }
  //  Get the total number of results
  int results = 0;
  {
    std::lock_guard<std::mutex> results_lock(_resultsMutex);
    results = _totalResults;
  }
  //  Done means there is one result for each envelope
  return envelopes == results && results > 0;
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
    //  Try to grab the next envelope on the queue
    geos::geom::Envelope envelope;
    envelope.setToNull();
    {
      std::lock_guard<std::mutex> bbox_lock(_bboxMutex);
      if (!_bboxes.empty())
      {
        envelope = _bboxes.front();
        _bboxes.pop();
      }
    }
    //  Make sure that we got a valid envelope
    if (!envelope.isNull())
    {
      //  Add the bbox to the query string
      bool add_bbox = true;
      QUrl url = _sourceUrl;
      QUrlQuery query(_sourceUrl);
      //  Remove any bbox that comes in so it can be updated
      if (query.hasQueryItem("bbox"))
        query.removeQueryItem("bbox");
      //  Use the correct type of bbox for this query
      QString bboxQuery;
      if (_useOsmApiBboxFormat)
        bboxQuery = GeometryUtils::toLonLatString(envelope);
      else
        bboxQuery = GeometryUtils::toMinMaxString(envelope);
      //  Overpass queries operate a little differently with the bbox
      if (_isOverpass)
      {
        //  Get the data parameter
        QString data = query.queryItemValue("data");
        //  Search the data parameter for {{bbox}} and replace it with the actual bounds
        if (data.contains("{{bbox}}"))
        {
          //  Embedded {{bbox}} data requires a Lat/Lon string
          bboxQuery = GeometryUtils::toLatLonString(envelope);
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
            //  Log the error
            LOG_DEBUG(error);
            //  Split the envelope into quads and retry
            _splitEnvelope(envelope);
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
            LOG_STATUS("Downloaded area (" << GeometryUtils::toLonLatString(envelope) << ")");
          }
        }
        catch(const std::bad_alloc&)
        {
          //  It is possible that the response is so big that Qt cannot handle it,
          //  split the envelope and try again
          _splitEnvelope(envelope);
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
          _splitEnvelope(envelope);
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
            std::lock_guard<std::mutex> bbox_lock(_bboxMutex);
            _sleep();
          }
          //  Retry, splitting the bbox for overpass timeouts
          if (_isOverpass)
            _splitEnvelope(envelope);
          else
          {
            //  The gateway timed out, retry the bbox
            std::lock_guard<std::mutex> bbox_lock(_bboxMutex);
            _bboxes.push(envelope);
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
            std::lock_guard<std::mutex> bbox_lock(_bboxMutex);
            _sleep(seconds * 1000);
            _bboxes.push(envelope);
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

void ParallelBoundedApiReader::_splitEnvelope(const geos::geom::Envelope &envelope)
{
  //  Split the envelope in quarters and push them all back on the queue
  double lon1 = envelope.getMinX();
  double lon2 = envelope.getMinX() + envelope.getWidth() / 2.0f;
  double lon3 = envelope.getMaxX();

  double lat1 = envelope.getMinY();
  double lat2 = envelope.getMinY() + envelope.getHeight() / 2.0f;
  double lat3 = envelope.getMaxY();
  //  Create the four envelopes
  std::vector<geos::geom::Envelope> envelopes(
    {
      geos::geom::Envelope(lon1, lon2, lat1, lat2),
      geos::geom::Envelope(lon2, lon3, lat1, lat2),
      geos::geom::Envelope(lon1, lon2, lat2, lat3),
      geos::geom::Envelope(lon2, lon3, lat2, lat3)
    });
  //  Lock down the _bbox queue
  std::lock_guard<std::mutex> bbox_lock(_bboxMutex);
  if (_isPolygon)
  {
    //  Split the envelope and remove any sub-envelopes that don't intersect the original polygon
    int envelope_count = 0;
    const geos::geom::GeometryFactory* factory = geos::geom::GeometryFactory::getDefaultInstance();
    //  Convert all of the envelopes
    for (const auto& e : envelopes)
    {
      //  Convert the envelope to a geometry
      std::unique_ptr<geos::geom::Geometry> g = factory->toGeometry(&e);
      //  Don't push envelopes that don't intersect the original geometry
      if (g->intersects(_boundingPoly.get()))
      {
        _bboxes.emplace(e);
        envelope_count++;
      }
    }
    //  Update the number of envelopes after the split
    if (envelope_count > 1)
      _totalEnvelopes += (envelope_count - 1);
  }
  else
  {
    //  Split the boxes into quads and push them all onto the queue
    for (const auto& e : envelopes)
      _bboxes.emplace(e);
    //  Increment by three because 1 turned into 4, i.e. 3 more were added
    _totalEnvelopes += 3;
  }
}


}
