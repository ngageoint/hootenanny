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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "CopyMapSubsetOp.h"

#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/UniqueElementIdVisitor.h>

using namespace std;

namespace hoot
{

class AddAllVisitor : public ConstElementVisitor
{
public:

  AddAllVisitor(ConstOsmMapPtr from, OsmMapPtr to, bool copyChildren = true,
                ElementId exempt = ElementId()) :
    _from(from),
    _to(to),
    _copyChildren(copyChildren),
    _exempt(exempt)
  {
    LOG_VART(_copyChildren);
  }
  virtual ~AddAllVisitor() = default;

  virtual void visit(const ConstElementPtr& e)
  {
    ElementId eid = e->getElementId();
    LOG_VART(eid);

    // If the element is not exempt and it isn't already in the map,
    if (eid != _exempt && _to->containsElement(eid) == false)
    {
      // create a copy of the element.
      ElementPtr ee(_from->getElement(eid)->clone());
      LOG_VART(ee->getElementId());

      // If it is a node, just copy it, as we don't need to worry about dependencies.
      if (ee->getElementType() == ElementType::Node)
      {
        LOG_TRACE("Adding " << ee->getElementId() << "...");
        _to->addElement(ee);
      }
      // If it is not a node, then we need to worry about dependencies.
      else
      {
        // only copy this element's children if we are configured to do so
        if (_copyChildren)
        {
          // Add all of this element's children to the map (we're exempting eid).
          LOG_TRACE("Adding children of " << ee->getElementId() << "...");
          AddAllVisitor v(_from, _to, true, eid);
          _from->getElement(eid)->visitRo(*_from, v);
          //  Add all of the elements affected.
          _elementsAdded.insert(v._elementsAdded.begin(), v._elementsAdded.end());
        }
        // Finally, add this element to the map.
        LOG_TRACE("Adding " << ee->getElementId() << "...");
        _to->addElement(ee);
      }
      //  Add this element to the list.
      _elementsAdded.insert(eid);
    }
  }

  virtual QString getDescription() const { return ""; }
  virtual std::string getClassName() const { return ""; }

  std::set<ElementId>& getElementsAdded() { return _elementsAdded; }

private:

  ConstOsmMapPtr _from;
  OsmMapPtr _to;
  bool _copyChildren;
  ElementId _exempt;
  std::set<ElementId> _elementsAdded;
};

CopyMapSubsetOp::CopyMapSubsetOp(const ConstOsmMapPtr& from, const set<ElementId>& eids) :
_eids(eids),
_from(from),
_copyChildren(true)
{
}

CopyMapSubsetOp::CopyMapSubsetOp(const ConstOsmMapPtr& from, const vector<long>& wayIds) :
_from(from),
_copyChildren(true)
{
  for (vector<long>::const_iterator it = wayIds.begin(); it != wayIds.end(); ++it)
  {
    if (from->containsWay(*it))
    {
      _eids.insert(ElementId(ElementType::Way, *it));
    }
  }
}

CopyMapSubsetOp::CopyMapSubsetOp(const ConstOsmMapPtr& from, ElementId eid) :
_from(from),
_copyChildren(true)
{
  _eids.insert(eid);
}

CopyMapSubsetOp::CopyMapSubsetOp(const ConstOsmMapPtr& from, ElementId eid1, ElementId eid2) :
_from(from),
_copyChildren(true)
{
  _eids.insert(eid1);
  _eids.insert(eid2);
}

CopyMapSubsetOp::CopyMapSubsetOp(const ConstOsmMapPtr& from, const ElementCriterionPtr& crit) :
_from(from),
_crit(crit),
_copyChildren(true)
{
  std::shared_ptr<UniqueElementIdVisitor> getIdVis(new UniqueElementIdVisitor());
  FilteredVisitor idVis(_crit, getIdVis);
  _from->visitRo(idVis);
  _eids = getIdVis->getElementSet();
  LOG_VART(_eids.size());
  LOG_VART(_eids);
}

void CopyMapSubsetOp::apply(OsmMapPtr& map)
{
  map->setProjection(_from->getProjection());
  LOG_VARD(_copyChildren);
  AddAllVisitor v(_from, map, _copyChildren);

  LOG_VARD(_eids.size());
  for (set<ElementId>::const_iterator it = _eids.begin(); it != _eids.end(); ++it)
  {
    if (_from->containsElement(*it) == false)
    {
      throw IllegalArgumentException("Unable to find element: " + hoot::toString(*it));
    }
    _from->getElement(*it)->visitRo(*_from, v, _copyChildren);
  }
  std::set<ElementId> eids = v.getElementsAdded();
  LOG_VARD(eids.size());
  _eidsCopied.insert(eids.begin(), eids.end());
}

}
