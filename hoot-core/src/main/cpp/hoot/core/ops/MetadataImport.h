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

#ifndef METADATAIMPORT_H
#define METADATAIMPORT_H

// Hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/NodeMap.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/WayMap.h>
#include <hoot/core/ops/MetadataOp.h>

// geos
#include <geos/geom/Geometry.h>
#include <geos/geom/Polygon.h>

namespace hoot
{

class MetadataImport : public MetadataOp
{
public:

  static std::string className() { return "hoot::MetadataImport"; }

  // OsmMapOperation
  virtual QString getDescription() const override { return "Imports metadata"; }

private:

  // MetadataOp
  virtual void _apply();

  // private data
  WayMap _allWays;
  NodeMap _allNodes;
  RelationMap _allRels;

  QMap<WayPtr,std::shared_ptr<geos::geom::Polygon>> _datasetWayPolys;
  QMap<WayPtr,std::shared_ptr<geos::geom::Geometry>> _mergedImportGeoms;
  QList<ElementPtr> _elementsToProcess;
  QHash<long,std::shared_ptr<geos::geom::Geometry>> _nodeLocations;

  // process sequence functions
  void _findDatasetWays();
  void _mergePolygonsWithMatchingMetadata();
  void _gatherTargetElements();
  void _importMetadataToElements();

  // helper functions
  bool _areMetadataTagsEqual(ElementPtr p1, ElementPtr p2);
  bool _applyToElement(ElementPtr pElement);
};

}

#endif // METADATAIMPORT_H
