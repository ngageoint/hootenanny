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
 * @copyright Copyright (C) 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#ifndef OSM_API_READER_TEST_SERVER_H
#define OSM_API_READER_TEST_SERVER_H

#include <hoot/core/util/HttpTestServer.h>

namespace hoot
{

class SimpleReaderTestServer : public HttpTestServer
{
public:
  /** Constructor */
  SimpleReaderTestServer(int port) : HttpTestServer(port) { }

protected:
  /** respond() function that responds once to the OSM API Capabilities request */
  virtual bool respond(HttpConnectionPtr& connection) override;
};

class GeographicSplitReaderTestServer : public HttpSequencedServer
{
public:
  /** Constructor */
  GeographicSplitReaderTestServer(int port)
    : HttpSequencedServer(4, port)
  { }

protected:
  /** respond() function that responds once to the OSM API Capabilities request */
  virtual bool respond(HttpConnectionPtr& connection) override;
  /** get_sequence_response() function that gets the response message based on the URL requested */
  virtual HttpResponsePtr get_sequence_response(const std::string& request) override;
};

class ElementSplitReaderTestServer : public HttpSequencedServer
{
public:
  /** Constructor */
  ElementSplitReaderTestServer(int port)
    : HttpSequencedServer(5, port),
      _split_forced(false)
  { }

protected:
  /** respond() function that responds once to the OSM API Capabilities request */
  virtual bool respond(HttpConnectionPtr& connection) override;
  /** get_sequence_response() function that gets the response message based on the URL requested */
  virtual HttpResponsePtr get_sequence_response(const std::string& url) override;
  /** Force split based on "element count" */
  bool _split_forced;
};

class OverpassReaderTestServer : public HttpSequencedServer
{
public:
  /** Constructor */
  OverpassReaderTestServer(int port)
    : HttpSequencedServer(2, port)
  { }

protected:
  /** respond() function that responds once to the OSM API Capabilities request */
  virtual bool respond(HttpConnectionPtr& connection) override;
  /** get_sequence_response() function that gets the response message based on the URL requested */
  virtual HttpResponsePtr get_sequence_response(const std::string& url) override;
};

}

#endif  //  OSM_API_READER_TEST_SERVER_H
