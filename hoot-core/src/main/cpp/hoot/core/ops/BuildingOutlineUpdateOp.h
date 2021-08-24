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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef BUILDINGOUTLINEUPDATEOP_H
#define BUILDINGOUTLINEUPDATEOP_H

// Hoot
#include <hoot/core/conflate/review/ReviewMarker.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/ops/OsmMapOperation.h>

namespace hoot
{
class OsmMap;

/**
 * Goes through all building relations and updates the outline of the building by taking the union
 * of all the building parts.
 */
class BuildingOutlineUpdateOp : public OsmMapOperation
{
public:

  static QString className() { return "hoot::BuildingOutlineUpdateOp"; }

  BuildingOutlineUpdateOp() = default;
  ~BuildingOutlineUpdateOp() = default;

  void apply(std::shared_ptr<OsmMap>& map) override;

  QString getInitStatusMessage() const override
  { return "Updating building outlines..."; }
  QString getCompletedStatusMessage() const override
  { return "Updated " + QString::number(_numAffected) + " building outlines"; }

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override
  { return "Updates multi-part building outlines"; }

  /**
   * @see FilteredByGeometryTypeCriteria
   */
  QStringList getCriteria() const override;

private:

  static int logWarnCount;

  std::shared_ptr<OsmMap> _map;
  ReviewMarker _reviewMarker;

  void _createOutline(const RelationPtr& building) const;
  void _unionOutline(const RelationPtr& building, const ElementPtr& element,
                     std::shared_ptr<geos::geom::Geometry>& outline) const;

  /**
   * Match nodes in change to nodes in reference. If there is an exact node match then change
   * "changed" by replacing the nodes with the equivalent nodes in reference.
   */
  void _mergeNodes(const std::shared_ptr<Element>& changed,
                   const RelationPtr& reference) const;
};

}

#endif // BUILDINGOUTLINEUPDATEOP_H
