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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef APIDBREADER_H
#define APIDBREADER_H

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/PartialOsmMapReader.h>
#include <hoot/core/util/Boundable.h>
#include <hoot/core/util/Configurable.h>

// Qt
#include <QtSql/QSqlQuery>

// tgs
#include <tgs/BigContainers/BigMap.h>

// OGR
#include <ogr_spatialref.h>

namespace hoot
{

class ApiDb;

/**
 * Abstract parent class for reading from an API style OSM database
 */
class ApiDbReader : public PartialOsmMapReader, public Boundable
{
public:

  static QString className() { return "ApiDbReader"; }

  ApiDbReader();
  virtual ~ApiDbReader() = default;

  bool isSupported(const QString& urlStr) override;

  void setBoundingBox(const QString& bbox);
  void setOverrideBoundingBox(const QString& bbox);
  void setReturnNodesOnly(const bool returnNodesOnly)
  { _returnNodesOnly = returnNodesOnly; }

  /**
   * Determines the reader's default element status. By default this is Invalid which specifies that
   * the file's status will be used.
   */
  void setDefaultStatus(Status status) override { _status = status; }

  /**
   * Determines whether the reader should use the element id's from the file being read
   */
  void setUseDataSourceIds(bool useDataSourceIds) override
  { _useDataSourceIds = useDataSourceIds; }

  void setUserEmail(const QString& email) { _email = email; }

  /**
   * @see PartialOsmMapReader
   */
  void initializePartial() override;

  /**
   * The read command called after open.
   */
  void read(const OsmMapPtr& map) override;

  /**
   * @see PartialOsmMapReader
   */
  void finalizePartial() override;

  void close() override;

  /**
   * @see PartialOsmMapReader
   */
  bool hasMoreElements() override;

  /**
   * @see PartialOsmMapReader
   */
  std::shared_ptr<Element> readNextElement() override;

  /**
   * @see PartialOsmMapReader
   */
  std::shared_ptr<OGRSpatialReference> getProjection() const override;

  void setKeepImmediatelyConnectedWaysOutsideBounds(bool keep)
  { _keepImmediatelyConnectedWaysOutsideBounds = keep; }

protected:

  static int logWarnCount;

  bool _useDataSourceIds;
  Status _status;
  bool _open;
  QString _email;
  // the CE value used if no CE tag is found
  double _defaultCircularError;

  Tgs::BigMap<long, long> _nodeIdMap;
  Tgs::BigMap<long, long> _relationIdMap;
  Tgs::BigMap<long, long> _wayIdMap;

  // TODO: explain
  std::shared_ptr<geos::geom::Geometry> _overrideBounds; // this will override _bounds
  // controls use of _readByBounds2 vs the default _readByBounds
  bool _readFullThenCropOnBounded;

  bool _returnNodesOnly;

  bool _keepStatusTag;

  ElementType _selectElementType;
  std::shared_ptr<QSqlQuery> _elementResultIterator;
  std::shared_ptr<Element> _nextElement;

  virtual NodePtr _resultToNode(const QSqlQuery& resultIterator, OsmMap& map) = 0;
  virtual WayPtr _resultToWay(const QSqlQuery& resultIterator, OsmMap& map) = 0;
  virtual RelationPtr _resultToRelation(const QSqlQuery& resultIterator, const OsmMap& map) = 0;

  virtual ElementId _mapElementId(const OsmMap& map, ElementId oldId);

  virtual std::shared_ptr<ApiDb> _getDatabase() const = 0;

  /*
   * Reads the entire dataset into a map
   */
  void _fullRead(OsmMapPtr map);
  /*
   * Reads a portion of the dataset into a map over the specified bounds. Note that this fully
   * hydrates relations, so extra filtering may need to be done after the fact to prevent conflating
   * features outside of the bounds.
   */
  void _readByBounds(OsmMapPtr map, const geos::geom::Envelope& bounds);
  /*
   * Reads a portion of the dataset into a map over the specified bounds. This reads the entire
   * dataset in and then crops it after the fact. See the description of the
   * apidb.reader.read.full.then.crop.on.bounded configuration option for more information.
   */
  void _readByBounds2(OsmMapPtr map, const geos::geom::Envelope& bounds);
  void _readWaysByNodeIds(OsmMapPtr map, const QSet<QString>& nodeIds, QSet<QString>& wayIds,
                          QSet<QString>& additionalNodeIds, long& nodeCount, long& wayCount);

  void _updateMetadataOnElement(ElementPtr element) const;

  bool _hasBounds() const;

private:

  // keys for tags containing CE data
  QStringList _circularErrorTagKeys;

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

  // only valid is _bounds is not null
  bool _keepImmediatelyConnectedWaysOutsideBounds;

  void _read(OsmMapPtr map, const ElementType& elementType);

  /**
   * Converts a query result to an OSM element
   * 
   * This will advance the results iterator *before* reading each element.
   */
  std::shared_ptr<Element> _resultToElement(QSqlQuery& resultIterator,
                                              const ElementType& elementType, OsmMap& map);

  /*
   * the current element type being returned by the partial query; order is: node, way, and then
   * relation
   */
  ElementType _getCurrentSelectElementType();

  std::shared_ptr<Element> _getElementUsingIterator();
};

}

#endif // APIDBREADER_H
