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
#include <hoot/core/util/HootNetworkUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/OsmApiUtils.h>

namespace hoot
{

bool CapabilitiesTestServer::respond(HttpConnection::HttpConnectionPtr &connection)
{
  //  Read the HTTP request headers
  std::string headers = read_request_headers(connection);
  //  Make sure that the capabilities were requested
  HttpResponsePtr response;
  if (headers.find(OsmApiEndpoints::API_PATH_CAPABILITIES) != std::string::npos)
    response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, OsmApiSampleRequestResponse::SAMPLE_CAPABILITIES_RESPONSE));
  else
    response.reset(new HttpResponse(HttpResponseCode::HTTP_NOT_FOUND));
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
  if (headers.find(OsmApiEndpoints::API_PATH_PERMISSIONS) != std::string::npos)
    response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, OsmApiSampleRequestResponse::SAMPLE_PERMISSIONS_RESPONSE));
  else
    response.reset(new HttpResponse(HttpResponseCode::HTTP_NOT_FOUND));
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
  if (headers.find(OsmApiEndpoints::API_PATH_CAPABILITIES) != std::string::npos)
    response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, OsmApiSampleRequestResponse::SAMPLE_CAPABILITIES_RESPONSE));
  else if (headers.find(OsmApiEndpoints::API_PATH_PERMISSIONS) != std::string::npos)
    response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, OsmApiSampleRequestResponse::SAMPLE_PERMISSIONS_RESPONSE));
  else if (headers.find(OsmApiEndpoints::API_PATH_CREATE_CHANGESET) != std::string::npos)
    response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, "1"));
  else if (headers.find("POST") != std::string::npos)
  {
    response.reset(new HttpResponse(HttpResponseCode::HTTP_METHOD_NOT_ALLOWED));
    response->add_header("Allow", "GET");
  }
  else if (headers.find(QString(OsmApiEndpoints::API_PATH_CLOSE_CHANGESET).arg(1).toStdString()) != std::string::npos)
  {
    response.reset(new HttpResponse(HttpResponseCode::HTTP_OK));
    continue_processing = false;
  }
  else
  {
    //  Error out here
    response.reset(new HttpResponse(HttpResponseCode::HTTP_NOT_FOUND));
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
  if (headers.find(OsmApiEndpoints::API_PATH_CAPABILITIES) != std::string::npos)
    response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, OsmApiSampleRequestResponse::SAMPLE_CAPABILITIES_RESPONSE));
  //  Permissions
  else if (headers.find(OsmApiEndpoints::API_PATH_PERMISSIONS) != std::string::npos)
    response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, OsmApiSampleRequestResponse::SAMPLE_PERMISSIONS_RESPONSE));
  //  Create changeset
  else if (headers.find(OsmApiEndpoints::API_PATH_CREATE_CHANGESET) != std::string::npos)
    response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, "1"));
  //  Upload changeset 1
  else if (headers.find(QString(OsmApiEndpoints::API_PATH_UPLOAD_CHANGESET).arg(1).toStdString()) != std::string::npos)
  {
    //  The first time through, the 'version' of element 1 should fail.
    if (!_has_error)
    {
      response.reset(new HttpResponse(HttpResponseCode::HTTP_CONFLICT, "Changeset conflict: Version mismatch: Provided 2, server had: 1 of Way 1"));
      _has_error = true;
    }
    else
      response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, OsmApiSampleRequestResponse::SAMPLE_CHANGESET_1_RESPONSE));
  }
  //  Get way 1's updated version
  else if (headers.find(QString(OsmApiEndpoints::API_PATH_GET_ELEMENT).arg("way").arg(1).toStdString()) != std::string::npos)
    response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, OsmApiSampleRequestResponse::SAMPLE_ELEMENT_1_GET_RESPONSE));
  //  Close changeset
  else if (headers.find(QString(OsmApiEndpoints::API_PATH_CLOSE_CHANGESET).arg(1).toStdString()) != std::string::npos)
  {
    response.reset(new HttpResponse(HttpResponseCode::HTTP_OK));
    continue_processing = false;
  }
  else
  {
    //  Error out here
    response.reset(new HttpResponse(HttpResponseCode::HTTP_NOT_FOUND));
    continue_processing = false;
  }
  //  Write out the response
  write_response(connection, response->to_string());
  //  Return true if we should continue listening and processing requests
  return continue_processing && !get_interupt();
}

