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

#ifndef HADOOPIDGENERATOR_H
#define HADOOPIDGENERATOR_H

#include <hoot/core/util/IdGenerator.h>
#include <hoot/core/util/HootException.h>

namespace hoot
{

class HadoopIdGenerator : public IdGenerator
{
public:
  HadoopIdGenerator(int taskId, int taskCount, long wayStart, long nodeStart,
    long relationStart = 0);

  virtual ~HadoopIdGenerator() {}

  /**
   * This method doesn't make sense in the context of Hadoop.
   */
  virtual IdGeneratorPtr clone() const { throw NotImplementedException(); }

  virtual long createNodeId();

  virtual long createRelationId();

  virtual long createWayId();

  void ensureNodeBounds(long nid);

  void ensureRelationBounds(long rid);

  void ensureWayBounds(long wid);

  long getMaxNodeId() { return _nodeId; }

  long getMaxRelationId() { return _relationId; }

  long getMaxWayId() { return _wayId; }

  void reset() { throw UnsupportedException(); }

private:

  long _nodeId;
  long _relationId;
  int _stepSize;
  long _wayId;
};

}

#endif // HADOOPIDGENERATOR_H
