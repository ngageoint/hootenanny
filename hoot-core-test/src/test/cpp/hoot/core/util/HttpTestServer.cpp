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

#include "HttpTestServer.h"

//  Hootenanny
#include <hoot/core/util/Log.h>

namespace hoot
{

HttpResponse::HttpResponse(int status, const std::string& response)
  : _status(status),
    _response(response)
{
  //  Add some default HTTP headers
  add_header("Host", "localhost");
  add_header("Connection", "closed");
  if (response != "")
    add_header("Content-Type", "text/xml");
}

void HttpResponse::add_header(const std::string& header, const std::string& value)
{
  //  Add the header to the list of headers we are using if it doesn't already exist
  if (_header_values.find(header) == _header_values.end())
    _headers.push_back(header);
  //  Add the key/value to the map
  _header_values[header] = value;
}

std::string HttpResponse::to_string()
{
  std::string end_line = "\r\n";
  std::stringstream ss;
  //  Add the HTTP status header
  ss << "HTTP/1.1 " << _status << " " << get_status_text() << end_line;
  //  Followed by all the other headers added
  for (std::vector<std::string>::iterator it = _headers.begin(); it != _headers.end(); ++it)
    ss << *it << ": " << _header_values[*it] << end_line;
  //  Responses with response text are given the `Content-Length` header
  if (_response != "")
    ss << "Content-Length: " << _response.size() << end_line;
  //  Output the response body (even if it is an empty string)
  ss << end_line << _response;
  return ss.str();
}

std::string HttpResponse::get_status_text()
{
  //  Convert the HTTP status code to status text
  switch (_status)
  {
  case 200:   return "OK";
  case 400:   return "Bad Request";
  case 404:
  default:    return "Not Found";
  case 405:   return "Method Not Allowed";
  case 409:   return "Conflict";
  }
}


HttpTestServer::HttpTestServer(int port)
  : _port(port),
    _interupt(false)
{
}

void HttpTestServer::start()
{
  //  Kick off the run_server thread
  _thread.reset(new std::thread(std::bind(&HttpTestServer::run_server, this, _port)));
}

void HttpTestServer::wait()
{
  //  Wait for the thread to end
  _thread->join();
}

void HttpTestServer::shutdown()
{
  //  Interupt the threads
  _interupt = true;
  //  Cancel the acceptor
  _acceptor->cancel();
  //  Stop the IO service and wait for the server to stop
  _io_service.stop();
  wait();
}

void HttpTestServer::run_server(int port)
{
  try
  {
    //  Create the acceptor
    _acceptor.reset(new boost::asio::ip::tcp::acceptor(_io_service,
      boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)));
    //  Start accepting connections
    start_accept();
    //  Run the IO service
    _io_service.run();
  }
  catch (std::exception& e)
  {
    LOG_ERROR(e.what());
    shutdown();
  }
}

void HttpTestServer::start_accept()
{
  //  Service the interupt
  if (_interupt)
    return;
  //  Creat the connection
  HttpConnection::HttpConnectionPtr new_connection(new HttpConnection(_acceptor->get_io_service()));
  //  Accept connections async
  _acceptor->async_accept(new_connection->socket(),
    boost::bind(&HttpTestServer::handle_accept, this, new_connection, boost::asio::placeholders::error));
}

void HttpTestServer::handle_accept(HttpConnection::HttpConnectionPtr new_connection, const boost::system::error_code& error)
{
  //  Call the overridden respond() function
  bool continue_processing = !error;
  if (!error)
    continue_processing = respond(new_connection) && !_interupt;
  //  Continue processing connections
  if (continue_processing)
    start_accept();
  else
  {
    _interupt = true;
    _io_service.stop();
  }
}

bool HttpTestServer::respond(HttpConnection::HttpConnectionPtr& connection)
{
  //  Read the HTTP request, and ignore them
  read_request_headers(connection);
  //  Respond with HTTP 200 OK
  HttpResponse response(200);
  //  Write out the response
  write_response(connection, response.to_string());
  //  Return true if we should continue listening and processing requests
  return !_interupt;
}

std::string HttpTestServer::read_request_headers(HttpConnection::HttpConnectionPtr& connection)
{
  //  Read the HTTP request headers
  boost::asio::streambuf buf;
  boost::asio::read_until(connection->socket(), buf, "\r\n\r\n");
  return boost::asio::buffer_cast<const char*>(buf.data());
}

std::string HttpTestServer::read_request_body(const std::string& headers, HttpConnection::HttpConnectionPtr& connection)
{
  //  Parse the headers to get the content length
  long content_length = parse_content_length(headers);
  std::vector<char> buf(content_length);
  //  Read the HTTP request body
  boost::asio::read(connection->socket(), boost::asio::buffer(buf));
  return buf.data();
}

void HttpTestServer::write_response(HttpConnection::HttpConnectionPtr& connection, const std::string& response)
{
  LOG_TRACE("Response:\n" << response);
  //  Write the response to the socket synchronously
  boost::asio::write(connection->socket(), boost::asio::buffer(response));
  connection->socket().close();
}

long HttpTestServer::parse_content_length(const std::string& headers)
{
  try
  {
    const std::string content_length = "Content-Length: ";
    //  Find the beginning of the string
    std::size_t start = headers.find(content_length);
    if (start != std::string::npos)
    {
      //
      start += content_length.length();
      //  Find the next end of line
      std::size_t end = headers.find("\r\n", start);
      if (end != std::string::npos)
      {
        std::string value = headers.substr(start, end - start);
        return std::stol(value);
      }
    }
  }
  catch(...) {}
  return 0;
}

}
