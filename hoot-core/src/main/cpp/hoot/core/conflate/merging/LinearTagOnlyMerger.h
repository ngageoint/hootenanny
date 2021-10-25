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
#ifndef LINEAR_TAG_ONLY_MERGER_H
#define LINEAR_TAG_ONLY_MERGER_H

// Hoot
#include <hoot/core/conflate/IdSwap.h>
#include <hoot/core/conflate/merging/LinearSnapMerger.h>
#include <hoot/core/conflate/network/PartialNetworkMerger.h>
#include <hoot/core/schema/TagMerger.h>

namespace hoot
{

/**
 * This merges roads together by merging tags only and keeping ref1 tags.
 *
 * Bridges are optionally merged separately from roads. The inheritance from LinearSnapMerger is in
 * part to support the geometry merging option for bridges. Technically, any linear feature could be
 * merged with this but none other than roads have been tested.
 */
class LinearTagOnlyMerger : public LinearSnapMerger
{

public:

  static QString className() { return "LinearTagOnlyMerger"; }

  LinearTagOnlyMerger();
  // This is definitely not ideal to be passing a Network Conflation merger in here like this to
  // deal with bridge merging. At the very least, passing in a MergerPtr would be less brittle.
  // Lots of refactoring would likely need to be done to avoid this, however.
  LinearTagOnlyMerger(
    const std::set<std::pair<ElementId, ElementId>>& pairs,
    std::shared_ptr<PartialNetworkMerger> networkMerger);
  ~LinearTagOnlyMerger() = default;

  QString getDescription() const override
  { return "Merges linear feature tags only with minimal geometry exceptions"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

  void setTagMerger(TagMergerPtr tagMerger) { _tagMerger = tagMerger; }

protected:

  bool _mergePair(
    const ElementId& eid1, const ElementId& eid2,
    std::vector<std::pair<ElementId, ElementId>>& replaced) override;

private:

  bool _performBridgeGeometryMerging;
  std::shared_ptr<PartialNetworkMerger> _networkMerger;
  TagMergerPtr _tagMerger;

  void _determineKeeperFeature(
    ElementPtr element1, ElementPtr element2, ElementPtr& keeper, ElementPtr& toRemove,
    bool& removeSecondaryElement) const;

  void _handleOneWayStreetReversal(
    ElementPtr elementWithTagsToKeep, ConstElementPtr elementWithTagsToRemove) const;
 
  bool _conflictExists(
    ConstElementPtr elementWithTagsToKeep, ConstElementPtr elementWithTagsToRemove) const;

  bool _mergeWays(
    ElementPtr elementWithTagsToKeep, ElementPtr elementWithTagsToRemove,
    const bool removeSecondaryElement,
    std::vector<std::pair<ElementId, ElementId>>& replaced) const;
};

using LinearTagOnlyMergerPtr = std::shared_ptr<LinearTagOnlyMerger>;

}

#endif // LINEAR_TAG_ONLY_MERGER_H
