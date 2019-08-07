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

//  Hootenanny
//#include <hoot/core/io/OsmApiWriter.h>
//#include <hoot/core/util/Log.h>

//  Standard
#include <memory>
#include <thread>

//  Boost
#include <boost/bind.hpp>
#include <boost/asio.hpp>

namespace hoot
{

class HttpConnection : public std::enable_shared_from_this<HttpConnection>
{
public:
  typedef std::shared_ptr<HttpConnection> HttpConnectionPtr;

  HttpConnection(boost::asio::io_service& io_service) : _socket(io_service) { }

  boost::asio::ip::tcp::socket& socket() { return _socket; }

  void handle_write(const boost::system::error_code& /* error */, size_t /* bytes_transferred */) { }

private:

  boost::asio::ip::tcp::socket _socket;

};

class HttpTestServer
{
public:

  static const int DEFAULT_SERVER_PORT = 8910;

  HttpTestServer(int port = DEFAULT_SERVER_PORT);

  void start();

  void wait();

  void shutdown();

  int get_port() { return _port; }

protected:

  virtual bool respond(HttpConnection::HttpConnectionPtr& connection);

  std::string read_request_headers(HttpConnection::HttpConnectionPtr& connection);

  void write_response(HttpConnection::HttpConnectionPtr& connection, const std::string& response);

  static const std::string HTTP_200_OK;
  static const std::string HTTP_404_NOT_FOUND;

private:

  void run_server(int port);

  void start_accept();

  void handle_accept(HttpConnection::HttpConnectionPtr new_connection, const boost::system::error_code& error);

  std::shared_ptr<boost::asio::ip::tcp::acceptor> _acceptor;

  boost::asio::io_service _io_service;

  std::shared_ptr<std::thread> _thread;

  int _port;
};

}

#endif  //  OSM_API_WRITER_TEST_SERVER_H
