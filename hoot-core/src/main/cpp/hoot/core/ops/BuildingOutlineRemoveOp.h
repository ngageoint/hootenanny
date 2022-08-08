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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef BUILDINGOUTLINEREMOVEOP_H
#define BUILDINGOUTLINEREMOVEOP_H

// Hoot
#include <hoot/core/ops/OsmMapOperation.h>

namespace hoot
{
class OsmMap;
class Relation;

/**
 * Goes through all building relations and removes features with the outline role.
 */
class BuildingOutlineRemoveOp : public OsmMapOperation
{
public:

  static QString className() { return "BuildingOutlineRemoveOp"; }

  BuildingOutlineRemoveOp() = default;
  ~BuildingOutlineRemoveOp() override = default;

  void apply(std::shared_ptr<OsmMap>& map) override;

  QString getInitStatusMessage() const override { return "Removing outlines around buildings..."; }
  QString getCompletedStatusMessage() const override
  { return "Removed " + QString::number(_numAffected) + " building outlines"; }

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override { return "Removes the outline around buildings"; }

  /**
   * @see FilteredByGeometryTypeCriteria
   */
  QStringList getCriteria() const override;

private:

  std::shared_ptr<OsmMap> _map;

  void _removeOutline(const std::shared_ptr<Relation>& building);
};

}

#endif // BUILDINGOUTLINEREMOVEOP_H
