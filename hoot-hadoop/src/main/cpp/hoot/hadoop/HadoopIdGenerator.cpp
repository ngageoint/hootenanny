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

#include "HadoopIdGenerator.h"

// Hoot
#include <hoot/core/util/Log.h>

// Standard
#include <algorithm>

namespace hoot
{

HadoopIdGenerator::HadoopIdGenerator(int taskId, int taskCount, long wayStart, long nodeStart,
  long relationStart)
{
  _wayId = wayStart + taskId + 1;
  _nodeId = nodeStart + taskId + 1;
  _relationId = relationStart + taskId + 1;
  _stepSize = taskCount;

  if (_stepSize > 0)
  {
    _wayId = std::max(1l + taskId, _wayId);
    _nodeId = std::max(1l + taskId, _nodeId);
    _relationId = std::max(1l + taskId, _relationId);
  }
  else
  {
    _wayId = std::min(-1l - taskId, _wayId);
    _nodeId = std::min(-1l - taskId, _nodeId);
    _relationId = std::min(-1l - taskId, _relationId);
  }
  LOG_INFO("HadoopIdGenerator() _wayId: " << _wayId << " _nodeId: " << _nodeId <<
           " _relationId: " << _relationId <<  " _stepSize: " << _stepSize);
}

long HadoopIdGenerator::createNodeId()
{
  _nodeId += _stepSize;
  return _nodeId;
}

long HadoopIdGenerator::createRelationId()
{
  _relationId += _stepSize;
  return _relationId;
}

long HadoopIdGenerator::createWayId()
{
  _wayId += _stepSize;
  return _wayId;
}

void HadoopIdGenerator::ensureNodeBounds(long nid)
{
  if (_stepSize > 0)
  {
    _nodeId = std::max(nid, _nodeId);
  }
  else
  {
    _nodeId = std::min(nid, _nodeId);
  }
}

void HadoopIdGenerator::ensureRelationBounds(long rid)
{
  if (_stepSize > 0)
  {
    _relationId = std::max(rid, _relationId);
  }
  else
  {
    _relationId = std::min(rid, _relationId);
  }
}

void HadoopIdGenerator::ensureWayBounds(long wid)
{
  if (_stepSize > 0)
  {
    _wayId = std::max(wid, _wayId);
  }
  else
  {
    _wayId = std::min(wid, _wayId);
  }
}


}
