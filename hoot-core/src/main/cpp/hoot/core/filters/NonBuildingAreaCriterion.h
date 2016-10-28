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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef NONBUILDINGAREACRITERION_H
#define NONBUILDINGAREACRITERION_H

// hoot
#include "ElementCriterion.h"

namespace hoot
{

/**
 * A filter that will either keep or remove areas, but not if the areas are also buildings.
 *
 * TODO: should be able to accomplish the same thing with a not building and is area chain but
 * couldn't.  See comments in train-area/RemoveIrrelevants.js
 */
class NonBuildingAreaCriterion : public ElementCriterion
{
public:

  static string className() { return "hoot::NonBuildingAreaCriterion"; }

  NonBuildingAreaCriterion();

  bool isSatisfied(const shared_ptr<const Element> &e) const;

  virtual ElementCriterion* clone() { return new NonBuildingAreaCriterion(); }

};

}

#endif // NONBUILDINGAREACRITERION_H
