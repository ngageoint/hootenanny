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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef SUBLINEMATCHER_H
#define SUBLINEMATCHER_H

// hoot
#include <hoot/core/algorithms/linearreference/WaySublineMatchString.h>
#include <hoot/core/info/ApiEntityInfo.h>

namespace hoot
{

/**
 * @brief The SublineMatcher class given two ways, calculates zero or more likely matching sublines.
 *
 * The definition of a matching subline is implementation dependent.
 */
class SublineMatcher : public ApiEntityInfo
{
public:

  static QString className() { return "hoot::SublineMatcher"; }

  SublineMatcher() :
  _minSplitSize(ConfigOptions().getWayMergerMinSplitSize()),
  _maxRelevantAngle(ConfigOptions().getWayMatcherMaxAngle()),
  _headingDelta(ConfigOptions().getWayMatcherHeadingDelta())
  {
  }
  virtual ~SublineMatcher() = default;

  /**
   * @param maxRelevantDistance This value is set on a per match basis because it tends to vary
   *  based on the CE of the inputs. If set to -1 then the value is derived based on the CE of the
   *  input ways.
   */
  virtual WaySublineMatchString findMatch(
    const ConstOsmMapPtr& map, const ConstWayPtr& way1, const ConstWayPtr& way2, double& score,
    Meters maxRelevantDistance = -1) const = 0;

  Meters getMinSplitSize() const { return _minSplitSize; }
  Meters getMaxRelevantAngle() const { return _maxRelevantAngle; }
  Meters getHeadingDelta() const { return _headingDelta; }

  void setMaxRelevantAngle(Radians angle) { _maxRelevantAngle = angle; }
  void setMinSplitSize(Meters minSplitSize) { _minSplitSize = minSplitSize; }
  void setHeadingDelta(Meters headingDelta) { _headingDelta = headingDelta; }

  QString toString() const override { return ""; }

protected:

  Meters _minSplitSize;
  Radians _maxRelevantAngle;
  Meters _headingDelta;
};

using SublineMatcherPtr = std::shared_ptr<SublineMatcher>;

}

#endif // SUBLINEMATCHER_H
