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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "MultiLineStringVisitor.h"

// geos
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LineString.h>

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Factory.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, MultiLineStringVisitor)

MultiLineStringVisitor::MultiLineStringVisitor() :
_provider(),
_ls(0)
{
}

MultiLineString* MultiLineStringVisitor::createMultiLineString()
{
  if (_ls != 0)
  {
    return GeometryFactory::getDefaultInstance()->createMultiLineString(_ls);
  }
  else
  {
    return GeometryFactory::getDefaultInstance()->createMultiLineString();
  }
}

void MultiLineStringVisitor::visit(const ConstElementPtr& e)
{
  if (!e)
  {
    return;
  }

  if (e->getElementType() == ElementType::Way)
  {
    ConstWayPtr w = std::dynamic_pointer_cast<const Way>(e);
    visit(w);
  }
}

void MultiLineStringVisitor::visit(const ConstWayPtr& w)
{
  if (!w)
  {
    return;
  }

  if (w->getNodeCount() >= 2)
  {
    if (_ls == 0)
    {
      _ls = new vector<Geometry*>();
    }

    Geometry* g = ElementToGeometryConverter(_provider).convertToLineString(w)->clone();
    _ls->push_back(g);
  }
}

}
