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
  virtual bool respond(HttpConnection::HttpConnectionPtr& connection) override;
};

class GeographicSplitReaderTestServer : public HttpTestServer
{
public:
  /** Constructor */
  GeographicSplitReaderTestServer(int port)
    : HttpTestServer(port),
      _section(0)
  { }

protected:
  /** respond() function that responds once to the OSM API Capabilities request */
  virtual bool respond(HttpConnection::HttpConnectionPtr& connection) override;
  /** Keep track of which section is being requested */
  int _section;
};

class ElementSplitReaderTestServer : public HttpTestServer
{
public:
  /** Constructor */
  ElementSplitReaderTestServer(int port)
    : HttpTestServer(port),
      _splitForced(false),
      _section(0)
  { }

protected:
  /** respond() function that responds once to the OSM API Capabilities request */
  virtual bool respond(HttpConnection::HttpConnectionPtr& connection) override;
  /** Force split based on "element count" */
  bool _splitForced;
  /** Keep track of which section is being requested */
  int _section;
};

}

#endif  //  OSM_API_READER_TEST_SERVER_H
