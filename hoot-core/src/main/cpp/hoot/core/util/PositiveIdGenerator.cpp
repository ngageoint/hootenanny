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
 * @copyright Copyright (C) 2015, 2016, 2017, 2021 Maxar (http://www.maxar.com/)
 */

#include "PositiveIdGenerator.h"

// Hoot
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(IdGenerator, PositiveIdGenerator)

PositiveIdGenerator::PositiveIdGenerator()
{
  reset();
}

IdGeneratorPtr PositiveIdGenerator::clone() const
{
  std::shared_ptr<PositiveIdGenerator> result = std::make_shared<PositiveIdGenerator>();
  result->_nodeId = _nodeId;
  result->_wayId = _wayId;
  result->_relationId = _relationId;
  return result;
}

void PositiveIdGenerator::reset()
{
  _wayId = (long)ConfigOptions().getIdGeneratorWayStart();
  _nodeId = (long)ConfigOptions().getIdGeneratorNodeStart();
  _relationId = (long)ConfigOptions().getIdGeneratorRelationStart();
}

}

