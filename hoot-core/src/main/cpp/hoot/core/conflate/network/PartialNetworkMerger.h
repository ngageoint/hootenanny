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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef PARTIALNETWORKMERGER_H
#define PARTIALNETWORKMERGER_H

// hoot
#include <hoot/core/algorithms/WayMatchStringMerger.h>
#include <hoot/core/conflate/merging/MergerBase.h>
#include <hoot/core/conflate/network/EdgeMatch.h>
#include <hoot/core/conflate/network/EidMapper.h>
#include <hoot/core/conflate/network/NetworkDetails.h>

namespace hoot
{

/**
 * Merges one or more partial matches. The matches must not be conflicting.
 */
class PartialNetworkMerger : public MergerBase, public EidMapper
{
public:

  static QString className() { return "PartialNetworkMerger"; }

  PartialNetworkMerger();
  /**
   * Constructed with a set of element matching pairs. The pairs are generally Unknown1 as first
   * and Unknown2 as second.
   */
  PartialNetworkMerger(const std::set<std::pair<ElementId, ElementId>>& pairs,
                       const QSet<ConstEdgeMatchPtr>& edgeMatches,
                       const ConstNetworkDetailsPtr& details);
  ~PartialNetworkMerger() = default;

  void apply(const OsmMapPtr& map, std::vector<std::pair<ElementId, ElementId>>& replaced) override;

  /**
   * Maps from a retired EID to its latest EID. If this EID has no mapping then the original EID
   * is returned.
   */
  ElementId mapEid(const ElementId& oldEid) const override;

  void replace(ElementId oldEid, ElementId newEid) override;

  QString toString() const override;

  QString getDescription() const override
  { return "Merges roads matched by the Network Algorithm"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

  bool getNeedsReview() const { return _needsReview; }

private:

  QSet<ConstEdgeMatchPtr> _edgeMatches;
  ConstNetworkDetailsPtr _details;
  QHash<ElementId, ElementId> _substitions;
  mutable QList<WayMatchStringMerger::SublineMappingPtr> _allSublineMappings;
  mutable QList<WayMatchStringMergerPtr> _mergerList;
  bool _needsReview;

  void _appendSublineMappings(QList<WayMatchStringMerger::SublineMappingPtr> mappings) const;

  void _applyMerger(const OsmMapPtr& map, WayMatchStringMergerPtr merger) const;

  WayMatchStringMergerPtr _createMatchStringMerger(const OsmMapPtr &map,
                                                   std::vector<std::pair<ElementId, ElementId>>& replaced,
                                                   ConstEdgeMatchPtr edgeMatch) const;

  void _processFullMatch(const OsmMapPtr& map,
                         std::vector<std::pair<ElementId, ElementId>>& replaced);
  void _processStubMatch(const OsmMapPtr& map,
                         const std::vector<std::pair<ElementId, ElementId>>& replaced,
                         ConstEdgeMatchPtr edgeMatch);
};

}

#endif // PARTIALNETWORKMERGER_H
