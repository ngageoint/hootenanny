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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef PARTIALNETWORKMERGER_H
#define PARTIALNETWORKMERGER_H

// hoot
#include <hoot/core/algorithms/WayMatchStringMerger.h>
#include <hoot/core/algorithms/linearreference/WayMatchStringMapping.h>
#include <hoot/core/conflate/MergerBase.h>

#include "EdgeMatch.h"
#include "EidMapper.h"
#include "NetworkDetails.h"

namespace hoot
{

/**
 * @todo Rename to NetworkMerger and delete NetworkMerger
 * Merges one or more partial matches. The matches must not be conflicting.
 */
class PartialNetworkMerger : public MergerBase, public EidMapper
{
public:
  /**
   * Constructed with a set of element matching pairs. The pairs are generally Unknown1 as first
   * and Unknown2 as second.
   */
  PartialNetworkMerger(const set<pair<ElementId, ElementId> > &pairs,
    QSet<ConstEdgeMatchPtr> edgeMatches, ConstNetworkDetailsPtr details);

  virtual void apply(const OsmMapPtr& map, vector< pair<ElementId, ElementId> >& replaced) const;

  /**
   * Maps from a retired EID to its latest EID. If this EID has no mapping then the original EID
   * is returned.
   */
  virtual ElementId mapEid(const ElementId& oldEid) const;

  virtual void replace(ElementId oldEid, ElementId newEid);

  virtual QString toString() const;

protected:
  virtual PairsSet& getPairs() { return _pairs; }
  virtual const PairsSet& getPairs() const { return _pairs; }

private:
  PairsSet _pairs;
  QSet<ConstEdgeMatchPtr> _edgeMatches;
  ConstNetworkDetailsPtr _details;
  QHash<ElementId, ElementId> _substitions;
  mutable QList<WayMatchStringMerger::SublineMappingPtr> _allSublineMappings;
  mutable QList<WayMatchStringMergerPtr> _mergerList;

  void _appendSublineMappings(QList<WayMatchStringMerger::SublineMappingPtr> mappings) const;

  void _applyMerger(const OsmMapPtr& map, WayMatchStringMergerPtr merger) const;

  WayMatchStringMergerPtr _createMatchStringMerger(const OsmMapPtr &map,
    vector<pair<ElementId, ElementId> > &replaced,
    ConstEdgeMatchPtr edgeMatch) const;

  void _processFullMatch(const OsmMapPtr& map, vector<pair<ElementId, ElementId> > &replaced) const;

  void _processStubMatch(const OsmMapPtr& map, vector<pair<ElementId, ElementId> > &replaced,
    ConstEdgeMatchPtr edgeMatch) const;

  void _splitAllWays(const OsmMapPtr& map, vector<pair<ElementId, ElementId> > &replaced,
    QList<WayMatchStringMerger::SublineMappingPtr> mappings) const;
};

}

#endif // PARTIALNETWORKMERGER_H
