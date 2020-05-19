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
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef HIGHWAY_TAG_ONLY_MERGER_H
#define HIGHWAY_TAG_ONLY_MERGER_H

// Hoot
#include <hoot/core/conflate/IdSwap.h>
#include <hoot/core/conflate/highway/HighwaySnapMerger.h>
#include <hoot/core/conflate/network/PartialNetworkMerger.h>

namespace hoot
{

/**
 * merges roads together by merging tags only, keeping ref1 tags
 * merges bridges separately from roads; merges tags and optionally geometries for bridges
 *
 * The inheritance from HighwaySnapMerger is to support the geometry merging option for bridges.
 */
class HighwayTagOnlyMerger : public HighwaySnapMerger
{

public:

  static std::string className() { return "hoot::HighwayTagOnlyMerger"; }

  HighwayTagOnlyMerger();
  HighwayTagOnlyMerger(const std::set<std::pair<ElementId, ElementId>>& pairs,
                       const std::shared_ptr<SublineStringMatcher>& sublineMatcher);
  // This is definitely not ideal to be passing a Network Conflation merger in here like this to
  // deal with bridge merging. At the very least, passing in a MergerPtr would be less brittle.
  // Lots of refactoring would likely need to be done to avoid this, however.
  HighwayTagOnlyMerger(
    const std::set<std::pair<ElementId, ElementId>>& pairs,
    std::shared_ptr<PartialNetworkMerger> networkMerger);

  ~HighwayTagOnlyMerger();

  virtual QString getDescription() const
  { return "Merges road tags only with minimal geometry exceptions"; }

protected:

  virtual bool _mergePair(
    const OsmMapPtr& map, ElementId eid1, ElementId eid2,
    std::vector<std::pair<ElementId, ElementId>>& replaced) override;

private:

  bool _performBridgeGeometryMerging;
  std::shared_ptr<PartialNetworkMerger> _networkMerger;

  void _determineKeeperFeature(ElementPtr element1, ElementPtr element2, ElementPtr& keeper,
                               ElementPtr& toRemove, bool& removeSecondaryElement);

  void _handleOneWayStreetReversal(ElementPtr elementWithTagsToKeep,
                                   ConstElementPtr elementWithTagsToRemove, const OsmMapPtr& map);
  bool _conflictExists(ConstElementPtr elementWithTagsToKeep,
                       ConstElementPtr elementWithTagsToRemove) const;

  void _copyTagsToWayMembers(ElementPtr e1, ElementPtr e2, const OsmMapPtr& map);
  bool _mergeWays(ElementPtr elementWithTagsToKeep, ElementPtr elementWithTagsToRemove,
                  const bool removeSecondaryElement, const OsmMapPtr& map,
                  std::vector<std::pair<ElementId, ElementId>>& replaced);
};

typedef std::shared_ptr<HighwayTagOnlyMerger> HighwayTagOnlyMergerPtr;

}

#endif // HIGHWAY_TAG_ONLY_MERGER_H
