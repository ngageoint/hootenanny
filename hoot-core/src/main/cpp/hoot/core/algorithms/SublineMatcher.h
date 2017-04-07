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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef SUBLINEMATCHER_H
#define SUBLINEMATCHER_H

// hoot
#include <hoot/core/algorithms/linearreference/WaySublineMatchString.h>

namespace hoot
{

/**
 * Given two ways, calculate zero or more likely matching sublines. The definition of a matching
 * subline is implementation dependent.
 */
class SublineMatcher
{
public:
  static std::string className() { return "hoot::SublineMatcher"; }

  SublineMatcher() {}

  virtual ~SublineMatcher() {}

  /**
   * @param maxRelevantDistance This value is set on a per match basis because it tends to vary
   *  based on the CE of the inputs. If set to -1 then the value is derived based on the CE of the
   *  input ways.
   */
  virtual WaySublineMatchString findMatch(const ConstOsmMapPtr& map, const ConstWayPtr& way1,
    const ConstWayPtr& way2, double& score, Meters maxRelevantDistance = -1) const = 0;

  virtual void setMaxRelevantAngle(Radians r) = 0;

  virtual void setMinSplitSize(Meters minSplitSize) = 0;

  virtual void setHeadingDelta(Meters headingDelta)  = 0;
};

typedef boost::shared_ptr<SublineMatcher> SublineMatcherPtr;

}

#endif // SUBLINEMATCHER_H
