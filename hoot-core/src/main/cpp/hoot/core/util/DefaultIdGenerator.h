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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef DEFAULTIDGENERATOR_H
#define DEFAULTIDGENERATOR_H

#include <hoot/core/util/IdGenerator.h>

namespace hoot
{

class DefaultIdGenerator : public IdGenerator
{
public:

  static QString className() { return "DefaultIdGenerator"; }

  DefaultIdGenerator();
  ~DefaultIdGenerator() = default;

  IdGeneratorPtr clone() const override;

  long createNodeId() override { return --_nodeId; }
  long createRelationId() override { return --_relationId; }
  long createWayId() override { return --_wayId; }

  void ensureNodeBounds(long nid) override { _nodeId = std::min(nid, _nodeId); }
  void ensureRelationBounds(long rid) override { _relationId = std::min(rid, _relationId); }
  void ensureWayBounds(long wid) override { _wayId = std::min(wid, _wayId); }

  void reset() override;

private:

  long _nodeId;
  long _relationId;
  long _wayId;
};

}

#endif // DEFAULTIDGENERATOR_H
