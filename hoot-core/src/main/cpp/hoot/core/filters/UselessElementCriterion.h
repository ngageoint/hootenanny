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
 * @copyright Copyright (C) 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef USELESSELEMENTCRITERION_H
#define USELESSELEMENTCRITERION_H

// hoot
#include "ElementCriterion.h"
#include <hoot/core/OsmMap.h>
#include <hoot/core/ConstOsmMapConsumer.h>

namespace hoot
{

/**
 * A filter for detecting elements with no parents or children.
 * For nodes: return true if the node has no parent ways or relations
 * For ways: return true if the way has no child nodes or parent relations
 * For relations: return true if the relation has no parent relations or members
 */
class UselessElementCriterion : public ElementCriterion, public ConstOsmMapConsumer
{
public:

  static std::string className() { return "hoot::UselessElementCriterion"; }

  UselessElementCriterion() { }
  UselessElementCriterion(ConstOsmMapPtr map) : _map(map) {}

  virtual void setOsmMap(const OsmMap* map) { _map = map->shared_from_this(); }

  virtual bool isSatisfied(const boost::shared_ptr<const Element> &e) const;

  virtual ElementCriterionPtr clone() { return ElementCriterionPtr(new UselessElementCriterion()); }

  virtual QString getDescription() const { return "Identifies useless elements"; }

private:

  ConstOsmMapPtr _map;
};

}

#endif // USELESSELEMENTCRITERION_H
