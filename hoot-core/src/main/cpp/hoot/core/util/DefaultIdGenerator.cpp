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
 * @copyright Copyright (C) 2015, 2016, 2017, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "DefaultIdGenerator.h"

// hoot
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(IdGenerator, DefaultIdGenerator)

DefaultIdGenerator::DefaultIdGenerator()
{
  reset();
}

IdGeneratorPtr DefaultIdGenerator::clone() const
{
  std::shared_ptr<DefaultIdGenerator> result = std::make_shared<DefaultIdGenerator>();
  result->_nodeId = _nodeId;
  result->_wayId = _wayId;
  result->_relationId = _relationId;
  return result;
}

void DefaultIdGenerator::reset()
{
  _wayId = static_cast<long>(ConfigOptions().getIdGeneratorWayStart());
  _nodeId = static_cast<long>(ConfigOptions().getIdGeneratorNodeStart());
  _relationId = static_cast<long>(ConfigOptions().getIdGeneratorRelationStart());
}

}
