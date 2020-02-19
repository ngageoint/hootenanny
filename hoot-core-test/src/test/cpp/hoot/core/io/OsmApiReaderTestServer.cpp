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
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "OsmApiReaderTestServer.h"

//  Hootenanny
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/HootNetworkUtils.h>
#include <hoot/core/util/OsmApiUtils.h>

namespace hoot
{

bool SimpleReaderTestServer::respond(HttpConnection::HttpConnectionPtr& connection)
{
  //  Read the HTTP request headers
  std::string headers = read_request_headers(connection);
  std::string message = "";
  //  Reply with ToyTestA.osm or with an HTTP 404 error
  HttpResponsePtr response;
  if (headers.find(OsmApiEndpoints::API_PATH_MAP) != std::string::npos)
    response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, FileUtils::readFully("test-files/ToyTestA.osm").toStdString()));
  else
    response.reset(new HttpResponse(HttpResponseCode::HTTP_NOT_FOUND));
  //  Write out the response
  write_response(connection, response->to_string());
  return false;
}

bool GeographicSplitReaderTestServer::respond(HttpConnection::HttpConnectionPtr& connection)
{
  //  Stop processing by setting this to false
  bool continue_processing = true;
  //  Read the HTTP request headers
  std::string headers = read_request_headers(connection);
  HttpResponsePtr response;
  //  Reply with some split up parts of ToyTestA.osm or with an HTTP 404 error
  if (headers.find(OsmApiEndpoints::API_PATH_MAP) != std::string::npos && _current < _max)
  {
    response = get_sequence_response(headers);
    //  After the fourth section, shutdown the test server
    if (_current == _max)
      continue_processing = false;
  }
  else
  {
    response.reset(new HttpResponse(HttpResponseCode::HTTP_NOT_FOUND));
    continue_processing = false;
  }
  //  Write out the response
  write_response(connection, response->to_string());
  //  Continue processing while there is still something to process or while he haven't been interupted
  return continue_processing && !get_interupt();
}

HttpResponsePtr GeographicSplitReaderTestServer::get_sequence_response(const std::string& request)
{
  HttpResponsePtr response(new HttpResponse(HttpResponseCode::HTTP_NOT_FOUND));
  //  Only respond up until the max is reached, then shutdown
  if (_current < _max)
  {
    //  If the same thing is being requested again, respond with the previous value
    std::map<std::string, HttpResponsePtr>::iterator it = _sequence_responses.find(request);
    if (it != _sequence_responses.end())
      response = it->second;
    else
    {
      //  Increment the sequence
      _current++;
      QString path = QString("test-files/io/OsmApiReaderTest/ToyTestA-Part%1.osm").arg(_current);
      response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, FileUtils::readFully(path).toStdString()));
      _sequence_responses[request] = response;
    }
  }
  return response;
}

bool ElementSplitReaderTestServer::respond(HttpConnection::HttpConnectionPtr& connection)
{
  //  Stop processing by setting this to false
  bool continue_processing = true;
  //  Read the HTTP request headers
  std::string headers = read_request_headers(connection);
  HttpResponsePtr response;
  //  Reply with some split up parts of ToyTestA.osm or with an HTTP 404 error
  if (headers.find(OsmApiEndpoints::API_PATH_MAP) != std::string::npos && _current < _max)
  {
    response = get_sequence_response(headers);
    //  After the fourth section, shutdown the test server
    if (_current == _max)
      continue_processing = false;
  }
  else
  {
    response.reset(new HttpResponse(HttpResponseCode::HTTP_NOT_FOUND));
    continue_processing = false;
  }
  //  Write out the response
  write_response(connection, response->to_string());
  //  Continue processing while there is still something to process or while he haven't been interupted
  return continue_processing && !get_interupt();
}

HttpResponsePtr ElementSplitReaderTestServer::get_sequence_response(const std::string& request)
{
  HttpResponsePtr response(new HttpResponse(HttpResponseCode::HTTP_NOT_FOUND));
  //  Only respond up until the max is reached, then shutdown
  if (_current < _max)
  {
    if (!_split_forced)
    {
      //  Force a split by responding with 400 BAD REQUEST
      _split_forced = true;
      _current = 1;
      response.reset(new HttpResponse(HttpResponseCode::HTTP_BAD_REQUEST));
      _sequence_responses[request] = response;
    }
    else
    {
      //  If the same thing is being requested again, respond with the previous value
      std::map<std::string, HttpResponsePtr>::iterator it = _sequence_responses.find(request);
      if (it != _sequence_responses.end())
        response = it->second;
      else
      {
        //  Increment the sequence
        _current++;
        QString path = QString("test-files/io/OsmApiReaderTest/ToyTestA-Part%1.osm").arg(_current - 1);
        response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, FileUtils::readFully(path).toStdString()));
        _sequence_responses[request] = response;
      }
    }
  }
  return response;
}

}
