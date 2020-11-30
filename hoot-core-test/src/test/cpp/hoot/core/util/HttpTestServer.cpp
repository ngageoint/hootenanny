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

#include "HttpTestServer.h"

//  Hootenanny
#include <hoot/core/util/HootNetworkUtils.h>
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
    add_header("Content-Type", HootNetworkUtils::CONTENT_TYPE_XML);
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
  case HttpResponseCode::HTTP_OK:                     return "OK";
  case HttpResponseCode::HTTP_BAD_REQUEST:            return "Bad Request";
  case HttpResponseCode::HTTP_UNAUTHORIZED:           return "Unauthorized";
  case HttpResponseCode::HTTP_NOT_FOUND:              return "Not Found";
  case HttpResponseCode::HTTP_METHOD_NOT_ALLOWED:     return "Method Not Allowed";
  case HttpResponseCode::HTTP_CONFLICT:               return "Conflict";
  case HttpResponseCode::HTTP_GONE:                   return "Gone";
  case HttpResponseCode::HTTP_PRECONDITION_FAILED:    return "Precondition Failed";
  case HttpResponseCode::HTTP_INTERNAL_SERVER_ERROR:  return "Internal Server Error";
  case HttpResponseCode::HTTP_BAD_GATEWAY:            return "Bad Gateway";
  case HttpResponseCode::HTTP_SERVICE_UNAVAILABLE:    return "Service Unavailable";
  case HttpResponseCode::HTTP_GATEWAY_TIMEOUT:        return "Gateway Timeout";
  case HttpResponseCode::HTTP_BANDWIDTH_EXCEEDED:     return "Bandwidth Limit Exceeded";
  default:                                            return "Unknown Status";
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

void HttpTestServer::stop()
{
  //  Interupt the threads
  _interupt = true;
  //  Close the acceptor
  boost::system::error_code ec;
  _acceptor->close(ec);
  //  Stop the IO service
  _io_service.stop();
}

void HttpTestServer::shutdown()
{
  //  Stop the server
  stop();
  //  Wait for the server to shutdown
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
    //  Stop the server here, do not `shutdown`
    //  Joining the thread inside of the thread causes resource deadlocks
    stop();
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
    stop();
}

bool HttpTestServer::respond(HttpConnection::HttpConnectionPtr& connection)
{
  //  Read the HTTP request
  parse_request(connection);
  //  Respond with HTTP 200 OK
  HttpResponse response(HttpResponseCode::HTTP_OK);
  //  Write out the response
  write_response(connection, response.to_string());
  //  Return true if we should continue listening and processing requests
  return !_interupt;
}

void HttpTestServer::parse_request(HttpConnection::HttpConnectionPtr &connection)
{
  //  Reset headers and body
  _headers.clear();
  _body.clear();
  //  Read the HTTP request headers
  boost::asio::streambuf stream_buf;
  size_t bytes_read = boost::asio::read_until(connection->socket(), stream_buf, "\r\n\r\n");
  std::string headers = boost::asio::buffer_cast<const char*>(stream_buf.data());
  std::string::size_type pos = headers.find("\r\n\r\n");
  if (pos == std::string::npos)
  {
    //  Throw some error here
  }
  //  Parse the content length
  long content_length = parse_content_length(headers);
  if (content_length == 0)
  {
    _headers = headers;
    return;
  }
  //  Create a buffer for the entire contents
  std::vector<char> buf(content_length + 1, 0);
  //  When only the headers are read, read all of the contents
  if (headers.length() == bytes_read)
  {
    //  Set the headers
    _headers = headers;
    //  Read the HTTP request body
    boost::asio::read(connection->socket(), boost::asio::buffer(buf, content_length));
    _body = buf.data();
  }
  else
  {
    //  Headers (for some reason) contains part of the body already read
    //  copy that section to the body
    _body = headers.substr(pos + 4);
    long read_length = content_length - (headers.length() - pos - 4);
    boost::asio::read(connection->socket(), boost::asio::buffer(buf, read_length));
    _body.append(buf.data());
    //  Finally update the headers
    _headers = headers.substr(0, pos);
  }
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
