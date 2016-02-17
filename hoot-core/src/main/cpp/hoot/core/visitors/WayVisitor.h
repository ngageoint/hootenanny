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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef WAYVISITOR_H
#define WAYVISITOR_H

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/ConstOsmMapConsumer.h>
#include <hoot/core/elements/ElementVisitor.h>

// tgs
#include <tgs/SharedPtr.h>

namespace hoot
{
using namespace std;

/**
 * Base class to ease OsmMapConsumer usage.
 */
class WayVisitor : public ElementVisitor, public ConstOsmMapConsumer
{
public:

  static string className() { return "hoot::WayVisitor"; }

  WayVisitor() {}

  virtual ~WayVisitor() {}

  virtual void setOsmMap(OsmMap* map) { _map = map; }
  virtual void setOsmMap(const OsmMap* /*map*/) { assert(false); }

  virtual void visit(const ConstElementPtr& e)
  {
    if (e->getElementType() == ElementType::Way)
    {
      WayPtr w = _map->getWay(e->getId());
      if (w)
      {
        visit(w);
      }
    }
  }

  virtual void visit(const WayPtr& w) = 0;

protected:
  OsmMap* _map;
};

}
#endif // WAYVISITOR_H
