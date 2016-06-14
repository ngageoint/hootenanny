#ifndef INTERSECTIONCRITERION_H
#define INTERSECTIONCRITERION_H

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/ConstOsmMapConsumer.h>
#include <hoot/core/filters/ElementCriterion.h>

namespace hoot
{

/**
 * This criterion is satisified for all highway intersections with 3 or more interacting ways.
 * Multilinestring relations are not handled.
 *
 * This class assumes that IntersectionSplitter was applied to the map before being called.
 */
class IntersectionCriterion : public ElementCriterion, public ConstOsmMapConsumer
{
public:
  static std::string className() { return "hoot::IntersectionCriterion"; }

  IntersectionCriterion() {}

  virtual ~IntersectionCriterion() {}

  IntersectionCriterion(ConstOsmMapPtr map);

  virtual ElementCriterion* clone() { return new IntersectionCriterion(_map); }

  virtual bool isSatisfied(const shared_ptr<const Element>& e) const;

  virtual void setOsmMap(const OsmMap* map);

private:
  set<long> _highwayIds;
  ConstOsmMapPtr _map;
};

}

#endif // INTERSECTIONCRITERION_H
