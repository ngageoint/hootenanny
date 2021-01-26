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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef NONCONFLATABLECRITERION_H
#define NONCONFLATABLECRITERION_H

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/elements/ConstOsmMapConsumer.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/criterion/GeometryTypeCriterion.h>

// Qt
#include <QList>

namespace hoot
{

/**
 * A filter that will remove elements that aren't conflatable by Hootenanny. These are elements
 * for which we have no matchers defined.
 */
class NonConflatableCriterion : public ElementCriterion, public ConstOsmMapConsumer,
  public Configurable
{

public:

  static QString className() { return "hoot::NonConflatableCriterion"; }

  NonConflatableCriterion();
  NonConflatableCriterion(ConstOsmMapPtr map);
  virtual ~NonConflatableCriterion() = default;

  virtual bool isSatisfied(const ConstElementPtr& e) const override;

  virtual void setConfiguration(const Settings& conf);

  virtual ElementCriterionPtr clone()
  {
    return ElementCriterionPtr(new NonConflatableCriterion(_map));
  }

  virtual QString getDescription() const { return "Identifies features that are not conflatable"; }

  virtual QString getName() const override { return className(); }

  virtual QString getClassName() const override { return className(); }

  virtual void setOsmMap(const OsmMap* map) { _map = map->shared_from_this(); }

  void setGeometryTypeFilter(const GeometryTypeCriterion::GeometryType& filter)
  { _geometryTypeFilter = filter; }
  void setIgnoreGenericConflators(bool ignore) { _ignoreGenericConflators = ignore; }
  void setIgnoreChildren(bool ignore) { _ignoreChildren = ignore; }

private:

  ConstOsmMapPtr _map;

  bool _ignoreChildren;

  // allows for only checking conflatable types that support a specific geometry for optimization
  // purposes
  GeometryTypeCriterion::GeometryType _geometryTypeFilter;

  bool _ignoreGenericConflators;
};

}

#endif // NONCONFLATABLECRITERION_H
