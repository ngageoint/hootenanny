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
