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

#ifndef RELATION_WITH_GEOMETRY_MEMBERS_CRITERION_H
#define RELATION_WITH_GEOMETRY_MEMBERS_CRITERION_H

// Hoot
#include <hoot/core/criterion/GeometryTypeCriterion.h>
#include <hoot/core/elements/ConstOsmMapConsumer.h>
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

/**
 * Abstract class for identifying relations based on the geometry types of their children
 */
class RelationWithGeometryMembersCriterion : public GeometryTypeCriterion,
  public ConstOsmMapConsumer
{
public:

  static std::string className() { return "hoot::RelationWithGeometryMembersCriterion"; }

  RelationWithGeometryMembersCriterion();
  RelationWithGeometryMembersCriterion(ConstOsmMapPtr map);
  virtual ~RelationWithGeometryMembersCriterion() = default;

  virtual bool isSatisfied(const ConstElementPtr& e) const override;

  virtual QString getCriterion() const = 0;

  virtual void setOsmMap(const OsmMap* map);

  void setAllowMixedChildren(bool allow) { _allowMixedChildren = allow; }

protected:

  ConstOsmMapPtr _map;

private:

  // If true, this crit will pass if there is only one geometry of the specified type. If false,
  // all children must have the specified geomertry type.
  bool _allowMixedChildren;

  mutable ElementCriterionPtr _crit;

  void _initCrit() const;
};

}
#endif // RELATION_WITH_GEOMETRY_MEMBERS_CRITERION_H
