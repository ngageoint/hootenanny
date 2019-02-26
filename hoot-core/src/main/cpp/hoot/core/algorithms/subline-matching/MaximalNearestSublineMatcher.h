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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef MAXIMALNEARESTSUBLINEMATCHER_H
#define MAXIMALNEARESTSUBLINEMATCHER_H

#include <hoot/core/algorithms/subline-matching/SublineMatcher.h>

namespace hoot
{

class MaximalNearestSublineMatcher : public SublineMatcher
{
public:

  static std::string className() { return "hoot::MaximalNearestSublineMatcher"; }

  MaximalNearestSublineMatcher();

  virtual WaySublineMatchString findMatch(const ConstOsmMapPtr& map, const ConstWayPtr& way1,
    const ConstWayPtr& way2, double& score, Meters maxRelevantDistance) const override;

  virtual void setMaxRelevantAngle(Radians r) override { _maxRelevantAngle = r; }

  virtual void setMinSplitSize(Meters minSplitSize) override { _minSplitSize = minSplitSize; }

  virtual void setHeadingDelta(Meters headingDelta) override { _headingDelta = headingDelta; }

  virtual QString getDescription() const
  { return "Matches lines based on the maximal nearest subline found"; }

private:

  Meters _minSplitSize;
  Radians _maxRelevantAngle;
  Meters _headingDelta;

  void _snapToEnds(const ConstOsmMapPtr &map, std::vector<WayLocation> &wl) const;
};

}

#endif // MAXIMALNEARESTSUBLINEMATCHER_H
