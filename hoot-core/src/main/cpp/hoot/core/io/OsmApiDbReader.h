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
 * @copyright Copyright (C) 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef OSMAPIDBREADER_H
#define OSMAPIDBREADER_H

#include "ApiDbReader.h"
#include "OsmApiDb.h"

namespace hoot
{

/**
 * Reads from an OSM API database.
 */
class OsmApiDbReader : public ApiDbReader
{
public:

  static std::string className() { return "hoot::OsmApiDbReader"; }

  OsmApiDbReader();
  virtual ~OsmApiDbReader();

  virtual void open(QString urlStr);

  virtual void setConfiguration(const Settings &conf);

  virtual QString supportedFormats() { return "osmapidb://"; }

protected:

  virtual NodePtr _resultToNode(const QSqlQuery& resultIterator, OsmMap& map);
  virtual WayPtr _resultToWay(const QSqlQuery& resultIterator, OsmMap& map);
  virtual RelationPtr _resultToRelation(const QSqlQuery& resultIterator, const OsmMap& map);

  virtual boost::shared_ptr<ApiDb> _getDatabase() const { return _database; }

private:

  //for white box testing
  friend class ServiceOsmApiDbBulkInserterTest;
  friend class ServicesDbTestUtils;
  friend class ServiceWriteOsmSqlStatementsDriverTest;

  boost::shared_ptr<OsmApiDb> _database;

  double _defaultCircularError;

  void _parseAndSetTagsOnElement(ElementPtr element);
};

}

#endif // OSMAPIDBREADER_H
