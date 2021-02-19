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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef MULTIPLE_SUBLINE_MATCHER_SNAP_MERGER_H
#define MULTIPLE_SUBLINE_MATCHER_SNAP_MERGER_H

// Hoot
#include <hoot/core/conflate/linear/LinearSnapMerger.h>

namespace hoot
{

class WaySublineMatchString;

/**
 * Merges way features potentially using multiple subline matchers
 *
 * This class primarily exists so that we can select a subline matcher based on properties of the
 * input data for runtime performance reasons.
 */
class MultipleSublineMatcherSnapMerger : public LinearSnapMerger
{

public:

  static QString className() { return "hoot::MultipleSublineMatcherSnapMerger"; }

  MultipleSublineMatcherSnapMerger();
  MultipleSublineMatcherSnapMerger(
    const std::set<std::pair<ElementId, ElementId>>& pairs,
    const std::shared_ptr<SublineStringMatcher>& sublineMatcher,
    const std::shared_ptr<SublineStringMatcher>& sublineMatcher2);
  virtual ~MultipleSublineMatcherSnapMerger() = default;

  virtual QString getDescription() const { return "Merges ways with one or more subline matchers"; }

  virtual QString getName() const override { return className(); }

  virtual QString getClassName() const override { return className(); }

protected:

  /*
   * @see LinearSnapMerger
   */
  virtual WaySublineMatchString _matchSubline(OsmMapPtr map, ElementPtr e1, ElementPtr e2);

private:

  // This is our backup matcher to use for long ways for runtime performance reasons. It may be
  // a little less accurate but prevents extremely long ways from slowing things down too much.
  std::shared_ptr<SublineStringMatcher> _sublineMatcher2;
};

typedef std::shared_ptr<MultipleSublineMatcherSnapMerger> MultipleSublineMatcherSnapMergerPtr;

}

#endif // MULTIPLE_SUBLINE_MATCHER_SNAP_MERGER_H
