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
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef LINEAR_MERGER_ABSTRACT_H
#define LINEAR_MERGER_ABSTRACT_H

// Hoot
#include <hoot/core/conflate/merging/MergerBase.h>
#include <hoot/core/conflate/review/ReviewMarker.h>
#include <hoot/core/visitors/LengthOfWaysVisitor.h>
#include <hoot/core/algorithms/subline-matching/SublineStringMatcher.h>

namespace hoot
{

class ShortestFirstComparator
{
public:

  bool operator()(const std::pair<ElementId, ElementId>& p1,
                  const std::pair<ElementId, ElementId>& p2)
  {
    return
      std::min(getLength(p1.first), getLength(p1.second)) <
        std::min(getLength(p2.first), getLength(p2.second));
  }

  Meters getLength(const ElementId& eid)
  {
    Meters result;
    if (_lengthMap.contains(eid))
    {
      result = _lengthMap[eid];
    }
    else
    {
      LengthOfWaysVisitor v;
      v.setOsmMap(map.get());
      map->getElement(eid)->visitRo(*map, v);
      result = v.getLengthOfWays();
      _lengthMap[eid] = result;
    }
    return result;
  }

  // commenting this out results in a crash...what??
  virtual QString getDescription() const { return ""; }

  OsmMapPtr map;

private:

  QHash<ElementId, Meters> _lengthMap;
};

/**
 * Base class for linear feature merging
 */
class LinearMergerAbstract : public MergerBase
{

public:

  static QString className() { return "hoot::LinearMergerAbstract"; }

  LinearMergerAbstract() = default;
  virtual ~LinearMergerAbstract() = default;

  virtual void apply(
    const OsmMapPtr& map, std::vector<std::pair<ElementId, ElementId>>& replaced) override;

  virtual QString toString() const override;

  void setPairs(const std::set<std::pair<ElementId, ElementId>>& pairs) { _pairs = pairs; }
  void setMatchedBy(const QString& matchedBy) { _matchedBy = matchedBy; }
  void setSublineMatcher(const std::shared_ptr<SublineStringMatcher>& sublineMatcher)
  { _sublineMatcher = sublineMatcher; }

protected:

  ReviewMarker _reviewMarker;

  static int logWarnCount;

  virtual PairsSet& _getPairs() override { return _pairs; }
  virtual const PairsSet& _getPairs() const override { return _pairs; }

  std::set<std::pair<ElementId, ElementId>> _pairs;

  std::shared_ptr<SublineStringMatcher> _sublineMatcher;

  // indicates which matcher matched the elements being processed by this merger
  QString _matchedBy;

  /*
   * Return true if pair needs review.
   */
  virtual bool _mergePair(const OsmMapPtr& map, ElementId eid1, ElementId eid2,
    std::vector<std::pair<ElementId, ElementId>>& replaced);

  virtual void _markNeedsReview(const OsmMapPtr& map, ElementPtr e1, ElementPtr e2, QString note,
                                QString reviewType);

  void _removeSpans(OsmMapPtr map, const ElementPtr& w1, const ElementPtr& w2) const;
  void _removeSpans(OsmMapPtr map, const WayPtr& w1, const WayPtr& w2) const;

private:

  /*
   * Returns true if the way directly connects the left and right ways. There is some tolerance
   * for "directly". See Redmine ticket #951 for details.
   */
  bool _directConnect(const ConstOsmMapPtr &map, WayPtr w) const;

  bool _doesWayConnect(long node1, long node2, const ConstWayPtr& w) const;
};

}

#endif // LINEAR_MERGER_ABSTRACT_H
