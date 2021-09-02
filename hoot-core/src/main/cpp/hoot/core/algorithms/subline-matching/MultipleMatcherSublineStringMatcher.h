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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */
#ifndef MULTIPLE_MATCHER_SUBLINE_STRING_MATCHER_H
#define MULTIPLE_MATCHER_SUBLINE_STRING_MATCHER_H

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/algorithms/subline-matching/SublineStringMatcher.h>
#include <hoot/core/algorithms/subline-matching/SublineMatcher.h>

namespace hoot
{

class WaySublineMatchString;

/**
 * @brief The MultipleMatcherSublineStringMatcher class is a facade that wraps usage of multiple
 * subline string matchers.
 *
 * The general idea being that the first matcher is possibly more accurate but expensive to run on
 * some features (e.g. maximal subline matching) and the second matcher is possibly slightly less
 * accurate but less expensive to run (e.g. Frechet subline matching). So, you can run the first one
 * and then back out at a certain point in the processing to run the second.
 */
class MultipleMatcherSublineStringMatcher : public SublineStringMatcher
{
public:

  static QString className() { return "MultipleMatcherSublineStringMatcher"; }

  MultipleMatcherSublineStringMatcher();
  MultipleMatcherSublineStringMatcher(
    const std::shared_ptr<SublineStringMatcher>& sublineMatcher1,
    const std::shared_ptr<SublineStringMatcher>& sublineMatcher2);
  virtual ~MultipleMatcherSublineStringMatcher() = default;

  /**
   * @see SublineStringMatcher
   */
  WaySublineMatchString findMatch(
    const ConstOsmMapPtr& map, const ConstElementPtr& e1, const ConstElementPtr& e2,
    Meters maxRelevantDistance = -1) const override;

  /**
   * @see Configurable
   */
  void setConfiguration(const Settings& s) override;

  void setMaxRelevantAngle(Radians r) override;
  /**
   * @brief setMinSplitSize minSplitSize is not supported at this time.
   */
  void setMinSplitSize(Meters minSplitSize) override;
  void setHeadingDelta(Meters headingDelta) override;
  QString getSublineMatcherName() const override
  {
    return
      _sublineMatcher1->getName() + "," + _sublineMatcher1->getSublineMatcherName() + ";" +
      _sublineMatcher2->getName() + "," + _sublineMatcher2->getSublineMatcherName();
  }

  QString getDescription() const override
  { return "Matches lines based on the maximal subline string found"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

  static int getNumTimesBackupMatcherUsed() { return _numTimesBackupMatcherUsed; }

private:

  // The first matcher to run.
  std::shared_ptr<SublineStringMatcher> _sublineMatcher1;

  // The second matcher to run. This is our backup matcher to use for long ways for runtime
  // performance reasons. It may be a little less accurate but prevents extremely long ways from
  // slowing things down too much.
  std::shared_ptr<SublineStringMatcher> _sublineMatcher2;

  static int _numTimesBackupMatcherUsed;
};

}

#endif // MULTIPLE_MATCHER_SUBLINE_STRING_MATCHER_H
