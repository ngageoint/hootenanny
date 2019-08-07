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

namespace hoot
{

std::string HttpTestServer::HTTP_200_OK = "HTTP/1.1 200 OK\r\n\r\n";
std::string HttpTestServer::HTTP_404_NOT_FOUND = "HTTP/1.1 404 Not Found\r\n\r\n";

HttpTestServer::HttpTestServer(int port)
  : _port(port)
{
}

void HttpTestServer::start()
{
  _thread.reset(new std::thread(std::bind(&HttpTestServer::run_server, this, _port)));
}

void HttpTestServer::wait()
{
  _thread->join();
}

void HttpTestServer::shutdown()
{
  //  Stop the IO service and wait for the server to stop
  _io_service.stop();
  wait();
}

void HttpTestServer::run_server(int port)
{
  try
  {
    _acceptor.reset(new boost::asio::ip::tcp::acceptor(_io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)));
    start_accept();
    _io_service.run();
  }
  catch (std::exception& e)
  {
    LOG_ERROR(e.what());
  }
}

void HttpTestServer::start_accept()
{
  HttpConnection::HttpConnectionPtr new_connection(new HttpConnection(_acceptor->get_io_service()));
  _acceptor->async_accept(new_connection->socket(),
                         boost::bind(&HttpTestServer::handle_accept, this, new_connection, boost::asio::placeholders::error));
}

void HttpTestServer::handle_accept(HttpConnection::HttpConnectionPtr new_connection, const boost::system::error_code& error)
{
  bool continue_processing = true;
  if (!error)
    continue_processing = respond(new_connection);

  if (continue_processing)
    start_accept();
}

bool HttpTestServer::respond(HttpConnection::HttpConnectionPtr& connection)
{
  //  Stop processing by setting this to false
  bool continue_processing = true;
  //  Read the HTTP request
  boost::asio::streambuf buf;
  boost::asio::read_until(connection->socket(), buf, "\r\n\r\n");
  //  Respond with HTTP 200 OK
  std::string message;
  message = std::string(HTTP_200_OK);
  //  Write out the response
  write_response(connection, message);
  //  Return true if we should continue listening and processing requests
  return continue_processing;
}

void HttpTestServer::write_response(HttpConnection::HttpConnectionPtr& connection, const std::string& response)
{
  //  Write the response to the socket asynchronously
  boost::asio::async_write(connection->socket(), boost::asio::buffer(response),
                           boost::bind(&HttpConnection::handle_write, connection,
                                       boost::asio::placeholders::error,
                                       boost::asio::placeholders::bytes_transferred));
}


}
