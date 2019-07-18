
#include "RecursiveSetTagValueOp.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/Log.h>

// geos
#include <geos/geom/Geometry.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, RecursiveSetTagValueOp)

RecursiveSetTagValueOp::RecursiveSetTagValueOp()
{
}

RecursiveSetTagValueOp::RecursiveSetTagValueOp(ElementCriterionPtr elementCriterion,
                                               const QString& key, const QString& val) :
_crit(elementCriterion),
_key(key),
_val(val)
{
}

void RecursiveSetTagValueOp::apply(std::shared_ptr<OsmMap>& map)
{
  const RelationMap& relations = map->getRelations();
  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
  {
    RelationPtr relation = it->second;
    LOG_VART(relation->getElementId());
    if (_crit->isSatisfied(relation))
    {
      LOG_TRACE("Setting " << _key << "=" << _val << " on " << relation->getElementId() << "...");
      relation->getTags().set(_key, _val);

      for (size_t i = 0; i < relation->getMembers().size(); i++)
      {
        ElementPtr member = map->getElement(relation->getMembers()[i].getElementId());
        LOG_TRACE("Setting " << _key << "=" << _val << " on " << member->getElementId() << "...");
        member->getTags().set(_key, _val);
      }
    }
  }

  const WayMap& ways = map->getWays();
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    WayPtr way = it->second;
    LOG_VART(way->getElementId());
    if (_crit->isSatisfied(way))
    {
      LOG_TRACE("Setting " << _key << "=" << _val << " on " << way->getElementId() << "...");
      way->getTags().set(_key, _val);

      const std::vector<long>& nodeIds = way->getNodeIds();
      for (std::vector<long>::const_iterator it2 = nodeIds.begin(); it2 != nodeIds.end(); ++it2)
      {
        NodePtr node = map->getNode(*it2);
        LOG_TRACE("Setting " << _key << "=" << _val << " on " << node->getElementId() << "...");
        node->getTags().set(_key, _val);
      }
    }
  }

  const NodeMap& nodes = map->getNodes();
  for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
  {
    NodePtr node = it->second;
    LOG_VART(node->getElementId());
    if (_crit->isSatisfied(node))
    {
      LOG_TRACE("Setting " << _key << "=" << _val << " on " << node->getElementId() << "...");
      node->getTags().set(_key, _val);
    }
  }
}

}
