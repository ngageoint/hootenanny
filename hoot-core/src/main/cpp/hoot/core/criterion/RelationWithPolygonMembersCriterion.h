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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef RELATION_WITH_POLYGON_MEMBERS_CRITERION_H
#define RELATION_WITH_POLYGON_MEMBERS_CRITERION_H

// Hoot
#include <hoot/core/criterion/RelationWithGeometryMembersCriterion.h>
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

/**
 * Identifies relations with children having polygon geometries
 */
class RelationWithPolygonMembersCriterion : public RelationWithGeometryMembersCriterion
{
public:

  static std::string className() { return "hoot::RelationWithPolygonMembersCriterion"; }

  RelationWithPolygonMembersCriterion() = default;
  RelationWithPolygonMembersCriterion(ConstOsmMapPtr map);
  virtual ~RelationWithPolygonMembersCriterion() = default;

  virtual ElementCriterionPtr clone()
  { return ElementCriterionPtr(new RelationWithPolygonMembersCriterion(_map)); }

  virtual QString getCriterion() const override { return "hoot::PolygonCriterion"; }

  virtual QString getDescription() const
  { return "Identifies relations with children having polygon geometries"; }

  virtual GeometryType getGeometryType() const { return GeometryType::Polygon; }

  virtual QString toString() const override
  { return QString::fromStdString(className()).remove("hoot::"); }
};

}
#endif // RELATION_WITH_POLYGON_MEMBERS_CRITERION_H
