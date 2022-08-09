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
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef POLYGON_CRITERION_H
#define POLYGON_CRITERION_H

// Hoot
#include <hoot/core/criterion/ConflatableElementCriterion.h>
#include <hoot/core/elements/ConstOsmMapConsumer.h>
#include <hoot/core/criterion/RelationWithPolygonMembersCriterion.h>

namespace hoot
{

/**
 * Identifies polygon features
 */
class PolygonCriterion : public ConflatableElementCriterion, public ConstOsmMapConsumer
{
public:

  static QString className() { return "PolygonCriterion"; }

  PolygonCriterion() = default;
  PolygonCriterion(ConstOsmMapPtr map);
  ~PolygonCriterion() override = default;

  bool isSatisfied(const ConstElementPtr& e) const override;
  ElementCriterionPtr clone() override { return std::make_shared<PolygonCriterion>(_map); }

  void setOsmMap(const OsmMap* map) override;

  bool supportsSpecificConflation() const override { return false; }
  GeometryType getGeometryType() const override { return GeometryType::Polygon; }
  QStringList getChildCriteria() const override;

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString toString() const override { return className(); }
  QString getDescription() const override { return "Identifies polygon features"; }

  void setAllowMixedChildren(bool allow) { _relationCrit.setAllowMixedChildren(allow); }

private:

  ConstOsmMapPtr _map;
  RelationWithPolygonMembersCriterion _relationCrit;
};

}
#endif // POLYGON_CRITERION_H
