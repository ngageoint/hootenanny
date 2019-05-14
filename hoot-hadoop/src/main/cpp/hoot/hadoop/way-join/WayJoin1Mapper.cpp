/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "WayJoin1Mapper.h"

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/filters/TagCriterion.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>
#include <hoot/hadoop/Debug.h>
#include <hoot/hadoop/pbf/PbfRecordWriter.h>

// Pretty Pipes
#include <pp/Factory.h>
#include <pp/HadoopPipesUtils.h>

using namespace std;

namespace hoot
{

PP_FACTORY_REGISTER(pp::Mapper, WayJoin1Mapper)

WayJoin1Mapper::WayJoin1Mapper()
{
}

WayJoin1Mapper::~WayJoin1Mapper()
{
}

void WayJoin1Mapper::_map(OsmMapPtr& m, HadoopPipes::MapContext& context)
{
  LOG_INFO("Starting map");
  string keyStr;
  string valueStr;
  keyStr.resize(sizeof(int64_t));
  int64_t* key = (int64_t*)keyStr.data();

  // Remove all non-roads.
  boost::shared_ptr<TagCriterion> pCrit(new TagCriterion("highway", ""));
  RemoveElementsVisitor::removeWays(m, pCrit);

  Debug::printTroubled(m);

  // emit the node's ID as the key and x/y as the value.
  valueStr.resize(sizeof(ValueNode));
  ValueNode* valueNode = (ValueNode*)valueStr.data();
  const NodeMap& nm = m->getNodes();
  for (NodeMap::const_iterator it = nm.begin(); it != nm.end(); ++it)
  {
    const ConstNodePtr& n = it->second;

    *key = n->getId();
    valueNode->x = n->getX();
    valueNode->y = n->getY();
    context.emit(keyStr, valueStr);
  }

  // emit the way's nodes as the key and the way's id as the value.
  valueStr.resize(sizeof(ValueWay));
  ValueWay* valueWay = (ValueWay*)valueStr.data();
  const WayMap& wm = m->getWays();
  for (WayMap::const_iterator it = wm.begin(); it != wm.end(); ++it)
  {
    const ConstWayPtr& w = it->second;

    valueWay->id = w->getId();

    const std::vector<long>& nids = w->getNodeIds();
    for (size_t i = 0; i < nids.size(); i++)
    {
      *key = nids[i];
      context.emit(keyStr, valueStr);
    }
  }
}

}
