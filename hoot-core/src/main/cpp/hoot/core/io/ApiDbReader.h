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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef APIDBREADER_H
#define APIDBREADER_H

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/io/ApiDb.h>

#include <boost/shared_ptr.hpp>

// Qt
#include <QtSql/QSqlQuery>

// tgs
#include <tgs/BigContainers/BigMap.h>

#include <ogr_spatialref.h>

namespace hoot
{

/**
 * Abstract parent class for reading from an API style OSM database
 */
class ApiDbReader
{
public:

  ApiDbReader();
  virtual ~ApiDbReader() {}

protected:

  bool _useDataSourceIds;
  Status _status;
  bool _open;

  Tgs::BigMap<long, long> _nodeIdMap;
  Tgs::BigMap<long, long> _relationIdMap;
  Tgs::BigMap<long, long> _wayIdMap;

  virtual shared_ptr<Node> _resultToNode(const QSqlQuery& resultIterator, OsmMap& map) = 0;
  virtual shared_ptr<Way> _resultToWay(const QSqlQuery& resultIterator, OsmMap& map) = 0;
  virtual shared_ptr<Relation> _resultToRelation(const QSqlQuery& resultIterator,
                                                 const OsmMap& map) = 0;

  virtual ElementId _mapElementId(const OsmMap& map, ElementId oldId);

  virtual shared_ptr<ApiDb> _getDatabase() const = 0;

  /*
   * This is based off of the Map.java query method.  Record paging to avoid OOM errors hasn't been
   * implemented yet.
   */
  virtual void _readByBounds(OsmMapPtr map, const Envelope& bounds);

  void _updateMetadataOnElement(ElementPtr element);
};

}

#endif // APIDBREADER_H