bool ChangesetOutputTestServer::respond(HttpConnection::HttpConnectionPtr& connection)
{
  //  Stop processing by setting this to false
  bool continue_processing = true;
  //  Read the HTTP request headers
  std::string headers = read_request_headers(connection);
  //  Determine the response message's HTTP header
  HttpResponsePtr response;
  if (headers.find(OsmApiEndpoints::API_PATH_CAPABILITIES) != std::string::npos)
    response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, OsmApiSampleRequestResponse::SAMPLE_CAPABILITIES_RESPONSE));
  else if (headers.find(OsmApiEndpoints::API_PATH_PERMISSIONS) != std::string::npos)
    response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, OsmApiSampleRequestResponse::SAMPLE_PERMISSIONS_RESPONSE));
  else if (headers.find(OsmApiEndpoints::API_PATH_CREATE_CHANGESET) != std::string::npos)
    response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, "1"));
  else if (headers.find("POST") != std::string::npos)
  {
    //  Read the HTTP request body to figure out which response to send back
    std::string body = read_request_body(headers, connection);
    if (body.find("way id=\"1\"") != std::string::npos)
      response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, OsmApiSampleRequestResponse::SAMPLE_CHANGESET_SUCCESS_1_RESPONSE));
    else
      response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, OsmApiSampleRequestResponse::SAMPLE_CHANGESET_SUCCESS_2_RESPONSE));
  }
  else if (headers.find(QString(OsmApiEndpoints::API_PATH_CLOSE_CHANGESET).arg(1).toStdString()) != std::string::npos)
  {
    response.reset(new HttpResponse(HttpResponseCode::HTTP_OK));
    continue_processing = false;
  }
  else
  {
    //  Error out here
    response.reset(new HttpResponse(HttpResponseCode::HTTP_NOT_FOUND));
    continue_processing = false;
  }
  //  Write out the response
  write_response(connection, response->to_string());
  //  Return true if we should continue listening and processing requests
  return continue_processing && !get_interupt();
}

bool ChangesetOutputThrottleTestServer::respond(HttpConnection::HttpConnectionPtr& connection)
{
  //  Stop processing by setting this to false
  bool continue_processing = true;
  //  Read the HTTP request headers
  std::string headers = read_request_headers(connection);
  //  Determine the response message's HTTP header
  HttpResponsePtr response;
  if (headers.find(OsmApiEndpoints::API_PATH_CAPABILITIES) != std::string::npos)
    response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, OsmApiSampleRequestResponse::SAMPLE_CAPABILITIES_RESPONSE));
  else if (headers.find(OsmApiEndpoints::API_PATH_PERMISSIONS) != std::string::npos)
    response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, OsmApiSampleRequestResponse::SAMPLE_PERMISSIONS_RESPONSE));
  else if (headers.find(OsmApiEndpoints::API_PATH_MAP) != std::string::npos)
    response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, OsmApiSampleRequestResponse::SAMPLE_CGIMAP_RESPONSE));
  else if (headers.find(OsmApiEndpoints::API_PATH_CREATE_CHANGESET) != std::string::npos)
    response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, "1"));
  else if (headers.find("POST") != std::string::npos)
  {
    //  Read the HTTP request body to figure out which response to send back
    std::string body = read_request_body(headers, connection);
    if (body.find("way id=\"1\"") != std::string::npos)
      response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, OsmApiSampleRequestResponse::SAMPLE_CHANGESET_SUCCESS_1_RESPONSE));
    else
      response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, OsmApiSampleRequestResponse::SAMPLE_CHANGESET_SUCCESS_2_RESPONSE));
  }
  else if (headers.find(QString(OsmApiEndpoints::API_PATH_CLOSE_CHANGESET).arg(1).toStdString()) != std::string::npos)
  {
    response.reset(new HttpResponse(HttpResponseCode::HTTP_OK));
    continue_processing = false;
  }
  else
  {
    //  Error out here
    response.reset(new HttpResponse(HttpResponseCode::HTTP_NOT_FOUND));
    continue_processing = false;
  }
  //  Write out the response
  write_response(connection, response->to_string());
  //  Return true if we should continue listening and processing requests
  return continue_processing && !get_interupt();
}

