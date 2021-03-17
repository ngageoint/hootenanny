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
#ifndef MAXIMALSUBLINEMATCHER_H
#define MAXIMALSUBLINEMATCHER_H

#include <hoot/core/algorithms/subline-matching/SublineMatcher.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

/**
 * A SublineMatcher facade for MaximalSubline.
 */
class MaximalSublineMatcher : public SublineMatcher, public Configurable
{
public:

  static QString className() { return "hoot::MaximalSublineMatcher"; }

  MaximalSublineMatcher();
  ~MaximalSublineMatcher() = default;

  /**
   * @param maxRelevantDistance This value is set on a per match basis because it tends to vary
   *  based on the CE of the inputs. If set to -1 then the value is derived based on the CE of the
   *  input ways.
   */
  WaySublineMatchString findMatch(const ConstOsmMapPtr& map, const ConstWayPtr& way1,
    const ConstWayPtr& way2, double& score, Meters maxRelevantDistance = -1) const override;

  void setConfiguration(const Settings &conf) override;

  void setMaxRelevantAngle(Radians r) override { _maxAngle = r; }
  void setMinSplitSize(Meters minSplitSize) override { _minSplitSize = minSplitSize; }
  void setHeadingDelta(Meters /*headingDelta*/) override { }

  void setMaxRecursions(int maxRecursions) { _maxRecursions = maxRecursions; }

  QString getDescription() const override
  { return "Matches lines based on the longest matching subline found"; }

  QString getName() const override { return className(); }

  QString getClassName() const override { return className(); }

private:

  Radians _maxAngle;
  Meters _minSplitSize;

  // See MaximalSubline::__maxRecursions
  int _maxRecursions;
};

}

#endif // MAXIMALSUBLINEMATCHER_H
