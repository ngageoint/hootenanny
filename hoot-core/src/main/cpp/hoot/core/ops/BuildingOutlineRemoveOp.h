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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef BUILDINGOUTLINEREMOVEOP_H
#define BUILDINGOUTLINEREMOVEOP_H

// Hoot
#include <hoot/core/ops/OsmMapOperation.h>

// Standard
#include <set>

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

  static std::string className() { return "hoot::BuildingOutlineRemoveOp"; }

  BuildingOutlineRemoveOp() = default;
  virtual ~BuildingOutlineRemoveOp() = default;

  virtual void apply(std::shared_ptr<OsmMap>& map) override;

  virtual QString getName() const { return QString::fromStdString(className()); }

  virtual QString getInitStatusMessage() const { return "Removing outlines around buildings..."; }

  virtual QString getCompletedStatusMessage() const
  { return "Removed " + QString::number(_numAffected) + " building outlines"; }

  virtual QString getDescription() const override { return "Removes the outline around buildings"; }

  /**
   * @see FilteredByGeometryTypeCriteria
   */
  virtual QStringList getCriteria() const;

private:

  std::shared_ptr<OsmMap> _map;

  void _removeOutline(const std::shared_ptr<Relation>& building);
};

}

#endif // BUILDINGOUTLINEREMOVEOP_H
