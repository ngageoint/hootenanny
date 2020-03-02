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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef AREACRITERION_H
#define AREACRITERION_H

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/ConstOsmMapConsumer.h>
#include <hoot/core/criterion/ConflatableElementCriterion.h>

namespace hoot
{

/**
 * A criterion that will either keep or remove areas.
 */
class AreaCriterion : public ConflatableElementCriterion, public ConstOsmMapConsumer
{
public:

  static std::string className() { return "hoot::AreaCriterion"; }

  AreaCriterion();
  AreaCriterion(ConstOsmMapPtr map);

  virtual bool isSatisfied(const ConstElementPtr& e) const override;

  bool isSatisfied(const Tags& tags, const ElementType& elementType) const;

  virtual GeometryType getGeometryType() const
  { return GeometryType::Polygon; }

  virtual ElementCriterionPtr clone() { return ElementCriterionPtr(new AreaCriterion(_map)); }

  virtual QString getDescription() const { return "Identifies areas"; }

  virtual QString toString() const override
  { return QString::fromStdString(className()).remove("hoot::"); }

  virtual void setOsmMap(const OsmMap* map) { _map = map->shared_from_this(); }

private:

  ConstOsmMapPtr _map;
  mutable ElementId _currentElementId;
};

}

#endif // AREACRITERION_H
