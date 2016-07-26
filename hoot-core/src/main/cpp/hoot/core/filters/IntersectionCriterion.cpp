#include "IntersectionCriterion.h"

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/conflate/NodeToWayMap.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/schema/OsmSchema.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, IntersectionCriterion)

IntersectionCriterion::IntersectionCriterion(ConstOsmMapPtr map)
{
  setOsmMap(map.get());
}

bool IntersectionCriterion::isSatisfied(const shared_ptr<const Element>& e) const
{
  if (e->getElementType() != ElementType::Node)
  {
    return false;
  }

  shared_ptr<NodeToWayMap> n2w = _map->getIndex().getNodeToWayMap();
  long id = e->getId();

  const set<long>& wids = n2w->getWaysByNode(id);

  // find all ways that are highways (ie roads)
  set<long> hwids;
  for (set<long>::const_iterator it = wids.begin(); it != wids.end(); ++it)
  {
    ConstWayPtr w = _map->getWay(*it);

    if (OsmSchema::getInstance().isLinearHighway(w->getTags(), w->getElementType()))
    {
      hwids.insert(*it);
    }
  }

  bool result = false;
  // three or more ways meeting at a node is an intersection
  if (hwids.size() >= 3)
  {
    result = true;
  }

  return result;
}

void IntersectionCriterion::setOsmMap(const OsmMap *map)
{
  _map = map->shared_from_this();
}

}
