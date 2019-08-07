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

#include "OsmApiWriterTestServer.h"

//  Hootenanny
#include <hoot/core/io/OsmApiWriter.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

const char* OsmApiSampleResponses::SAMPLE_CAPABILITIES =
    "<?xml version='1.0' encoding='UTF-8'?>"
    "<osm version='0.6' generator='OpenStreetMap server' copyright='OpenStreetMap and contributors' attribution='https://www.openstreetmap.org/copyright' license='http://opendatacommons.org/licenses/odbl/1-0/'>"
    "  <api>"
    "    <version minimum='0.6' maximum='0.6'/>"
    "    <area maximum='0.25'/>"
    "    <note_area maximum='25'/>"
    "    <tracepoints per_page='5000'/>"
    "    <waynodes maximum='2000'/>"
    "    <changesets maximum_elements='10000'/>"
    "    <timeout seconds='300'/>"
    "    <status database='online' api='online' gpx='online'/>"
    "  </api>"
    "  <policy>"
    "    <imagery>"
    "      <blacklist regex='http://xdworld\\.vworld\\.kr:8080/.*'/>"
    "      <blacklist regex='.*\\.here\\.com[/:].*'/>"
    "    </imagery>"
    "  </policy>"
    "</osm>";
const char* OsmApiSampleResponses::SAMPLE_PERMISSIONS =
    "<?xml version='1.0' encoding='UTF-8'?>"
    "<osm version='0.6' generator='OpenStreetMap server'>"
    "  <permissions>"
    "    <permission name='allow_read_prefs'/>"
    "    <permission name='allow_read_gpx'/>"
    "    <permission name='allow_write_api'/>"
    "    <permission name='allow_write_gpx'/>"
    "  </permissions>"
    "</osm>";

bool CapabilitiesTestServer::respond(HttpConnection::HttpConnectionPtr &connection)
{
  //  Read the HTTP request headers
  std::string headers = read_request_headers(connection);
  //  Make sure that the capabilities were requested
  std::string message;
  if (headers.find(OsmApiWriter::API_PATH_CAPABILITIES) != std::string::npos)
    message = HTTP_200_OK + OsmApiSampleResponses::SAMPLE_CAPABILITIES + "\r\n";
  else
    message = HTTP_404_NOT_FOUND;
  //  Write out the response
  write_response(connection, message);
  //  Only respond once to the client
  return false;
}

bool PermissionsTestServer::respond(HttpConnection::HttpConnectionPtr &connection)
{
  //  Read the HTTP request headers
  std::string headers = read_request_headers(connection);
  //  Make sure that the permissions were requested
  std::string message;
  if (headers.find(OsmApiWriter::API_PATH_PERMISSIONS) != std::string::npos)
    message = HTTP_200_OK + OsmApiSampleResponses::SAMPLE_PERMISSIONS + "\r\n";
  else
    message = HTTP_404_NOT_FOUND;
  //  Write out the response
  write_response(connection, message);
  //  Only respond once to the client
  return false;
}

bool RetryConflictsTestServer::respond(HttpConnection::HttpConnectionPtr& connection)
{
  //  Stop processing by setting this to false
  bool continue_processing = true;
  //  Read the HTTP request headers
  std::string headers = read_request_headers(connection);
  //  Determine the response message's HTTP header
  std::string message;
  if (headers.find(OsmApiWriter::API_PATH_CAPABILITIES) != std::string::npos)
    message = HTTP_200_OK + OsmApiSampleResponses::SAMPLE_CAPABILITIES + "\r\n";
  else if (headers.find(OsmApiWriter::API_PATH_PERMISSIONS) != std::string::npos)
    message = HTTP_200_OK + OsmApiSampleResponses::SAMPLE_PERMISSIONS + "\r\n";
  else if (headers.find(OsmApiWriter::API_PATH_CREATE_CHANGESET) != std::string::npos)
    message = HTTP_200_OK + "1\r\n";
  else if (headers.find("POST") != std::string::npos)
    message = std::string("HTTP/1.1 405 Method Not Allowed\r\nAllow: GET\r\n\r\n");
  else if (headers.find("/close/"))
  {
    message = HTTP_200_OK;
    continue_processing = false;
  }
  else
  {
    //  Error out here
    message = HTTP_404_NOT_FOUND;
    continue_processing = false;
  }
  //  Write out the response
  write_response(connection, message);
  //  Return true if we should continue listening and processing requests
  return continue_processing;
}

}
