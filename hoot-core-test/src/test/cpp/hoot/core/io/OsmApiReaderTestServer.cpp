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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "OsmApiReaderTestServer.h"

//  Hootenanny
#include <hoot/core/util/FileUtils.h>

namespace hoot
{

bool SimpleReaderTestServer::respond(HttpConnection::HttpConnectionPtr& connection)
{
  //  Read the HTTP request headers
  std::string headers = read_request_headers(connection);
  std::string message = "";
  //  Reply with ToyTestA.osm or with an HTTP 404 error
  if (headers.find("/api/0.6/map") != std::string::npos)
    message = HTTP_200_OK + FileUtils::readFully("test-files/ToyTestA.osm").toStdString() + "\r\n";
  else
    message = HTTP_404_NOT_FOUND;
  //  Write out the response
  write_response(connection, message);
  return false;
}

bool GeographicSplitReaderTestServer::respond(HttpConnection::HttpConnectionPtr& connection)
{
  //  Stop processing by setting this to false
  bool continue_processing = true;
  //  Read the HTTP request headers
  std::string headers = read_request_headers(connection);
  std::string message = "";
  //  Reply with some split up parts of ToyTestA.osm or with an HTTP 404 error
  if (headers.find("/api/0.6/map") != std::string::npos && _section < 4)
  {
    //  Increment the section
    _section++;
    QString path = QString("test-files/io/OsmApiReaderTest/ToyTestA-Part%1.osm").arg(_section);
    message = HTTP_200_OK + FileUtils::readFully(path).toStdString() + "\r\n";
    //  After the fourth section, shutdown the test server
    if (_section == 4)
      continue_processing = false;
  }
  else
  {
    message = HTTP_404_NOT_FOUND;
    continue_processing = false;
  }
  //  Write out the response
  write_response(connection, message);
  return continue_processing;
}

bool ElementSplitReaderTestServer::respond(HttpConnection::HttpConnectionPtr& connection)
{
  //  Stop processing by setting this to false
  bool continue_processing = true;
  //  Read the HTTP request headers
  std::string headers = read_request_headers(connection);
  std::string message = "";
  //  Reply with some split up parts of ToyTestA.osm or with an HTTP 404 error
  if (headers.find("/api/0.6/map") != std::string::npos && _section < 4)
  {
    if (!_splitForced)
    {
      message = HTTP_400_BAD_REQUEST;
      _splitForced = true;
    }
    else
    {
      //  Increment the section
      _section++;
      QString path = QString("test-files/io/OsmApiReaderTest/ToyTestA-Part%1.osm").arg(_section);
      message = HTTP_200_OK + FileUtils::readFully(path).toStdString() + "\r\n";
      //  After the fourth section, shutdown the test server
      if (_section == 4)
        continue_processing = false;
    }
  }
  else
    message = HTTP_404_NOT_FOUND;
  //  Write out the response
  write_response(connection, message);
  return continue_processing;
}

}