bool ChangesetCreateFailureTestServer::respond(HttpConnection::HttpConnectionPtr& connection)
{
  //  Stop processing by setting this to false
  bool continue_processing = true;
  //  Read the HTTP request headers
  std::string headers = read_request_headers(connection);
  //  Determine the response message's HTTP header
  HttpResponsePtr response;
  if (headers.find(OsmApiEndpoints::API_PATH_CAPABILITIES) != std::string::npos)
    response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, OsmApiSampleRequestResponse::SAMPLE_CAPABILITIES_RESPONSE));
  else if (headers.find(OsmApiEndpoints::API_PATH_PERMISSIONS) != std::string::npos)
    response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, OsmApiSampleRequestResponse::SAMPLE_PERMISSIONS_RESPONSE));
  else if (headers.find(OsmApiEndpoints::API_PATH_CREATE_CHANGESET) != std::string::npos)
  {
    static int count = 0;
    response.reset(new HttpResponse(HttpResponseCode::HTTP_UNAUTHORIZED, "User is not authorized"));
    if (++count >= 3)
      continue_processing = false;
  }
  else
  {
    //  Error out here
    response.reset(new HttpResponse(HttpResponseCode::HTTP_NOT_FOUND));
    continue_processing = false;
  }
  //  Write out the response
  write_response(connection, response->to_string());
  //  Return true if we should continue listening and processing requests
  return continue_processing && !get_interupt();
}

bool CreateWaysFailNodesTestServer::respond(HttpConnection::HttpConnectionPtr& connection)
{
  //  Stop processing by setting this to false
  bool continue_processing = true;
  //  Read the HTTP request headers
  std::string headers = read_request_headers(connection);
  //  Determine the response message's HTTP header
  HttpResponsePtr response;
  if (headers.find(OsmApiEndpoints::API_PATH_CAPABILITIES) != std::string::npos)
    response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, OsmApiSampleRequestResponse::SAMPLE_CAPABILITIES_RESPONSE));
  else if (headers.find(OsmApiEndpoints::API_PATH_PERMISSIONS) != std::string::npos)
    response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, OsmApiSampleRequestResponse::SAMPLE_PERMISSIONS_RESPONSE));
  else if (headers.find(OsmApiEndpoints::API_PATH_CREATE_CHANGESET) != std::string::npos)
    response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, "1"));
  else if (headers.find("POST") != std::string::npos)
  {
    //  Read the HTTP request body to figure out which response to send back
    std::string body = read_request_body(headers, connection);
    if (body.find("way id=\"-1\"") != std::string::npos)
      response.reset(new HttpResponse(HttpResponseCode::HTTP_PRECONDITION_FAILED, OsmApiSampleRequestResponse::SAMPLE_CHANGESET_FAILURE_RESPONSE_1));
    else
      response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, OsmApiSampleRequestResponse::SAMPLE_CHANGESET_FAILURE_RESPONSE_2));
  }
  else if (headers.find(QString(OsmApiEndpoints::API_PATH_CLOSE_CHANGESET).arg(1).toStdString()) != std::string::npos)
  {
    response.reset(new HttpResponse(HttpResponseCode::HTTP_OK));
    continue_processing = false;
  }
  else
  {
    //  Error out here
    response.reset(new HttpResponse(HttpResponseCode::HTTP_NOT_FOUND));
    continue_processing = false;
  }
  //  Write out the response
  write_response(connection, response->to_string());
  //  Return true if we should continue listening and processing requests
  return continue_processing && !get_interupt();
}

