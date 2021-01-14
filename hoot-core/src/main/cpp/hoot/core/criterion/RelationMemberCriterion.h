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
#ifndef RELATION_MEMBER_CRITERION_H
#define RELATION_MEMBER_CRITERION_H

// hoot
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/elements/ConstOsmMapConsumer.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/criterion/ElementCriterion.h>

namespace hoot
{

/**
 * Identifies relation members
 */
class RelationMemberCriterion : public ElementCriterion, public ConstOsmMapConsumer
{
public:

  static std::string className() { return "hoot::RelationMemberCriterion"; }

  RelationMemberCriterion() = default;
  RelationMemberCriterion(ConstOsmMapPtr map);
  virtual ~RelationMemberCriterion() = default;

  /**
   * @see ElementVisitor
   */
  virtual bool isSatisfied(const ConstElementPtr& e) const override;

  virtual ElementCriterionPtr clone()
  { return ElementCriterionPtr(new RelationMemberCriterion(_map)); }

  virtual QString getDescription() const { return "Identifies relation members"; }

  virtual QString toString() const override
  { return QString::fromStdString(className()).remove("hoot::"); }

  virtual void setOsmMap(const OsmMap* map) { _map = map->shared_from_this(); }

private:

  ConstOsmMapPtr _map;
};

}

#endif // AREACRITERION_H
