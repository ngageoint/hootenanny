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
#include "ChangesetReplacementCreator4.h"

// Hoot
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/criterion/PolygonCriterion.h>
#include <hoot/core/criterion/PointCriterion.h>
#include <hoot/core/criterion/LinearCriterion.h>
#include <hoot/core/criterion/RelationWithLinearMembersCriterion.h>
#include <hoot/core/criterion/RelationWithPointMembersCriterion.h>
#include <hoot/core/criterion/RelationWithPolygonMembersCriterion.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>
#include <hoot/core/criterion/OrCriterion.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/ops/RemoveElementByEid.h>
#include <hoot/core/util/MapProjector.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ChangesetReplacementCreator, ChangesetReplacementCreator4)

ChangesetReplacementCreator4::ChangesetReplacementCreator4() :
ChangesetReplacementCreator1()
{
}

QMap<GeometryTypeCriterion::GeometryType, ElementCriterionPtr>
  ChangesetReplacementCreator4::_getDefaultGeometryFilters() const
{
  LOG_VART(toString());

  QMap<GeometryTypeCriterion::GeometryType, ElementCriterionPtr> featureFilters;

  ElementCriterionPtr pointCrit(new PointCriterion());
  std::shared_ptr<RelationWithPointMembersCriterion> relationPointCrit(
    new RelationWithPointMembersCriterion());
  relationPointCrit->setAllowMixedChildren(true);
  OrCriterionPtr pointOr(new OrCriterion(pointCrit, relationPointCrit));
  featureFilters[GeometryTypeCriterion::GeometryType::Point] = pointOr;

  ElementCriterionPtr lineCrit(new LinearCriterion());
  std::shared_ptr<RelationWithLinearMembersCriterion> relationLinearCrit(
    new RelationWithLinearMembersCriterion());
  relationLinearCrit->setAllowMixedChildren(true);
  OrCriterionPtr lineOr(new OrCriterion(lineCrit, relationLinearCrit));
  featureFilters[GeometryTypeCriterion::GeometryType::Line] = lineOr;

  // Poly crit has been converted over to encapsulate RelationWithGeometryMembersCriterion, while
  // the other types have not yet (#4151).
  std::shared_ptr<PolygonCriterion> polyCrit(new PolygonCriterion());
  polyCrit->setAllowMixedChildren(true);
  featureFilters[GeometryTypeCriterion::GeometryType::Polygon] = polyCrit;

  return featureFilters;
}

}
