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
 * @copyright Copyright (C) 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "BuildingLevelsVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, BuildingLevelsVisitor)

BuildingLevelsVisitor::BuildingLevelsVisitor()
  : _totalLevels(0),
    _minLevels(0),
    _maxLevels(0)
{
}

void BuildingLevelsVisitor::visit(const ConstElementPtr& e)
{
  if (_crit.isSatisfied(e))
  {
    const int numLevels = _getNumLevels(e);
    // num levels = 0 denotes no levels tag. We're only going to return stats for buildings with
    // the tag.
    if (numLevels > 0)
    {
      _totalLevels += numLevels;
      if (_minLevels == 0 || numLevels < _minLevels)
        _minLevels = numLevels;
      if (numLevels > _maxLevels)
        _maxLevels = numLevels;
      _numAffected++;
    }
  }
}

int BuildingLevelsVisitor::_getNumLevels(const ConstElementPtr& e) const
{
  // just handling the levels above ground for now
  if (e->getTags().contains("building:levels"))
  {
    bool ok = false;
    const int numLevels = e->getTags().get("building:levels").toInt(&ok);
    if (ok)
      return numLevels;
  }
  return 0;
}

}
