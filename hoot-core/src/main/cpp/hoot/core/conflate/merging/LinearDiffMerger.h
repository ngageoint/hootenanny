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
 * @copyright Copyright (C) 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef LINEAR_DIFF_MERGER_H
#define LINEAR_DIFF_MERGER_H

// Hoot
#include <hoot/core/conflate/merging/LinearMergerAbstract.h>

namespace hoot
{

/**
 * TODO
 */
class LinearDiffMerger : public LinearMergerAbstract
{

public:

  static QString className() { return "hoot::LinearDiffMerger"; }

  LinearDiffMerger();
  LinearDiffMerger(
    const std::set<std::pair<ElementId, ElementId>>& pairs,
    const std::shared_ptr<SublineStringMatcher>& sublineMatcher);
  virtual ~LinearDiffMerger() = default;

  void apply(const OsmMapPtr& map, std::vector<std::pair<ElementId, ElementId>>& replaced) override;

  virtual QString getDescription() const { return "TODO"; }
  virtual QString getName() const override { return className(); }
  virtual QString getClassName() const override { return className(); }

  void setTreatReviewsAsMatches(bool treatReviewsAsMatches)
  { _treatReviewsAsMatches = treatReviewsAsMatches; }

protected:

  virtual bool _mergePair(
    ElementId eid1, ElementId eid2,
    std::vector<std::pair<ElementId, ElementId>>& replaced);

private:

  bool _treatReviewsAsMatches;

  /*
   * TODO
   */
  bool _findAndProcessMatch(
    const WayPtr& way1, const WayPtr& way2, std::vector<std::pair<ElementId, ElementId>>& replaced,
    bool& matched);
};

}

#endif // LINEAR_DIFF_MERGER_H
