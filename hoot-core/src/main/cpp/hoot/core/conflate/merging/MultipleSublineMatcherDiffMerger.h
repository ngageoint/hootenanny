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
#ifndef MULTIPLE_SUBLINE_MATCHER_DIFF_MERGER_H
#define MULTIPLE_SUBLINE_MATCHER_DIFF_MERGER_H

// Hoot
#include <hoot/core/conflate/merging/LinearDiffMerger.h>

namespace hoot
{

class WaySublineMatchString;

/**
 * TODO
 *
 * @todo This inheritance causes duplicated code. To fix:
 *
 * 1) Get rid of this class.
 * 2) Change MultipleSublineMatcherSnapMerger to inherit SublineStringMatcher and rename it to
 *    MultipleSublineMatcher (?).
 * 3) Set up LinearMergerFactory to populate the subline matcher on any LinearMergerAbstract as
 *    MultipleSublineMatcher when river or line conflation is being done.
 */
class MultipleSublineMatcherDiffMerger : public LinearDiffMerger
{

public:

  static QString className() { return "hoot::MultipleSublineMatcherDiffMerger"; }

  MultipleSublineMatcherDiffMerger();
  MultipleSublineMatcherDiffMerger(
    const std::set<std::pair<ElementId, ElementId>>& pairs,
    const std::shared_ptr<SublineStringMatcher>& sublineMatcher,
    const std::shared_ptr<SublineStringMatcher>& sublineMatcher2);
  ~MultipleSublineMatcherDiffMerger() = default;

  QString getDescription() const override
  { return "Merges ways using differencing with one or more subline matchers"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

  static int getNumTimesBackupMatcherUsed() { return _numTimesBackupMatcherUsed; }

protected:

  /*
   * @see LinearMergerAbstract
   */
  WaySublineMatchString _matchSubline(ElementPtr e1, ElementPtr e2) override;

private:

  // This is our backup matcher to use for long ways for runtime performance reasons. It may be
  // a little less accurate but prevents extremely long ways from slowing things down too much.
  std::shared_ptr<SublineStringMatcher> _sublineMatcher2;

  static int _numTimesBackupMatcherUsed;
};

using MultipleSublineMatcherDiffMergerPtr = std::shared_ptr<MultipleSublineMatcherDiffMerger>;

}

#endif // MULTIPLE_SUBLINE_MATCHER_DIFF_MERGER_H