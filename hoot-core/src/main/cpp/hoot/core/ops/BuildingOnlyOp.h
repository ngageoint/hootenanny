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
 * @copyright Copyright (C) 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef BUILDINGONLYOP_H
#define BUILDINGONLYOP_H

// Hoot
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/elements/Element.h>

namespace hoot
{
class OsmMap;

/**
 * @brief The BuildingOnlyOp class operates is a custom op written to help
 * facilitate the bulk import of building outlines into an OSM database. It
 * does a few things:
 *
 * 1) Visits all elements in the map, and removes extreneous tags:
 *    OBJECTID
 *    PAGENUMBER
 *    SHAPE_ARE
 *    SHAPE_LENG
 *    hoot:layername
 *
 * 2) It converts tags "BUILDING=yes" to "building=yes"
 *
 * 3) It removes building relations where two outlines are joined by a shared
 *    way segment. (Note that it leave multipolygon relations alone)
 */
class BuildingOnlyOp : public OsmMapOperation
{
public:

  /**
   * @brief className - get the namespace-qualified class name
   * @return class name string
   */
  static std::string className() { return "hoot::BuildingOnlyOp"; }

  /**
   * @brief BuildingOnlyOp - default constructor
   */
  BuildingOnlyOp() = default;
  virtual ~BuildingOnlyOp() = default;

  /**
   * @brief apply - perform the operation on the map (see class description)
   * @param map - map to operate on
   */
  virtual void apply(std::shared_ptr<OsmMap>& map) override;

  /**
   * @brief getClassName - get the namespace-qualified class name
   * @return class name string
   */
  virtual std::string getClassName() const { return className(); }

  virtual QString getDescription() const override
  { return "Allows for filtering building parts out of buildings"; }

private:

  std::shared_ptr<OsmMap> _map;

  bool _isBuildingRelation(ConstElementPtr e);
};

}

#endif // BUILDINGONLYOP_H
