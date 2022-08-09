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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef HIGHWAYCRITERION_H
#define HIGHWAYCRITERION_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/ConstOsmMapConsumer.h>
#include <hoot/core/criterion/ConflatableElementCriterion.h>

namespace hoot
{

/**
 * A criterion that will either keep or remove road matches.
 */
class HighwayCriterion : public ConflatableElementCriterion, public ConstOsmMapConsumer
{
public:

  static QString className() { return "HighwayCriterion"; }

  HighwayCriterion(const bool includeRelations = false);
  HighwayCriterion(ConstOsmMapPtr map, const bool includeRelations = false);
  ~HighwayCriterion() override = default;

  bool isSatisfied(const ConstElementPtr& e) const override;
  ElementCriterionPtr clone() override { return std::make_shared<HighwayCriterion>(_map); }

  void setOsmMap(const OsmMap* map) override { _map = map->shared_from_this(); }

  GeometryType getGeometryType() const override { return GeometryType::Line; }
  bool supportsSpecificConflation() const override { return true; }
  QStringList getChildCriteria() const override;

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override { return "Identifies roads"; }
  QString toString() const override { return className(); }

private:

  ConstOsmMapPtr _map;
  // Determines whether we count relations with a highway tag as highways. The main reason we'd want
  // to for conflate is to handle untagged roads in a tagged highway relation. *However*, its still
  // not clear if that situation is valid OSM data. Therefore, have added this switch to allow us
  // to switch back to the original behavior of allowing highway relations to be roads. If the
  // aforementioned situation definitely turns out to be invalid OSM, then this var can be removed.
  bool _includeRelations;
};

}
#endif // HIGHWAYCRITERION_H
