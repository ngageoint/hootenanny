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

#include "OsmApiWriterTestServer.h"

//  Hootenanny
#include <hoot/core/io/OsmApiWriter.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

bool CapabilitiesTestServer::respond(HttpConnection::HttpConnectionPtr &connection)
{
  //  Read the HTTP request headers
  std::string headers = read_request_headers(connection);
  //  Make sure that the capabilities were requested
  HttpResponsePtr response;
  if (headers.find(OsmApiWriter::API_PATH_CAPABILITIES) != std::string::npos)
    response.reset(new HttpResponse(200, OsmApiSampleRequestResponse::SAMPLE_CAPABILITIES_RESPONSE));
  else
    response.reset(new HttpResponse(404));
  //  Write out the response
  write_response(connection, response->to_string());
  //  Only respond once to the client
  return false;
}

bool PermissionsTestServer::respond(HttpConnection::HttpConnectionPtr &connection)
{
  //  Read the HTTP request headers
  std::string headers = read_request_headers(connection);
  //  Make sure that the permissions were requested
  HttpResponsePtr response;
  if (headers.find(OsmApiWriter::API_PATH_PERMISSIONS) != std::string::npos)
    response.reset(new HttpResponse(200, OsmApiSampleRequestResponse::SAMPLE_PERMISSIONS_RESPONSE));
  else
    response.reset(new HttpResponse(404));
  //  Write out the response
  write_response(connection, response->to_string());
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
  HttpResponsePtr response;
  if (headers.find(OsmApiWriter::API_PATH_CAPABILITIES) != std::string::npos)
    response.reset(new HttpResponse(200, OsmApiSampleRequestResponse::SAMPLE_CAPABILITIES_RESPONSE));
  else if (headers.find(OsmApiWriter::API_PATH_PERMISSIONS) != std::string::npos)
    response.reset(new HttpResponse(200, OsmApiSampleRequestResponse::SAMPLE_PERMISSIONS_RESPONSE));
  else if (headers.find(OsmApiWriter::API_PATH_CREATE_CHANGESET) != std::string::npos)
    response.reset(new HttpResponse(200, "1"));
  else if (headers.find("POST") != std::string::npos)
  {
    response.reset(new HttpResponse(405));
    response->add_header("Allow", "GET");
  }
  else if (headers.find(QString(OsmApiWriter::API_PATH_CLOSE_CHANGESET).arg(1).toStdString()))
  {
    response.reset(new HttpResponse(200));
    continue_processing = false;
  }
  else
  {
    //  Error out here
    response.reset(new HttpResponse(404));
    continue_processing = false;
  }
  //  Write out the response
  write_response(connection, response->to_string());
  //  Return true if we should continue listening and processing requests
  return continue_processing && !get_interupt();
}

bool RetryVersionTestServer::respond(HttpConnection::HttpConnectionPtr &connection)
{
  /*
  *  - Capabilities
  *  - Permissions
  *  - Changeset Create
  *  - Changeset 1 Upload - respond with an HTTP 404 error for the test
  *  - Element get
  *  - Changeset 1 Upload - respond with updated version
  *  - Changeset Close
  */
  //  Stop processing by setting this to false
  bool continue_processing = true;
  //  Read the HTTP request headers
  std::string headers = read_request_headers(connection);
  //  Determine the response message's HTTP response
  HttpResponsePtr response;
  //  Capabilities
  if (headers.find(OsmApiWriter::API_PATH_CAPABILITIES) != std::string::npos)
    response.reset(new HttpResponse(200, OsmApiSampleRequestResponse::SAMPLE_CAPABILITIES_RESPONSE));
  //  Permissions
  else if (headers.find(OsmApiWriter::API_PATH_PERMISSIONS) != std::string::npos)
    response.reset(new HttpResponse(200, OsmApiSampleRequestResponse::SAMPLE_PERMISSIONS_RESPONSE));
  //  Create changeset
  else if (headers.find(OsmApiWriter::API_PATH_CREATE_CHANGESET) != std::string::npos)
    response.reset(new HttpResponse(200, "1"));
  //  Upload changeset 1
  else if (headers.find(QString(OsmApiWriter::API_PATH_UPLOAD_CHANGESET).arg(1).toStdString()) != std::string::npos)
  {
    //  The first time through, the 'version' of element 1 should fail.
    if (!_has_error)
    {
      response.reset(new HttpResponse(409, "Changeset conflict: Version mismatch: Provided 2, server had: 1 of Way 1"));
      _has_error = true;
    }
    else
      response.reset(new HttpResponse(200, OsmApiSampleRequestResponse::SAMPLE_CHANGESET_1_RESPONSE));
  }
  //  Get way 1's updated version
  else if (headers.find(QString(OsmApiWriter::API_PATH_GET_ELEMENT).arg("way").arg(1).toStdString()) != std::string::npos)
    response.reset(new HttpResponse(200, OsmApiSampleRequestResponse::SAMPLE_ELEMENT_1_GET_RESPONSE));
  //  Close changeset
  else if (headers.find(QString(OsmApiWriter::API_PATH_CLOSE_CHANGESET).arg(1).toStdString()))
  {
    response.reset(new HttpResponse(200));
    continue_processing = false;
  }
  else
  {
    //  Error out here
    response.reset(new HttpResponse(404));
    continue_processing = false;
  }
  //  Write out the response
  write_response(connection, response->to_string());
  //  Return true if we should continue listening and processing requests
  return continue_processing && !get_interupt();
}

