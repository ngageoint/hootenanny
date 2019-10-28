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

#include "UpdateWayParentVisitor.h"

using namespace std;

namespace hoot
{

UpdateWayParentVisitor::UpdateWayParentVisitor(long oldParentId, long newParentId)
  : _oldParentId(oldParentId),
    _newParentId(newParentId)
{
}

void UpdateWayParentVisitor::visit(const ElementPtr& e)
{
  if (e->getElementType() == ElementType::Way)
  {
    WayPtr w = std::dynamic_pointer_cast<Way>(e);
    if (w->getPid() == _oldParentId)
      w->setPid(_newParentId);
  }
}

}
