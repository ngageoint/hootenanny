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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "ElementIdCriterion.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, ElementIdCriterion)

ElementIdCriterion::ElementIdCriterion()
{
}

ElementIdCriterion::ElementIdCriterion(const ElementId& id)
{
  _ids.insert(id);
}

ElementIdCriterion::ElementIdCriterion(const std::set<ElementId>& ids) :
_ids(ids)
{
}

ElementIdCriterion::ElementIdCriterion(const ElementType& elementType, const std::set<long>& ids)
{
  for (std::set<long>::const_iterator it = ids.begin(); it != ids.end(); ++it)
  {
    _ids.insert(ElementId(elementType, *it));
  }
}

void ElementIdCriterion::setConfiguration(const Settings& conf)
{
  ConfigOptions configOpts(conf);
  _ids.clear();
  const QStringList idStrs = configOpts.getElementIdCriterionIds();
  for (int i = 0; i < idStrs.size(); i++)
  {
    _ids.insert(ElementId(idStrs.at(i)));
  }
}

bool ElementIdCriterion::isSatisfied(const ConstElementPtr& e) const
{
  return _ids.find(e->getElementId()) != _ids.end();
}

}