const char* OsmApiSampleRequestResponse::SAMPLE_CAPABILITIES_RESPONSE =
    "<?xml version='1.0' encoding='UTF-8'?>\n"
    "<osm version='0.6' generator='OpenStreetMap server'>\n"
    "  <api>\n"
    "    <version minimum='0.6' maximum='0.6'/>\n"
    "    <area maximum='0.25'/>\n"
    "    <note_area maximum='25'/>\n"
    "    <tracepoints per_page='5000'/>\n"
    "    <waynodes maximum='2000'/>\n"
    "    <changesets maximum_elements='10000'/>\n"
    "    <timeout seconds='300'/>\n"
    "    <status database='online' api='online' gpx='online'/>\n"
    "  </api>\n"
    "  <policy>\n"
    "    <imagery>\n"
    "      <blacklist regex='http://xdworld\\.vworld\\.kr:8080/.*'/>\n"
    "      <blacklist regex='.*\\.here\\.com[/:].*'/>\n"
    "    </imagery>\n"
    "  </policy>\n"
    "</osm>";
const char* OsmApiSampleRequestResponse::SAMPLE_PERMISSIONS_RESPONSE =
    "<?xml version='1.0' encoding='UTF-8'?>\n"
    "<osm version='0.6' generator='OpenStreetMap server'>\n"
    "  <permissions>\n"
    "    <permission name='allow_read_prefs'/>\n"
    "    <permission name='allow_read_gpx'/>\n"
    "    <permission name='allow_write_api'/>\n"
    "    <permission name='allow_write_gpx'/>\n"
    "  </permissions>\n"
    "</osm>";
const char* OsmApiSampleRequestResponse::SAMPLE_CHANGESET_REQUEST =
    "<?xml version='1.0' encoding='UTF-8'?>\n"
    "<osmChange version='0.6' generator='hootenanny'>\n"
    "  <modify>\n"
    "    <way id='1' version='2'>\n"
    "      <nd ref='1'/>\n"
    "      <nd ref='2'/>\n"
    "      <nd ref='3'/>\n"
    "      <tag k='name' v='Way 1'/>\n"
    "      <tag k='hoot:status' v='3'/>\n"
    "    </way>\n"
    "    <way id='2' version='1'>\n"
    "      <nd ref='4'/>\n"
    "      <nd ref='5'/>\n"
    "      <nd ref='6'/>\n"
    "      <tag k='name' v='Way 2'/>\n"
    "      <tag k='hoot:status' v='1'/>\n"
    "    </way>\n"
    "    <way id='3' version='1'>\n"
    "      <nd ref='7'/>\n"
    "      <nd ref='8'/>\n"
    "      <nd ref='9'/>\n"
    "      <tag k='name' v='Way 3'/>\n"
    "      <tag k='hoot:status' v='2'/>\n"
    "    </way>\n"
    "    <way id='4' version='1'>\n"
    "      <nd ref='3'/>\n"
    "      <nd ref='8'/>\n"
    "      <nd ref='7'/>\n"
    "      <tag k='name' v='Way 4'/>\n"
    "      <tag k='hoot:status' v='1'/>\n"
    "    </way>\n"
    "  </modify>\n"
    "</osmChange>";
const char* OsmApiSampleRequestResponse::SAMPLE_CHANGESET_1_RESPONSE =
    "<diffResult generator='OpenStreetMap Server' version='0.6'>\n"
    "  <way old_id='1' new_id='1' new_version='2'/>\n"
    "  <way old_id='2' new_id='2' new_version='2'/>\n"
    "  <way old_id='3' new_id='3' new_version='2'/>\n"
    "  <way old_id='4' new_id='4' new_version='2'/>\n"
    "</diffResult>";
const char* OsmApiSampleRequestResponse::SAMPLE_ELEMENT_1_GET_RESPONSE =
    "<?xml version='1.0' encoding='UTF-8'?>\n"
    "<osm version='0.6' generator='OpenStreetMap server'>\n"
    "  <way id='1' version='1'>\n"
    "    <nd ref='1'/>\n"
    "    <nd ref='2'/>\n"
    "    <nd ref='3'/>\n"
    "    <tag k='name' v='Way 1'/>\n"
    "    <tag k='highway' v='road'/>\n"
    "  </way>\n"
    "</osm>";
}