bool VersionFailureTestServer::respond(HttpConnection::HttpConnectionPtr& connection)
{
  static int version = 1;
  //  Stop processing by setting this to false
  bool continue_processing = true;
  //  Read the HTTP request headers
  std::string headers = read_request_headers(connection);
  //  Determine the response message's HTTP header
  HttpResponsePtr response;
  if (headers.find(OsmApiEndpoints::API_PATH_CAPABILITIES) != std::string::npos)
    response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, OsmApiSampleRequestResponse::SAMPLE_CAPABILITIES_RESPONSE));
  else if (headers.find(OsmApiEndpoints::API_PATH_PERMISSIONS) != std::string::npos)
    response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, OsmApiSampleRequestResponse::SAMPLE_PERMISSIONS_RESPONSE));
  else if (headers.find(OsmApiEndpoints::API_PATH_CREATE_CHANGESET) != std::string::npos)
    response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, "1"));
  else if (headers.find("POST") != std::string::npos)
  {
    //  Increment the ID each time so that it will constantly fail until OsmApiWriter gives up
    int v = version;
    response.reset(new HttpResponse(HttpResponseCode::HTTP_CONFLICT,
                   QString(OsmApiSampleRequestResponse::SAMPLE_CHANGESET_VERSION_FAILURE_RESPONSE).arg(v).arg(++version).toStdString()));
  }
  else if (headers.find(QString(OsmApiEndpoints::API_PATH_GET_ELEMENT).arg("way").arg(1).toStdString()) != std::string::npos)
  {
    //  Update the GET response with a different ID
    response.reset(new HttpResponse(HttpResponseCode::HTTP_OK,
                   QString(OsmApiSampleRequestResponse::SAMPLE_CHANGESET_VERSION_FAILURE_GET_RESPONSE).arg(++version).toStdString()));
  }
  else if (headers.find(QString(OsmApiEndpoints::API_PATH_CLOSE_CHANGESET).arg(1).toStdString()) != std::string::npos)
  {
    response.reset(new HttpResponse(HttpResponseCode::HTTP_OK));
    continue_processing = false;
  }
  else
  {
    //  Error out here
    response.reset(new HttpResponse(HttpResponseCode::HTTP_NOT_FOUND));
    continue_processing = false;
  }
  //  Write out the response
  write_response(connection, response->to_string());
  //  Return true if we should continue listening and processing requests
  return continue_processing && !get_interupt();
}

bool ElementGoneTestServer::respond(HttpConnection::HttpConnectionPtr& connection)
{
  //  Stop processing by setting this to false
  bool continue_processing = true;
  //  Read the HTTP request headers
  std::string headers = read_request_headers(connection);
  //  Determine the response message's HTTP header
  HttpResponsePtr response;
  if (headers.find(OsmApiEndpoints::API_PATH_CAPABILITIES) != std::string::npos)
    response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, OsmApiSampleRequestResponse::SAMPLE_CAPABILITIES_RESPONSE));
  else if (headers.find(OsmApiEndpoints::API_PATH_PERMISSIONS) != std::string::npos)
    response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, OsmApiSampleRequestResponse::SAMPLE_PERMISSIONS_RESPONSE));
  else if (headers.find(OsmApiEndpoints::API_PATH_MAP) != std::string::npos)
    response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, OsmApiSampleRequestResponse::SAMPLE_CGIMAP_RESPONSE));
  else if (headers.find(OsmApiEndpoints::API_PATH_CREATE_CHANGESET) != std::string::npos)
    response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, "1"));
  else if (headers.find("POST") != std::string::npos)
  {
    //  Read the HTTP request body to figure out which response to send back
    std::string body = read_request_body(headers, connection);
    if (body.find("node id=\"40\"") != std::string::npos)
      response.reset(new HttpResponse(HttpResponseCode::HTTP_GONE, OsmApiSampleRequestResponse::SAMPLE_ELEMENT_GONE_RESPONSE));
    else
      response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, OsmApiSampleRequestResponse::SAMPLE_CHANGESET_1_RESPONSE));
  }
  else if (headers.find(QString(OsmApiEndpoints::API_PATH_CLOSE_CHANGESET).arg(1).toStdString()) != std::string::npos)
  {
    response.reset(new HttpResponse(HttpResponseCode::HTTP_OK));
    continue_processing = false;
  }
  else
  {
    //  Error out here
    response.reset(new HttpResponse(HttpResponseCode::HTTP_NOT_FOUND));
    continue_processing = false;
  }
  //  Write out the response
  write_response(connection, response->to_string());
  //  Return true if we should continue listening and processing requests
  return continue_processing && !get_interupt();
}


