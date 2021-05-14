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
#ifndef LINEAR_SNAP_MERGER_H
#define LINEAR_SNAP_MERGER_H

// Hoot
#include <hoot/core/conflate/merging/LinearMergerAbstract.h>

namespace hoot
{

class WaySublineCollection;

/**
 * Merges linear geometries and tags by snapping the secondary geometry to the reference geometry
 *
 * Note that this was originally written specifically for roads, but now is used by several linear
 * script conflate routines.
 */
class LinearSnapMerger : public LinearMergerAbstract
{

public:

  static QString className() { return "hoot::LinearSnapMerger"; }

  LinearSnapMerger();
  LinearSnapMerger(
    const std::set<std::pair<ElementId, ElementId>>& pairs,
    const std::shared_ptr<SublineStringMatcher>& sublineMatcher);
  ~LinearSnapMerger() = default;

  QString getDescription() const override
  { return "Merges linear features by snapping geometries"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

protected:

  // When roads are split and the pieces stored in multilinestring relations, this decides if the
  // tags get removed from the constituent way members.
  bool _removeTagsFromWayMembers;

  // Determines whether we add a custom tag to any multilinestring relation created during merging.
  // This is useful for getting rid of them later, if necessary.
  bool _markAddedMultilineStringRelations;

  bool _mergePair(
    ElementId eid1, ElementId eid2,
    std::vector<std::pair<ElementId, ElementId>>& replaced) override;

  /*
   * @see LinearMergerAbstract
   */
  WaySublineMatchString _matchSubline(ElementPtr e1, ElementPtr e2) override;

private:

  // for white box testing
  friend class LinearSnapMergerTest;

  static int logWarnCount;

  bool _checkForIdenticalElements(const ElementPtr& e1, const ElementPtr& e2) const;

  /*
   * Snap the ends of snapee that match with either end point of middle to snapTo's end points.
   */
  void _snapEnds(ElementPtr snapee, ElementPtr snapTo) const;
  void _snapEnds(WayPtr snapee, WayPtr middle, WayPtr snapTo) const;
  void _snapEnd(WayPtr snapee, NodePtr replacedNode, NodePtr replacementNode) const;
  /*
   * Splits the splitee up into a match element and a scrap element. All the tags are assigned
   * appropriately and the match and scrap are added to the replaced list and added to the map.
   * The original elements are deleted.
   */
  void _splitElement(
    const WaySublineCollection& s, const std::vector<bool>& reverse,
    std::vector<std::pair<ElementId, ElementId>>& replaced,
    const ConstElementPtr& splitee, ElementPtr& match, ElementPtr& scrap) const;

  void _mergeTags(const Tags& e1Tags, const Tags& e2Tags, const ElementPtr& e1Match) const;

  void _handleSplitWay(
    const ElementPtr& e1, const ElementPtr& scraps1, const ElementPtr& e1Match,
    const bool swapWayIds);
  void _handleSplitWay(
    const ElementId& eid1, const ElementPtr& scraps1, const ElementPtr& e1Match,
    std::vector<std::pair<ElementId, ElementId>>& replaced);
  void _updateScrapParent(long id, const ElementPtr& scrap);
  void _manageElementIds(
    const WayPtr& w1, const WayPtr& w2, const WayPtr& wMatch, const ElementPtr& scraps1,
    const ElementPtr& scraps2);
  void _handleScrapsIds(const ElementPtr& scraps, const WayPtr& way) const;
  void _swapSecondaryElementWithScraps(
    const ElementId& secElementId, const ElementPtr& matchElement, const ElementPtr& scraps) const;
  void _dropSecondaryElements(
    const ElementId& eid1, const ElementId& eidMatch1, const ElementId& eid2,
    const ElementId& eidMatch2) const;

  void _validateMarkedMultilineStringRelations(const ElementPtr& element) const;
};

using LinearSnapMergerPtr = std::shared_ptr<LinearSnapMerger>;

}

#endif // LINEAR_SNAP_MERGER_H
