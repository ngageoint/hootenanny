#ifndef NETWORKMERGER_H
#define NETWORKMERGER_H

// hoot
#include <hoot/core/conflate/MergerBase.h>

namespace hoot
{

class NetworkMerger : public MergerBase
{
public:
  /**
   * Constructed with a set of element matching pairs. The pairs are generally Unknown1 as first
   * and Unknown2 as second.
   */
  NetworkMerger(const set< pair<ElementId, ElementId> >& pairs);

  virtual void apply(const OsmMapPtr& map, vector< pair<ElementId, ElementId> >& replaced)
    const;

  virtual QString toString() const;

protected:
  virtual PairsSet& getPairs() { return _pairs; }
  virtual const PairsSet& getPairs() const { return _pairs; }

private:
  set< pair<ElementId, ElementId> > _pairs;
};

}

#endif // NETWORKMERGER_H
