#include "Debug.h"

#include <hoot/core/OsmMap.h>

namespace hoot
{

bool Debug::_initialized = false;
set<long> Debug::_troubledNodes;
set<long> Debug::_troubledWays;

void Debug::_init()
{
  if (_initialized == false)
  {
    _initialized = true;
  }
}

#ifdef DEBUG
bool Debug::printTroubled(const boost::shared_ptr<const OsmMap>& map)
{
  bool result = false;
  const NodeMap& nm = map->getNodes();
  for (NodeMap::const_iterator it = nm.begin(); it != nm.end(); ++it)
  {
    const boost::shared_ptr<Node>& n = it->second;
    if (isTroubledNode(n->getId()))
    {
      LOG_WARN("Found troubled node: " << n->toString());
      result = true;
    }
  }

  const WayMap& wm = map->getWays();
  for (WayMap::const_iterator it = wm.begin(); it != wm.end(); ++it)
  {
    const boost::shared_ptr<Way>& w = it->second;

    if (isTroubledWay(w->getId()))
    {
      LOG_WARN("Found troubled way: " << w->toString());
      result = true;
    }
    else
    {
      vector<long> troubled;
      const vector<long>& nids = w->getNodeIds();
      for (size_t i = 0; i < nids.size(); i++)
      {
        if (isTroubledNode(nids[i]))
        {
          troubled.push_back(nids[i]);
        }
      }
      if (troubled.size() > 0)
      {
        LOG_WARN("Found a way with troubled node(s): " << w->toString());
        LOG_WARN("  troubled nodes: " << troubled);
        result = true;
      }
    }
  }
  return result;
}
#endif

}
