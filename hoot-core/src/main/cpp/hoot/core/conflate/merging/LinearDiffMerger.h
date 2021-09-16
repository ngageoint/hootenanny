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
#ifndef LINEAR_DIFF_MERGER_H
#define LINEAR_DIFF_MERGER_H

// Hoot
#include <hoot/core/conflate/merging/LinearMergerAbstract.h>

namespace hoot
{

/**
 * A merger for Differential Conflation
 *
 * There appears to be a potential bug when using this as a merger where hoot ms relations are
 * left in the output. Whether that happens or not is dependent upon the ordering or merges during
 * conflate. Its the callers responsbility to clear out these relations after usage.
 */
class LinearDiffMerger : public LinearMergerAbstract
{

public:

  static QString className() { return "LinearDiffMerger"; }

  LinearDiffMerger();
  LinearDiffMerger(
    const std::set<std::pair<ElementId, ElementId>>& pairs,
    const std::shared_ptr<SublineStringMatcher>& sublineMatcher);
  virtual ~LinearDiffMerger() = default;

  /**
   * @see OsmMapOperation
   */
  void apply(const OsmMapPtr& map, std::vector<std::pair<ElementId, ElementId>>& replaced) override;

  QString getDescription() const override
  { return "Merges linear features by keeping only matching portions from secondary elements"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

  void setTreatReviewsAsMatches(bool treatReviewsAsMatches)
  { _treatReviewsAsMatches = treatReviewsAsMatches; }

protected:

  bool _mergePair(
    const ElementId& eid1, const ElementId& eid2,
    std::vector<std::pair<ElementId, ElementId>>& replaced) override;

  /*
   * @see LinearMergerAbstract
   */
  WaySublineMatchString _matchSubline(ElementPtr e1, ElementPtr e2) override;

private:

  // see differential.treat.reviews.as.matches
  bool _treatReviewsAsMatches;

  /*
   * Matches the ways together and creates a new merged way with only the non-matching portions from
   * way2 and adds it to the map. way1 and the matching portions of way2 are removed from the map.
   *
   * @param way1 reference feature
   * @param way2 secondary feature
   * @param replaced element ID mapping for replaced features
   * @param matched determines whether a match was found between way1 and way2
   * @return true if a review is required between the two features; false otherwise
   */
  bool _findAndProcessMatch(
    const WayPtr& way1, const WayPtr& way2, std::vector<std::pair<ElementId, ElementId>>& replaced,
    bool& matched);
};

}

#endif // LINEAR_DIFF_MERGER_H
