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
#ifndef APIDBREADER_H
#define APIDBREADER_H

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/ops/Boundable.h>

// Boost
#include <boost/shared_ptr.hpp>

// Qt
#include <QtSql/QSqlQuery>

// tgs
#include <tgs/BigContainers/BigMap.h>

#include <ogr_spatialref.h>

namespace hoot
{

class ApiDb;

/**
 * Abstract parent class for reading from an API style OSM database
 */
class ApiDbReader : public Boundable
{
public:

  static std::string className() { return "hoot::ApiDbReader"; }

  static unsigned int logWarnCount;

  ApiDbReader();
  virtual ~ApiDbReader() {}

  virtual void setBounds(const geos::geom::Envelope& bounds) { _bounds = bounds; }

  void setBoundingBox(const QString bbox);
  void setOverrideBoundingBox(const QString bbox);
  void setReturnNodesOnly(const bool returnNodesOnly)
  { _returnNodesOnly = returnNodesOnly; }

protected:

  bool _useDataSourceIds;
  Status _status;
  bool _open;

  Tgs::BigMap<long, long> _nodeIdMap;
  Tgs::BigMap<long, long> _relationIdMap;
  Tgs::BigMap<long, long> _wayIdMap;

  geos::geom::Envelope _bounds;
  geos::geom::Envelope _overrideBounds; //this will override _bounds

  bool _returnNodesOnly;

  virtual NodePtr _resultToNode(const QSqlQuery& resultIterator, OsmMap& map) = 0;
  virtual WayPtr _resultToWay(const QSqlQuery& resultIterator, OsmMap& map) = 0;
  virtual RelationPtr _resultToRelation(const QSqlQuery& resultIterator,
                                                 const OsmMap& map) = 0;

  virtual ElementId _mapElementId(const OsmMap& map, ElementId oldId);

  virtual boost::shared_ptr<ApiDb> _getDatabase() const = 0;

  /*
   * This is based off of the Map.java query method.
   */
  virtual void _readByBounds(OsmMapPtr map, const geos::geom::Envelope& bounds);

  void _updateMetadataOnElement(ElementPtr element);

  static bool _isValidBounds(const geos::geom::Envelope& bounds);
  bool _hasBounds();

};

}

#endif // APIDBREADER_H
