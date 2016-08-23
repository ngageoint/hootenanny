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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "CopySubsetOp.h"

#include <hoot/core/elements/ElementVisitor.h>

namespace hoot
{

class AddAllVisitor : public ElementVisitor
{
public:
  AddAllVisitor(ConstOsmMapPtr from, OsmMapPtr to, ElementId exempt = ElementId()) :
    _from(from),
    _to(to),
    _exempt(exempt)
  {
  }

  virtual void visit(const ConstElementPtr& e)
  {
    ElementId eid = e->getElementId();

    // if the element is not exempt and it isn't already in the map
    if (eid != _exempt && _to->containsElement(eid) == false)
    {
      // create a copy of the element
      ElementPtr ee(_from->getElement(eid)->clone());

      // if it is a node, just copy it, we don't need to worry about dependencies.
      if (ee->getElementType() == ElementType::Node)
      {
        _to->addElement(ee);
      }
      // if this is not a node then we need to worry about dependencies.
      else
      {
        // add all of this element's children to the map (we're expempting eid).
        AddAllVisitor v(_from, _to, eid);
        _from->getElement(eid)->visitRo(*_from, v);
        // finally, add this element to the map.
        _to->addElement(ee);
      }
    }
  }

private:
  ConstOsmMapPtr _from;
  OsmMapPtr _to;
  ElementId _exempt;
};

CopySubsetOp::CopySubsetOp(const ConstOsmMapPtr& from, const set<ElementId>& eids) :
  _eids(eids),
  _from(from)
{
}

CopySubsetOp::CopySubsetOp(const ConstOsmMapPtr& from, const vector<long>& wayIds) :
  _from(from)
{
  // Need to get ElementIds from our vector of longs
  for (vector<long>::const_iterator it = wayIds.begin(); it != wayIds.end(); ++it)
  {
    if(from->containsWay(*it))
    { // make sure it's a way
      _eids.insert(ElementId(ElementType::Way, *it));
    }
  }
}

CopySubsetOp::CopySubsetOp(const ConstOsmMapPtr& from, ElementId eid1, ElementId eid2) :
  _from(from)
{
  _eids.insert(eid1);
  _eids.insert(eid2);
}

void CopySubsetOp::apply(shared_ptr<OsmMap> &map)
{
  map->setProjection(_from->getProjection());
  AddAllVisitor v(_from, map);

  for (set<ElementId>::const_iterator it = _eids.begin(); it != _eids.end(); ++it)
  {
    if (_from->containsElement(*it) == false)
    {
      throw IllegalArgumentException("Unable to find element: " + hoot::toString(*it));
    }
    _from->getElement(*it)->visitRo(*_from, v);
  }
}

}