bool ChangesetSplitDeleteTestServer::respond(HttpConnection::HttpConnectionPtr& connection)
{
  //  Stop processing by setting this to false
  bool continue_processing = true;
  //  Read the HTTP request headers
  std::string headers = read_request_headers(connection);
  //  Determine the response message's HTTP header
  HttpResponsePtr response;
  if (headers.find(OsmApiEndpoints::API_PATH_CAPABILITIES) != std::string::npos)
    response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, OsmApiSampleRequestResponse::SAMPLE_CAPABILITIES_RESPONSE));
  else if (headers.find(OsmApiEndpoints::API_PATH_PERMISSIONS) != std::string::npos)
    response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, OsmApiSampleRequestResponse::SAMPLE_PERMISSIONS_RESPONSE));
  else if (headers.find(OsmApiEndpoints::API_PATH_MAP) != std::string::npos)
    response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, OsmApiSampleRequestResponse::SAMPLE_CGIMAP_RESPONSE));
  else if (headers.find(OsmApiEndpoints::API_PATH_CREATE_CHANGESET) != std::string::npos)
    response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, "1"));
  else if (headers.find("POST") != std::string::npos)
  {
    //  Read the HTTP request body to figure out which response to send back
    std::string body = read_request_body(headers, connection);
    LOG_INFO(body);
    if (body.find("way id=\"1\"") != std::string::npos && body.find("node id=\"1\"") != std::string::npos)
      response.reset(new HttpResponse(HttpResponseCode::HTTP_GONE, OsmApiSampleRequestResponse::SAMPLE_CHANGESET_SPLIT_FAILED_RESPONSE));
    else if (body.find("node id=\"1\"") != std::string::npos)
      response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, OsmApiSampleRequestResponse::SAMPLE_CHANGESET_SPLIT_SUCCESS_RESPONSE_1));
    else if (body.find("node id=\"") == std::string::npos)
      response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, OsmApiSampleRequestResponse::SAMPLE_CHANGESET_SPLIT_SUCCESS_RESPONSE_2));
    else
      response.reset(new HttpResponse(HttpResponseCode::HTTP_OK, OsmApiSampleRequestResponse::SAMPLE_CHANGESET_SPLIT_SUCCESS_RESPONSE_3));
  }
  else if (headers.find(QString(OsmApiEndpoints::API_PATH_CLOSE_CHANGESET).arg(1).toStdString()) != std::string::npos)
  {
    response.reset(new HttpResponse(HttpResponseCode::HTTP_OK));
    continue_processing = false;
  }
  else
  {
    //  Error out here
    response.reset(new HttpResponse(HttpResponseCode::HTTP_NOT_FOUND));
    continue_processing = false;
  }
  //  Write out the response
  write_response(connection, response->to_string());
  //  Return true if we should continue listening and processing requests
  return continue_processing && !get_interupt();
}

/** OsmApiSampleRequestResponse values */
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
    "<?xml version='1.0' encoding='UTF-8'?>\n"
    "<diffResult generator='OpenStreetMap Server' version='0.6'>\n"
    "  <way old_id='1' new_id='1' new_version='2'/>\n"
    "  <way old_id='2' new_id='2' new_version='2'/>\n"
    "  <way old_id='3' new_id='3' new_version='2'/>\n"
    "  <way old_id='4' new_id='4' new_version='2'/>\n"
    "</diffResult>";
const char* OsmApiSampleRequestResponse::SAMPLE_CGIMAP_RESPONSE =
    "<?xml version='1.0' encoding='UTF-8'?>\n"
    "<osm generator='CGImap 0.8.3' version='0.6'>\n"
    "  <bounds minlat='38.9600315' minlon='-77.4224954' maxlat='38.9600315' maxlon='-77.4224954'/>\n"
    "  <node id='1' visible='true' version='1' changeset='1' lat='38.9600315' lon='-77.4224954'>\n"
    "    <tag k='addr:city' v='Herndon'/>\n"
    "    <tag k='addr:housenumber' v='2325'/>\n"
    "    <tag k='addr:postcode' v='20171'/>\n"
    "    <tag k='addr:street' v='Dulles Corner Boulevard'/>\n"
    "    <tag k='facility:id' v='37489576'/>\n"
    "    <tag k='name' v='Maxar'/>\n"
    "    <tag k='office' v='company'/>\n"
    "    <tag k='website' v='https://www.maxar.com/'/>\n"
    "  </node>\n"
    "</osm>";
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
const char* OsmApiSampleRequestResponse::SAMPLE_CHANGESET_SUCCESS_1_RESPONSE =
    "<?xml version='1.0' encoding='UTF-8'?>\n"
    "<diffResult generator='OpenStreetMap Server' version='0.6'>\n"
    "  <way old_id='1' new_id='1' new_version='2'/>\n"
    "  <way old_id='2' new_id='2' new_version='2'/>\n"
    "</diffResult>";
const char* OsmApiSampleRequestResponse::SAMPLE_CHANGESET_SUCCESS_2_RESPONSE =
    "<?xml version='1.0' encoding='UTF-8'?>\n"
    "<diffResult generator='OpenStreetMap Server' version='0.6'>\n"
    "  <way old_id='3' new_id='3' new_version='2'/>\n"
    "  <way old_id='4' new_id='4' new_version='2'/>\n"
    "</diffResult>";
