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

#ifndef OSM_API_WRITER_TEST_SERVER_H
#define OSM_API_WRITER_TEST_SERVER_H

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

class OsmApiSampleResponses
{
public:
  static const char* SAMPLE_CAPABILITIES;
  static const char* SAMPLE_PERMISSIONS;
};


class HttpConnection : public std::enable_shared_from_this<HttpConnection>
{
public:
  typedef std::shared_ptr<HttpConnection> HttpConnectionPtr;

  boost::asio::ip::tcp::socket& socket();

  static HttpConnectionPtr create(boost::asio::io_service& io_service);

  bool start();

private:
  HttpConnection(boost::asio::io_service& io_service);
  void handle_write(const boost::system::error_code& error, size_t bytes_transferred);

  boost::asio::ip::tcp::socket _socket;
};

class HttpTestServer
{
public:

  static const int TEST_SERVER_PORT;

  static std::shared_ptr<std::thread> start();

  HttpTestServer(boost::asio::io_service& io_service);

private:

  static void run_server();

  void start_accept();

  void handle_accept(HttpConnection::HttpConnectionPtr new_connection, const boost::system::error_code& error);

  boost::asio::ip::tcp::acceptor _acceptor;
};

}

#endif  //  OSM_API_WRITER_TEST_SERVER_H
