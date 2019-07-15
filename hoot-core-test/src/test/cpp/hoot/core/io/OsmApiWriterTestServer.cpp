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

//  Standard
#include <memory>

//  Boost
#include <boost/bind.hpp>
#include <boost/asio.hpp>

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

HttpConnection::HttpConnection(boost::asio::io_service& io_service)
  : _socket(io_service)
{
}

void HttpConnection::handle_write(const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/)
{
}

HttpConnection::HttpConnectionPtr HttpConnection::create(boost::asio::io_service& io_service)
{
  //  Create the connection object
  return HttpConnectionPtr(new HttpConnection(io_service));
}

boost::asio::ip::tcp::socket& HttpConnection::socket()
{
  return _socket;
}

bool HttpConnection::start()
{
  //  Stop processing by setting this to false
  bool continue_processing = true;
  //  Read the HTTP request
  boost::asio::streambuf buf;
  boost::asio::read_until(_socket, buf, "\r\n\r\n");
  std::string input = boost::asio::buffer_cast<const char*>(buf.data());
  //  Determine the response message's HTTP header
  std::string message;
  if (input.find(OsmApiWriter::API_PATH_CAPABILITIES) != std::string::npos)
    message = std::string("HTTP/1.1 200 OK\r\n\r\n") + OsmApiSampleResponses::SAMPLE_CAPABILITIES + "\r\n";
  else if (input.find(OsmApiWriter::API_PATH_PERMISSIONS) != std::string::npos)
    message = std::string("HTTP/1.1 200 OK\r\n\r\n") + OsmApiSampleResponses::SAMPLE_PERMISSIONS + "\r\n";
  else if (input.find(OsmApiWriter::API_PATH_CREATE_CHANGESET) != std::string::npos)
    message = std::string("HTTP/1.1 200 OK\r\n\r\n1\r\n");
  else if (input.find("POST") != std::string::npos)
    message = std::string("HTTP/1.1 405 Method Not Allowed\r\nAllow: GET\r\n\r\n");
  else if (input.find("/close/"))
  {
    message = std::string("HTTP/1.1 200 OK\r\n\r\n");
    continue_processing = false;
  }
  //  Write out the response async
  boost::asio::async_write(_socket, boost::asio::buffer(message),
                           boost::bind(&HttpConnection::handle_write, shared_from_this(),
                                       boost::asio::placeholders::error,
                                       boost::asio::placeholders::bytes_transferred));
  return continue_processing;
}

const int HttpTestServer::TEST_SERVER_PORT = 8910;

std::shared_ptr<std::thread> HttpTestServer::start()
{
  return std::shared_ptr<std::thread>(new std::thread(&HttpTestServer::run_server));
}

HttpTestServer::HttpTestServer(boost::asio::io_service& io_service)
  : _acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), TEST_SERVER_PORT))
{
  start_accept();
}

void HttpTestServer::run_server()
{
  try
  {
    boost::asio::io_service io_service;
    HttpTestServer server(io_service);
    io_service.run();
  }
  catch (std::exception& e)
  {
    LOG_ERROR(e.what());
  }
}

void HttpTestServer::start_accept()
{
  HttpConnection::HttpConnectionPtr new_connection = HttpConnection::create(_acceptor.get_io_service());
  _acceptor.async_accept(new_connection->socket(),
                         boost::bind(&HttpTestServer::handle_accept, this, new_connection, boost::asio::placeholders::error));
}

void HttpTestServer::handle_accept(HttpConnection::HttpConnectionPtr new_connection, const boost::system::error_code& error)
{
  bool continue_processing = true;
  if (!error)
    continue_processing = new_connection->start();

  if (continue_processing)
    start_accept();
}

}
