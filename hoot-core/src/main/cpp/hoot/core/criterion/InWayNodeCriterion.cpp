
#include "InWayNodeCriterion.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/elements/Way.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, InWayNodeCriterion)

InWayNodeCriterion::InWayNodeCriterion(const OsmMap& map, const std::vector<long>& wayIds)
{
  for (size_t i = 0; i < wayIds.size(); i++)
  {
    ConstWayPtr w = map.getWay(wayIds[i]);
    const std::vector<long>& nids = w->getNodeIds();

    _nids.insert(nids.begin(), nids.end());
  }
}

bool InWayNodeCriterion::isSatisfied(const boost::shared_ptr<const Element>& e) const
{
  return _nids.find(e->getId()) != _nids.end();
}

}