const char* OsmApiSampleRequestResponse::SAMPLE_CHANGESET_FAILURE_RESPONSE_1 =
    "Precondition failed: Way -1 requires the nodes with id in 111111111111, which either do not exist, or are not visible.";
const char* OsmApiSampleRequestResponse::SAMPLE_CHANGESET_FAILURE_RESPONSE_2 =
    "<?xml version='1.0' encoding='UTF-8'?>\n"
    "<diffResult generator='OpenStreetMap Server' version='0.6'>\n"
    " <node old_id='-2' new_id='1' new_version='1'/>\n"
    " <node old_id='-3' new_id='2' new_version='1'/>\n"
    " <node old_id='-4' new_id='3' new_version='1'/>\n"
    " <node old_id='-5' new_id='4' new_version='1'/>\n"
    " <node old_id='-6' new_id='5' new_version='1'/>\n"
    " <node old_id='-7' new_id='6' new_version='1'/>\n"
    " <node old_id='-8' new_id='7' new_version='1'/>\n"
    " <node old_id='-9' new_id='8' new_version='1'/>\n"
    " <node old_id='-10' new_id='9' new_version='1'/>\n"
    " <node old_id='-11' new_id='10' new_version='1'/>\n"
    " <node old_id='-12' new_id='11' new_version='1'/>\n"
    " <node old_id='-13' new_id='12' new_version='1'/>\n"
    " <node old_id='-14' new_id='13' new_version='1'/>\n"
    " <node old_id='-15' new_id='14' new_version='1'/>\n"
    " <node old_id='-16' new_id='15' new_version='1'/>\n"
    " <node old_id='-17' new_id='16' new_version='1'/>\n"
    " <node old_id='-18' new_id='17' new_version='1'/>\n"
    " <node old_id='-19' new_id='18' new_version='1'/>\n"
    " <node old_id='-20' new_id='19' new_version='1'/>\n"
    " <node old_id='-21' new_id='20' new_version='1'/>\n"
    " <node old_id='-22' new_id='21' new_version='1'/>\n"
    " <node old_id='-23' new_id='22' new_version='1'/>\n"
    " <node old_id='-24' new_id='23' new_version='1'/>\n"
    " <node old_id='-25' new_id='24' new_version='1'/>\n"
    " <node old_id='-26' new_id='25' new_version='1'/>\n"
    " <node old_id='-27' new_id='26' new_version='1'/>\n"
    " <node old_id='-28' new_id='27' new_version='1'/>\n"
    " <node old_id='-29' new_id='28' new_version='1'/>\n"
    " <node old_id='-30' new_id='29' new_version='1'/>\n"
    " <node old_id='-31' new_id='30' new_version='1'/>\n"
    " <way old_id='-2' new_id='1' new_version='1'/>\n"
    "</diffResult>";
const char* OsmApiSampleRequestResponse::SAMPLE_CHANGESET_VERSION_FAILURE_CHANGESET =
    "<?xml version='1.0' encoding='UTF-8'?>\n"
    "<osmChange version='0.6' generator='hootenanny'>\n"
    " <modify>\n"
    "  <way id='1' version='2' timestamp=''>\n"
    "   <nd ref='1'/>\n"
    "   <nd ref='2'/>\n"
    "   <nd ref='3'/>\n"
    "   <tag k='amenity' v='restaurant'/>\n"
    "   <tag k='name' v='Pizza Shop'/>\n"
    "   <tag k='building' v='yes'/>\n"
    "  </way>\n"
    " </modify>\n"
    "</osmChange>";
const char* OsmApiSampleRequestResponse::SAMPLE_CHANGESET_VERSION_FAILURE_RESPONSE =
    "Changeset conflict: Version mismatch: Provided %1, server had: %2 of Way 1";
const char* OsmApiSampleRequestResponse::SAMPLE_CHANGESET_VERSION_FAILURE_GET_RESPONSE =
    "<?xml version='1.0' encoding='UTF-8'?>\n"
    "<osm version='0.6' generator='hootenanny'>\n"
    " <way id='1' version='%1' timestamp=''>\n"
    "  <nd ref='1'/>\n"
    "  <nd ref='2'/>\n"
    "  <nd ref='3'/>\n"
    "  <tag k='amenity' v='restaurant'/>\n"
    "  <tag k='name' v='Pizza Shop'/>\n"
    "  <tag k='building' v='yes'/>\n"
    " </way>\n"
    "</osm>";
