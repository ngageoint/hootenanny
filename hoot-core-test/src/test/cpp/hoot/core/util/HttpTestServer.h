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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
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
 * @brief The HttpResponse class encapsulates an HTTP response for writing out to a socket
 */
class HttpResponse
{
public:
  /**
   * @brief Constructor
   * @param status - HTTP status code
   * @param response - Response body
   */
  HttpResponse(int status, const std::string& response = "");
  /**
   * @brief add_header Add an HTTP header to the reponse message
   * @param header HTTP header text
   * @param value HTTP header value
   */
  void add_header(const std::string& header, const std::string& value);
  /**
   * @brief to_string Converts the object to an HTTP response string
   * @return Complete HTTP response
   */
  std::string to_string();

private:
  /**
   * @brief get_status_text Get the "OK" part of the "HTTP/1.1 200 OK" HTTP response
   * @return HTTP status code text
   */
  std::string get_status_text();
  /** HTTP status code */
  int _status;
  /** HTTP response body */
  std::string _response;
  /** Vector of HTTP header strings */
  std::vector<std::string> _headers;
  /** Key value pairs for HTTP headers */
  std::map<std::string, std::string> _header_values;
};

typedef std::shared_ptr<HttpResponse> HttpResponsePtr;

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
  /** Parset the headers and body of the HTTP request */
  void parse_request(HttpConnection::HttpConnectionPtr& connection);
  /** Write the response back to the requestor, called by the overridden respond() function */
  void write_response(HttpConnection::HttpConnectionPtr& connection, const std::string& response);
  /** Get the value of the interupt flag */
  bool get_interupt() { return _interupt; }
  /** Shutdown the thread but don't wait on it */
  void stop();
  /** Headers string */
  std::string _headers;
  /** Message body string */
  std::string _body;

private:
  /** Function that starts the server listening and accepting connections */
  void run_server(int port);
  /** Begin the process of accepting an HTTP connection */
  void start_accept();
  /** Handle the HTTP connection and calls the overridden respond() function */
  void handle_accept(HttpConnection::HttpConnectionPtr new_connection, const boost::system::error_code& error);
  /** Parse out the value of the "Content-Length" HTTP header */
  long parse_content_length(const std::string& headers);
  /** IO Service object */
  boost::asio::io_service _io_service;
  /** Pointer to the ASIO TCP connection acceptor */
  std::shared_ptr<boost::asio::ip::tcp::acceptor> _acceptor;
  /** HTTP Test server thread */
  std::shared_ptr<std::thread> _thread;
  /** HTTP listening port */
  int _port;
  /** Interupt flag */
  bool _interupt;
};

/**
 * @brief The HttpSequencedServer class is a base class for a test server that runs a sequence of steps that the
 *    server must keep track of and respond differently to.
 */
class HttpSequencedServer : public HttpTestServer
{
public:
  /**
   * @brief HttpSequencedServer constructor
   * @param num Number of sequences or stages in the test server
   * @param port Listening port for the test server
   */
  HttpSequencedServer(int num, int port = DEFAULT_SERVER_PORT)
    : HttpTestServer(port),
      _max(num),
      _current(0)
  { }

protected:
  /**
   * @brief get_sequence_response Return the HttpResponse object corresponding to the
   *    _current sequence number and the HTTP headers passed in
   * @param headers String value of the HTTP headers from the request object
   * @return HttpResponse object (cached or created)
   */
  virtual HttpResponsePtr get_sequence_response(const std::string& headers) = 0;

protected:
  /** Number of sequences in the test server */
  int _max;
  /** Current sequence number of the test server */
  int _current;
  /** Cache of responses to HTTP headers requested */
  std::map<std::string, HttpResponsePtr> _sequence_responses;
};

}

#endif  //  HTTP_TEST_SERVER_H
