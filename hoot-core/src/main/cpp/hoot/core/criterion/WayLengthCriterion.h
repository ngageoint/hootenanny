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
 * @copyright Copyright (C) 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef WAY_LENGTH_CRITERION_H
#define WAY_LENGTH_CRITERION_H

// hoot
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/util/NumericComparisonType.h>
#include <hoot/core/elements/ConstOsmMapConsumer.h>
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

class WayLengthCriterion : public ElementCriterion, public ConstOsmMapConsumer
{
public:

  static QString className() { return "hoot::WayLengthCriterion"; }

  WayLengthCriterion();
  WayLengthCriterion(ConstOsmMapPtr map);
  WayLengthCriterion(const double comparisonLength,
                     const NumericComparisonType& numericComparisonType, ConstOsmMapPtr map);
  virtual ~WayLengthCriterion() = default;

  /**
   * @see ElementCriterion
   */
  virtual bool isSatisfied(const ConstElementPtr& e) const override;

  virtual ElementCriterionPtr clone() override
  { return ElementCriterionPtr(new WayLengthCriterion(_map)); }

  virtual QString getDescription() const override
  { return "Identifies ways that meet a length threshold"; }

  virtual QString getName() const override { return className(); }

  virtual QString getClassName() const override { return className(); }

  virtual void setOsmMap(const OsmMap* map) { _map = map->shared_from_this(); }

private:

  double _comparisonLength;
  NumericComparisonType _numericComparisonType;

  ConstOsmMapPtr _map;
};

}

#endif // WAY_LENGTH_CRITERION_H
