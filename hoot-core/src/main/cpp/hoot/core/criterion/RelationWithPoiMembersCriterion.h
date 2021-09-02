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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */

#ifndef RELATION_WITH_POI_MEMBERS_CRITERION_H
#define RELATION_WITH_POI_MEMBERS_CRITERION_H

// Hoot
#include <hoot/core/criterion/RelationWithMembersOfTypeCriterion.h>

namespace hoot
{

/**
 * Identifies relations having POI members
 */
class RelationWithPoiMembersCriterion : public RelationWithMembersOfTypeCriterion
{
public:

  static QString className() { return "RelationWithPoiMembersCriterion"; }

  RelationWithPoiMembersCriterion();
  ~RelationWithPoiMembersCriterion() = default;

  ElementCriterionPtr clone() override
  { return std::make_shared<RelationWithPoiMembersCriterion>(); }

  QString getCriterion() const override;

  GeometryType getGeometryType() const override;

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString toString() const override { return className(); }
  QString getDescription() const override { return "Identifies relations with POI members"; }
};

}
#endif // RELATION_WITH_POI_MEMBERS_CRITERION_H
