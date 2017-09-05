/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef DEFAULTIDGENERATOR_H
#define DEFAULTIDGENERATOR_H

#include "IdGenerator.h"

namespace hoot
{

class DefaultIdGenerator : public IdGenerator
{
public:
  static std::string className() { return "hoot::DefaultIdGenerator"; }

  DefaultIdGenerator() { reset(); }

  virtual ~DefaultIdGenerator() {}

  virtual IdGeneratorPtr clone() const;

  virtual long createNodeId() { return --_nodeId; }

  virtual long createRelationId() { return --_relationId; }

  virtual long createWayId() { return --_wayId; }

  void ensureNodeBounds(long nid) { _nodeId = std::min(nid, _nodeId); }

  void ensureRelationBounds(long rid) { _relationId = std::min(rid, _relationId); }

  void ensureWayBounds(long wid) { _wayId = std::min(wid, _wayId); }

  void reset();

private:
  long _nodeId;
  long _relationId;
  long _wayId;
};

}

#endif // DEFAULTIDGENERATOR_H
