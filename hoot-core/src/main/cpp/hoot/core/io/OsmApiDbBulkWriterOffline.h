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
 * @copyright Copyright (C) 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef OSMAPIDBBULKWRITEROFFLINE_H
#define OSMAPIDBBULKWRITEROFFLINE_H

#include <hoot/core/io/OsmApiDbBulkWriter.h>

namespace hoot
{

using namespace boost;
using namespace std;

class OsmApiDbCsvTableFileSetWriter;

/**
 * Version of osm api db bulk writing intended to utilize the pg_bulkload utility for increased
 * write performance.
 *
 * IMPORTANT: This class should only be used with offline databases (this class is the only writer).
 */
class OsmApiDbBulkWriterOffline : public OsmApiDbBulkWriter
{

public:

  static string className() { return "hoot::OsmApiDbBulkWriterOffline"; }

  static unsigned int logWarnCount;

  OsmApiDbBulkWriterOffline();

  virtual ~OsmApiDbBulkWriterOffline();

  virtual bool isSupported(QString url);

  virtual void finalizePartial();

protected:

  virtual void _retainOutputFiles();
  virtual void _writeDataToDb();

private:


};

}

#endif // OSMAPIDBBULKWRITEROFFLINE_H
