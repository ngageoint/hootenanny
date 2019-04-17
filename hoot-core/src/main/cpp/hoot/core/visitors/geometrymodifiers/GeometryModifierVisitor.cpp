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

#include "GeometryModifierVisitor.h"
#include "GeometryModifierAction.h"

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>

using namespace std;
using namespace boost;

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, GeometryModifierVisitor)

void GeometryModifierVisitor::visit(const ElementPtr& pElement)
{
  if( _actionDesc.pAction == NULL ) return;
  int filterSize = _actionDesc.filter.size();
  if( filterSize == 0 ) return;

  _numProcessed++;

  // apply _actionDesc filter
  Tags& tags = pElement->getTags();

  int matches = 0;

  foreach (QString filterKey, _actionDesc.filter.keys())
  {
    foreach (QString tagKey, tags.keys())
    {
      if( tagKey == filterKey && tags[tagKey] == _actionDesc.filter[filterKey])
      {
        matches++;
      }
    }
  }

  if( matches == filterSize )
  {
    if( _actionDesc.pAction->process(pElement, _pMap) )
    {
      _numAffected++;
    }
  }
}

} // namespace hoot
