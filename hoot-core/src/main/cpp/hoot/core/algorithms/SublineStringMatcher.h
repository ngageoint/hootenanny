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
#ifndef SUBLINESTRINGMATCHER_H
#define SUBLINESTRINGMATCHER_H

// Standard
#include <deque>

#include "linearreference/WaySublineMatchString.h"

// hoot
#include <hoot/core/util/Configurable.h>

namespace hoot
{

/**
 * Finds matching sublines in linear elements.
 */
class SublineStringMatcher : public Configurable
{
public:

  static string className() { return "hoot::SublineStringMatcher"; }

  SublineStringMatcher() {}

  /**
   * Given two elements find one or more collections of sublines that match. The input elements may
   * be either multilinestrings or simple ways. The output collection of sublines matches must not
   * overlap.
   */
  virtual WaySublineMatchString findMatch(const ConstOsmMapPtr& map, const ConstElementPtr& e1,
    const ConstElementPtr& e2, Meters maxRelevantDistance = -1) const = 0;

  virtual void setConfiguration(const Settings& s) = 0;

  virtual void setMaxRelevantAngle(Radians r) = 0;

  virtual void setMinSplitSize(Meters minSplitSize) = 0;

  virtual void setHeadingDelta(Meters headingDelta) = 0;
};

typedef boost::shared_ptr<SublineStringMatcher> SublineStringMatcherPtr;

}

#endif // SUBLINESTRINGMATCHER_H
