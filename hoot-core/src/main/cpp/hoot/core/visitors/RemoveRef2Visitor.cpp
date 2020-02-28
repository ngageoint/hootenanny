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
#include "RemoveRef2Visitor.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/ConstOsmMapConsumer.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/elements/ConstElementVisitor.h>

namespace hoot
{

int RemoveRef2Visitor::logWarnCount = 0;

HOOT_FACTORY_REGISTER(ElementVisitor, RemoveRef2Visitor)

QStringList RemoveRef2Visitor::_ref2Keys;
QMutex RemoveRef2Visitor::_mutex;

/**
 * Traverses the OsmMap and creates a map from uuid tags to ElementIds.
 */
class Ref1ToEidVisitor : public ConstElementVisitor
{
public:

  Ref1ToEidVisitor() {}

  virtual ~Ref1ToEidVisitor() {}

  const RemoveRef2Visitor::Ref1ToEid& getRef1ToEid() const { return _ref1ToEid; }

  virtual void visit(const ConstElementPtr& e)
  {
    if (e->getTags().contains(MetadataTags::Ref1()))
    {
      QString ref1 = e->getTags().get(MetadataTags::Ref1());
      _ref1ToEid[ref1] = e->getElementId();
    }
  }

  virtual QString getDescription() const { return ""; }
  virtual std::string getClassName() const { return ""; }

private:

  RemoveRef2Visitor::Ref1ToEid _ref1ToEid;
};

RemoveRef2Visitor::RemoveRef2Visitor() :
_errorOnMissingRef1(false)
{
  // make sure we're re-entrant.
  QMutexLocker ml(&_mutex);

  if (_ref2Keys.size() == 0)
  {
    _ref2Keys << MetadataTags::Ref2();
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
      const QString errMsg =
        "Found a " + MetadataTags::Ref2() + " that references a non-existing " +
         MetadataTags::Ref1() + ": " + r;
      if (logWarnCount < Log::getWarnMessageLimit())
      {
        LOG_WARN(errMsg);
      }
      else if (logWarnCount == Log::getWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
      refs.removeAll(r);
      if (refs.size() == 0 && key == MetadataTags::Ref2())
      {
        refs.append("none");
      }
    }
    else
    {
      ElementPtr e = _map->getElement(eid);
      // if the REF1 element meets the criterion.
      if (ref1CriterionSatisfied(e))
      {
        // remove the specified REF2 from the appropriate REF2 field.
        refs.removeAll(r);
        if (refs.size() == 0 && key == MetadataTags::Ref2())
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

void RemoveRef2Visitor::visit(const ElementPtr& e)
{
  if (!_criterion)
  {
    throw IllegalArgumentException("You must specify a criterion before calling "
                                   "RemoveRef2Visitor.");
  }

  // if e has a REF2 and meets the criterion
  if (_hasRef2Tag(e) && ref2CriterionSatisfied(e))
  {
    // go through each REF2 and evaluate for deletion
    for (int i = 0; i < _ref2Keys.size(); i++)
    {
      _checkAndDeleteRef2(e, _ref2Keys[i]);
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
