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
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef METADATAOP_H
#define METADATAOP_H

// Hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/NodeMap.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/WayMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Configurable.h>

// Qt
#include <QString>
#include <QPair>

// geos
#include <geos/geom/Geometry.h>
#include <geos/geom/Polygon.h>

namespace hoot
{

class MetadataOp : public OsmMapOperation, public Configurable
{
public:

  static std::string className() { return "hoot::MetadataOp"; }
  MetadataOp() : _pConf(&conf()) {}

  // OsmMapOperation
  virtual void apply(std::shared_ptr<OsmMap>& pMap) override;

  // OperationStatusInfo
  virtual QString getInitStatusMessage() const { return "Processing metadata..."; }
  virtual QString getCompletedStatusMessage() const { return "Modified " + QString::number(_numAffected) + " elements"; }

  // Configurable
  virtual void setConfiguration(const Settings& conf);

  virtual std::string getClassName() const { return className(); }

protected:

  // configuration data
  QPair<QString,QString> _datasetIndicator;
  QHash<QString,QString> _tags;
  double _gridCellSize;

  // protected data
  std::shared_ptr<OsmMap> _pMap;

  WayMap _allWays;
  NodeMap _allNodes;
  RelationMap _allRels;

  QList<ElementPtr> _elementsToProcess;
  QMap<WayPtr,std::shared_ptr<geos::geom::Polygon>> _datasetWayPolys;
  QMap<WayPtr,std::shared_ptr<geos::geom::Geometry>> _mergedGeoms;
  QHash<long,std::shared_ptr<geos::geom::Geometry>> _nodeLocations;

  // shared processing functions

  // collects all elements in the map that have information tags and creates
  // a geos Point for determining their location in relation to a dataset in
  // _assignToDataset
  void _gatherProcessElements();

  // determines the dataset way to which an element belongs
  WayPtr _assignToDataset( ElementPtr pElement );

  // removes unused or unneeded dataset ways and their nodes
  void _removeDatasetWay(WayPtr pDataset);

private:

  const Settings* _pConf;

  virtual void _apply() = 0;

  void _configure();
};

}

#endif // METADATAOP_H
