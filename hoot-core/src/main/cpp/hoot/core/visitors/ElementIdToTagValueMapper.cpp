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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ElementIdToTagValueMapper.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, ElementIdToTagValueMapper)

ElementIdToTagValueMapper::ElementIdToTagValueMapper()
{
}

void ElementIdToTagValueMapper::addCriterion(const ElementCriterionPtr& e)
{
  _crit = e;
}

void ElementIdToTagValueMapper::visit(const ConstElementPtr& e)
{
  if (_tagKey.trimmed().isEmpty())
  {
    throw IllegalArgumentException("No keys specified for ElementIdToTagValueMapper.");
  }

  if (e->getTags().contains(_tagKey) && (!_crit || _crit->isSatisfied(e)))
  {
    LOG_TRACE(e->getElementId() << ";" << e->getTags()[_tagKey]);
    _idToTagValueMappings[e->getElementId()] = e->getTags()[_tagKey];
    _numAffected++;
  }
}

}
