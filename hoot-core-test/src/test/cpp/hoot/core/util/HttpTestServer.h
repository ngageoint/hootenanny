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

#ifndef HTTP_TEST_SERVER_H
#define HTTP_TEST_SERVER_H

//  Standard
#include <memory>
#include <thread>

//  Boost
#include <boost/bind.hpp>
#include <boost/asio.hpp>

namespace hoot
{

/**
 * @brief The HttpConnection class encapsulates an HTTP connection on a socket
 */
class HttpConnection : public std::enable_shared_from_this<HttpConnection>
{
public:
  /** Type def for shared pointer */
  typedef std::shared_ptr<HttpConnection> HttpConnectionPtr;
  /** Constructor */
  HttpConnection(boost::asio::io_service& io_service) : _socket(io_service) { }
  /** Socket accessor */
  boost::asio::ip::tcp::socket& socket() { return _socket; }
  /** Error checking for write function, empty for test server */
  void handle_write(const boost::system::error_code& /* error */, size_t /* bytes_transferred */) { }

private:
  /** Socket for this HTTP connection */
  boost::asio::ip::tcp::socket _socket;
};

/**
 * @brief The HttpTestServer class is a base class that provides simple HTTP server capabilites.
 *  In its basic form it runs forever and returns an HTTP 200 OK message and must be shutdown by
 *  calling the shutdown() function.
 *
 *  The only thing required to derive a class from this is to override the virtual respond() function
 *  that first calls read_request_headers(), decides how to respond, and finally writes out the
 *  response by calling the write_response() function.
 */
class HttpTestServer
{
public:
  /** Static constants for use in the HttpTestServer class */
  static const int DEFAULT_SERVER_PORT = 8910;
  static const std::string HTTP_200_OK;
  static const std::string HTTP_404_NOT_FOUND;
  static const std::string HTTP_409_CONFLICT;
  /** Constructor */
  HttpTestServer(int port = DEFAULT_SERVER_PORT);
  /** Function to start the HTTP server */
  void start();
  /** Function to wait for the HTTP server to finish processing */
  void wait();
  /** Function to shutdown the HTTP server even if it is still running */
  void shutdown();

protected:
  /**
   * @brief respond() function is overridden in derived classes to respond to different HTTP
   *  test requests
   * @param connection Current connection
   * @return True if the server should continue to listen for connections, false to stop
   */
  virtual bool respond(HttpConnection::HttpConnectionPtr& connection);
  /** Read the HTTP headers from the request and return them, called by the overridden respond() function */
  std::string read_request_headers(HttpConnection::HttpConnectionPtr& connection);
  /** Write the response back to the requestor, called by the overridden respond() function */
  void write_response(HttpConnection::HttpConnectionPtr& connection, const std::string& response);

private:
  /** Function that starts the server listening and accepting connections */
  void run_server(int port);
  /** Begin the process of accepting an HTTP connection */
  void start_accept();
  /** Handle the HTTP connection and calls the overridden respond() function */
  void handle_accept(HttpConnection::HttpConnectionPtr new_connection, const boost::system::error_code& error);
  /** IO Service object */
  boost::asio::io_service _io_service;
  /** Pointer to the ASIO TCP connection acceptor */
  std::shared_ptr<boost::asio::ip::tcp::acceptor> _acceptor;
  /** HTTP Test server thread */
  std::shared_ptr<std::thread> _thread;
  /** HTTP listening port */
  int _port;
};

}

#endif  //  OSM_API_WRITER_TEST_SERVER_H
