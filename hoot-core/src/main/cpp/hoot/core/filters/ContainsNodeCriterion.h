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
 * @copyright Copyright (C) 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef CONTAINSNODECRITERION_H
#define CONTAINSNODECRITERION_H

#include "ElementCriterion.h"


namespace hoot
{

/**
 * A criterion that determines if an element contains the given nodeId
 */
class ContainsNodeCriterion : public ElementCriterion
{

public:

  ContainsNodeCriterion() {}

  static string className() { return "hoot::ContainsNodeCriterion"; }

  explicit ContainsNodeCriterion(long nodeId): _nodeId(nodeId)
  {
    // This space intentionally left blank
  }

  bool isSatisfied(const boost::shared_ptr<const Element>& e) const;

  ContainsNodeCriterion* clone() { return new ContainsNodeCriterion(_nodeId); }

private:

  long _nodeId;

};

}
#endif // CONTAINSNODECRITERION_H
