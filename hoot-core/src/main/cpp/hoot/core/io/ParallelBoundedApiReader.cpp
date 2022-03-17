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
 * @copyright Copyright (C) 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
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

namespace hoot
{

ParallelBoundedApiReader::ParallelBoundedApiReader(bool useOsmApiBboxFormat, bool addProjection)
  : _dataType(DataType::Text),
    _coordGridSize(ConfigOptions().getReaderHttpBboxMaxSize()),
    _threadCount(ConfigOptions().getReaderHttpBboxThreadCount()),
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
  {
    throw UnsupportedException(
      QString("Cannot request areas larger than %1 square degrees.").arg(QString::number(_maxGridSize, 'f', 4)));
  }
  //  Save the endpoint URL to query
  _sourceUrl = endpoint;
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

void ParallelBoundedApiReader::_sleep() const
{
  //  Sleep for 10 milliseconds
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void ParallelBoundedApiReader::_process()
{
  int timeout = 0;
  int max_timeout = 3;
  //  Continue working until all of the results are back
  while (!isComplete() && _continueRunning && !_fatalError)
  {
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
      QUrl url = _sourceUrl;
      QUrlQuery query(_sourceUrl);
      if (query.hasQueryItem("bbox"))
        query.removeQueryItem("bbox");
      //  Use the correct type of bbox for this query
      QString bboxQuery;
      if (_useOsmApiBboxFormat)
        bboxQuery = GeometryUtils::toConfigString(envelope);
      else
        bboxQuery = GeometryUtils::toString(envelope);
      //  Some APIs require the bounding box's projection, add it here
      if (_addProjection)
        bboxQuery += ",EPSG:4326";
      query.addQueryItem("bbox", bboxQuery);
      url.setQuery(query);

      HootNetworkRequest request;
      LOG_VART(url);
      request.networkRequest(url, _timeout);
      //  Check the HTTP status code and result
      int status = request.getHttpStatus();
      QString result = QString::fromUtf8(request.getResponseContent().data());
      switch (status)
      {
      case HttpResponseCode::HTTP_OK:
        //  Store the result and increment the number of results received
        {
          std::lock_guard<std::mutex> results_lock(_resultsMutex);
          _resultsList.append(result);
          _totalResults++;
        }
        //  Write out a "debug map" for each result that comes in
        writeDebugMap(result, "bounded-reader-result");
        break;
      case HttpResponseCode::HTTP_BAD_REQUEST:
        //  Split the envelope in quarters and push them all back on the queue
        {
          double lon1 = envelope.getMinX();
          double lon2 = envelope.getMinX() + envelope.getWidth() / 2.0f;
          double lon3 = envelope.getMaxX();

          double lat1 = envelope.getMinY();
          double lat2 = envelope.getMinY() + envelope.getHeight() / 2.0f;
          double lat3 = envelope.getMaxY();
          std::lock_guard<std::mutex> bbox_lock(_bboxMutex);
          //  Split the boxes into quads and push them onto the queue
          _bboxes.emplace(lon1, lon2, lat1, lat2);
          _bboxes.emplace(lon2, lon3, lat1, lat2);
          _bboxes.emplace(lon1, lon2, lat2, lat3);
          _bboxes.emplace(lon2, lon3, lat2, lat3);
          //  Increment by three because 1 turned into 4, i.e. 3 more were added
          _totalEnvelopes += 3;
        }
        break;
      case HttpResponseCode::HTTP_BANDWIDTH_EXCEEDED:
      {
        //  Bandwidth for downloads has been exceeded, fail immediately
        std::lock_guard<std::mutex> error_lock(_errorMutex);
        LOG_ERROR(request.getErrorString());
        _fatalError = true;
        break;
      }
      case HttpResponseCode::HTTP_GATEWAY_TIMEOUT:
        timeout++;
        if (timeout <= max_timeout)
        {
          //  The gateway timed out, retry the bbox
          std::lock_guard<std::mutex> bbox_lock(_bboxMutex);
          //  Sleep before retrying, inside of the mutex so that other threads wait too
          _sleep();
          //  Retry
          _bboxes.push(envelope);
          LOG_DEBUG("Gateway timed out while communicating with API, retrying.");
        }
        else
          logNetworkError(request);
        break;
      default:
        logNetworkError(request);
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

void ParallelBoundedApiReader::writeDebugMap(const QString& data, const QString& name)
{
  if (ConfigOptions().getDebugMapsWrite())
  {
    //  Get the file extension based on the data type downloaded
    QString ext;
    switch (_dataType)
    {
    default:
    case DataType::Text:      ext = "txt";      break;
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

void ParallelBoundedApiReader::logNetworkError(const HootNetworkRequest& request)
{
  std::lock_guard<std::mutex> error_lock(_errorMutex);
  request.logConnectionError();
  _fatalError = true;
}

}
