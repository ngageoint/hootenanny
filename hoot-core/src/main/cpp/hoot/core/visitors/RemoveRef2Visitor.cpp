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
#include "RemoveRef2Visitor.h"

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/ConstOsmMapConsumer.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, RemoveRef2Visitor)

QStringList RemoveRef2Visitor::_ref2Keys;
QMutex RemoveRef2Visitor::_mutex;

/**
 * Traverses the OsmMap and creates a map from uuid tags to ElementIds.
 */
class Ref1ToEidVisitor : public ElementVisitor, public ConstOsmMapConsumer
{
public:

  Ref1ToEidVisitor() {}

  virtual ~Ref1ToEidVisitor() {}

  const RemoveRef2Visitor::Ref1ToEid& getRef1ToEid() const { return _ref1ToEid; }

  virtual void setOsmMap(const OsmMap* map) { _map = map; }

  virtual void visit(const ConstElementPtr& e)
  {
    if (e->getTags().contains("REF1"))
    {
      QString ref1 = e->getTags().get("REF1");
      _ref1ToEid[ref1] = e->getElementId();
    }
  }

private:
  const OsmMap* _map;
  RemoveRef2Visitor::Ref1ToEid _ref1ToEid;
};

RemoveRef2Visitor::RemoveRef2Visitor() :
_errorOnMissingRef1(false)
{
  // make sure we're re-entrant.
  QMutexLocker ml(&_mutex);

  if (_ref2Keys.size() == 0)
  {
    _ref2Keys << "REF2";
    _ref2Keys << "REVIEW";
    _ref2Keys << "CONFLICT";
    _ref2Keys << "DIVIDED1";
    _ref2Keys << "DIVIDED2";
  }
}

void RemoveRef2Visitor::addCriterion(const ElementCriterionPtr& e)
{
  if (_criterion)
  {
    throw IllegalArgumentException("Expected only a single criterion in RemoveRef2Visitor.");
  }
  _criterion = e;
}

void RemoveRef2Visitor::_checkAndDeleteRef2(ElementPtr e, QString key)
{
  // get the associated REF1 element
  QStringList refs = e->getTags().getList(key);
  QStringList refsCopy = refs;

  for (int i = 0; i < refsCopy.size(); i++)
  {
    QString r = refsCopy[i];

    // if it isn't a valid ref, carry on.
    if (r == "todo" || r == "none" || r.isEmpty())
    {
      continue;
    }

    ElementId eid = _ref1ToEid[r];

    if (eid.isNull())
    {
      const QString errMsg = "Found a REF2 that references a non-existing REF1: " + r;
      //TODO: make _errorOnMissingRef1 configurable from nodejs - see #1175
      //if (_errorOnMissingRef1)
      //{
        //throw IllegalArgumentException(errMsg);
      //}
      //else
      //{
        LOG_WARN(errMsg);
        refs.removeAll(r);
        if (refs.size() == 0 && key == "REF2")
        {
          refs.append("none");
        }
      //}
    }
    else
    {
      ElementPtr e = _map->getElement(eid);
      // if the REF1 element meets the criterion.
      if (ref1CriterionSatisfied(e))
      {
        // remove the specified REF2 from the appropriate REF2 field.
        refs.removeAll(r);
        if (refs.size() == 0 && key == "REF2")
        {
          refs.append("none");
        }
      }
    }
  }

  if (refs.size() > 0)
  {
    e->getTags().setList(key, refs);
  }
  else
  {
    e->getTags().remove(key);
  }
}

bool RemoveRef2Visitor::_hasRef2Tag(ElementPtr e) const
{
  for (int i = 0; i < _ref2Keys.size(); i++)
  {
    if (e->getTags().contains(_ref2Keys[i]))
    {
      QString v = e->getTags().get(_ref2Keys[i]);
      if (!v.isEmpty() && v != "none")
      {
        return true;
      }
    }
  }

  return false;
}

void RemoveRef2Visitor::setOsmMap(OsmMap* map)
{
  _map = map;

  // traverse the map and create a REF1 to ElementId map.
  Ref1ToEidVisitor v;
  _map->visitRo(v);
  _ref1ToEid = v.getRef1ToEid();
}

void RemoveRef2Visitor::visit(const ConstElementPtr& e)
{
  if (!_criterion)
  {
    throw IllegalArgumentException("You must specify a criterion before calling "
                                   "RemoveRef2Visitor.");
  }
  ElementType type = e->getElementType();
  long id = e->getId();
  ElementPtr ee = _map->getElement(ElementId(type, id));

  // if e has a REF2 and meets the criterion
  if (_hasRef2Tag(ee) && ref2CriterionSatisfied(ee))
  {
    // go through each REF2 and evaluate for deletion
    for (int i = 0; i < _ref2Keys.size(); i++)
    {
      _checkAndDeleteRef2(ee, _ref2Keys[i]);
    }
  }
}

bool RemoveRef2Visitor::ref1CriterionSatisfied(const ConstElementPtr& e) const
{
  return _criterion->isSatisfied(e);
}

bool RemoveRef2Visitor::ref2CriterionSatisfied(const ConstElementPtr& e) const
{
  return ref1CriterionSatisfied(e);
}

}
