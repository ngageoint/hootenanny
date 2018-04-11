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
#ifndef APIDBREADER_H
#define APIDBREADER_H

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/ops/Boundable.h>
#include <hoot/core/util/Configurable.h>
#include "PartialOsmMapReader.h"

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
class ApiDbReader : public PartialOsmMapReader, public Boundable, public Configurable
{
public:

  static std::string className() { return "hoot::ApiDbReader"; }

  static unsigned int logWarnCount;

  ApiDbReader();
  virtual ~ApiDbReader() {}

  virtual bool isSupported(QString urlStr);

  virtual void setBounds(const geos::geom::Envelope& bounds) { _bounds = bounds; }

  void setBoundingBox(const QString bbox);
  void setOverrideBoundingBox(const QString bbox);
  void setReturnNodesOnly(const bool returnNodesOnly)
  { _returnNodesOnly = returnNodesOnly; }

  /**
   * Determines the reader's default element status. By default this is Invalid which specifies that
   * the file's status will be used.
   */
  virtual void setDefaultStatus(Status status) { _status = status; }

  /**
   * Determines whether the reader should use the element id's from the file being read
   */
  virtual void setUseDataSourceIds(bool useDataSourceIds) { _useDataSourceIds = useDataSourceIds; }

  void setUserEmail(const QString& email) { _email = email; }

  /**
   * @see PartialOsmMapReader
   */
  virtual void initializePartial();

  /**
   * The read command called after open.
   */
  virtual void read(OsmMapPtr map);

  /**
   * @see PartialOsmMapReader
   */
  virtual void finalizePartial();

  void close();

  /**
   * @see PartialOsmMapReader
   */
  virtual bool hasMoreElements();

  /**
   * @see PartialOsmMapReader
   */
  virtual boost::shared_ptr<Element> readNextElement();

  /**
   * @see PartialOsmMapReader
   */
  virtual boost::shared_ptr<OGRSpatialReference> getProjection() const;

protected:

  bool _useDataSourceIds;
  Status _status;
  bool _open;
  QString _email;

  Tgs::BigMap<long, long> _nodeIdMap;
  Tgs::BigMap<long, long> _relationIdMap;
  Tgs::BigMap<long, long> _wayIdMap;

  geos::geom::Envelope _bounds;
  geos::geom::Envelope _overrideBounds; //this will override _bounds

  bool _returnNodesOnly;

  bool _keepStatusTag;

  ElementType _selectElementType;
  boost::shared_ptr<QSqlQuery> _elementResultIterator;
  boost::shared_ptr<Element> _nextElement;

  virtual NodePtr _resultToNode(const QSqlQuery& resultIterator, OsmMap& map) = 0;
  virtual WayPtr _resultToWay(const QSqlQuery& resultIterator, OsmMap& map) = 0;
  virtual RelationPtr _resultToRelation(const QSqlQuery& resultIterator, const OsmMap& map) = 0;

  virtual ElementId _mapElementId(const OsmMap& map, ElementId oldId);

  virtual boost::shared_ptr<ApiDb> _getDatabase() const = 0;

  /*
   * This is the same logic as in the Map.java query method.
   */
  virtual void _readByBounds(OsmMapPtr map, const geos::geom::Envelope& bounds);

  void _updateMetadataOnElement(ElementPtr element);

  static bool _isValidBounds(const geos::geom::Envelope& bounds);
  bool _hasBounds();

private:

  bool _firstPartialReadCompleted;

  //the maximum ID for each element present in the table; since the results coming back are sorted
  //by ID, these can be used to know when to stop reading elements of each type
  long _maxNodeId;
  long _maxWayId;
  long _maxRelationId;

  //last element ID read
  long _lastId;

  long _statusUpdateInterval;

  long _totalNumMapNodes;
  long _totalNumMapWays;
  long _totalNumMapRelations;

  long _numNodesRead;
  long _numWaysRead;
  long _numRelationsRead;

  void _read(OsmMapPtr map, const ElementType& elementType);

  /**
   * Converts a query result to an OSM element
   * 
   * This will advance the results iterator *before* reading each element.
   */
  boost::shared_ptr<Element> _resultToElement(QSqlQuery& resultIterator,
                                              const ElementType& elementType, OsmMap& map);

  /*
   * the current element type being returned by the partial query; order is: node, way, and then
   * relation
   */
  ElementType _getCurrentSelectElementType();

  boost::shared_ptr<Element> _getElementUsingIterator();
};

}

#endif // APIDBREADER_H