const char* OsmApiSampleRequestResponse::SAMPLE_ELEMENT_GONE_RESPONSE =
    "The node with the id 40 has already been deleted";
const char* OsmApiSampleRequestResponse::SAMPLE_CHANGESET_SPLIT_DELETE =
    "<?xml version='1.0' encoding='UTF-8'?>\n"
    "<osmChange version='0.6' generator='hootenanny'>\n"
    " <modify>\n"
    "  <way id='1' version='1' timestamp=''>\n"
    "   <nd ref='11'/>\n"
    "   <nd ref='12'/>\n"
    "   <tag k='name' v='Some road'/>\n"
    "  </way>\n"
    "  <way id='2' version='1' timestamp=''>\n"
    "   <nd ref='12'/>\n"
    "   <nd ref='13'/>\n"
    "   <tag k='name' v='Some road'/>\n"
    "  </way>\n"
    "  <way id='3' version='1' timestamp=''>\n"
    "   <nd ref='13'/>\n"
    "   <nd ref='14'/>\n"
    "   <tag k='name' v='Some road'/>\n"
    "  </way>\n"
    "  <way id='4' version='1' timestamp=''>\n"
    "   <nd ref='14'/>\n"
    "   <nd ref='15'/>\n"
    "   <tag k='name' v='Some road'/>\n"
    "  </way>\n"
    " </modify>\n"
    " <delete>\n"
    "  <node id='1' visible='true' version='1' changeset='1' lat='38.9500315' lon='-77.4224954'/>\n"
    "  <node id='2' visible='true' version='1' changeset='1' lat='38.9600315' lon='-77.4224954'/>\n"
    "  <node id='3' visible='true' version='1' changeset='1' lat='38.9700315' lon='-77.4224954'/>\n"
    "  <node id='4' visible='true' version='1' changeset='1' lat='38.9600315' lon='-77.4324954'/>\n"
    "  <node id='5' visible='true' version='1' changeset='1' lat='38.9600315' lon='-77.4124954'/>\n"
    "  <way id='5' version='1' timestamp=''>\n"
    "   <nd ref='1'/>\n"
    "   <nd ref='2'/>\n"
    "   <nd ref='3'/>\n"
    "   <tag k='name' v='Road 1'/>\n"
    "  </way>\n"
    "  <way id='6' version='1' timestamp=''>\n"
    "   <nd ref='4'/>\n"
    "   <nd ref='2'/>\n"
    "   <nd ref='5'/>\n"
    "   <tag k='name' v='Road 2'/>\n"
    "  </way>\n"
    " </delete>\n"
    "</osmChange>";
const char* OsmApiSampleRequestResponse::SAMPLE_CHANGESET_SPLIT_FAILED_RESPONSE =
    "Precondition failed";
const char* OsmApiSampleRequestResponse::SAMPLE_CHANGESET_SPLIT_SUCCESS_RESPONSE_1 =
    "<?xml version='1.0' encoding='UTF-8'?>\n"
    "<diffResult generator='OpenStreetMap Server' version='0.6'>\n"
    "  <way old_id='5'/>\n"
    "  <node old_id='1'/>\n"
    "  <node old_id='3'/>\n"
    "</diffResult>";
const char* OsmApiSampleRequestResponse::SAMPLE_CHANGESET_SPLIT_SUCCESS_RESPONSE_2 =
    "<?xml version='1.0' encoding='UTF-8'?>\n"
    "<diffResult generator='OpenStreetMap Server' version='0.6'>\n"
    "  <way old_id='1' new_id='1' new_version='2'/>\n"
    "  <way old_id='2' new_id='2' new_version='2'/>\n"
    "  <way old_id='3' new_id='3' new_version='2'/>\n"
    "  <way old_id='4' new_id='4' new_version='2'/>\n"
    "  <way old_id='6'/>\n"
    "</diffResult>";
const char* OsmApiSampleRequestResponse::SAMPLE_CHANGESET_SPLIT_SUCCESS_RESPONSE_3 =
    "<?xml version='1.0' encoding='UTF-8'?>\n"
    "<diffResult generator='OpenStreetMap Server' version='0.6'>\n"
    "  <node old_id='2'/>\n"
    "  <node old_id='4'/>\n"
    "  <node old_id='5'/>\n"
    "</diffResult>";
}
