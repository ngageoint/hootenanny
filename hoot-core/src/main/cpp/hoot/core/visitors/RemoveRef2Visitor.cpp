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
 * @copyright Copyright (C) 2015-2023 Maxar (http://www.maxar.com/)
 */
#include "RemoveRef2Visitor.h"

// hoot
#include <hoot/core/elements/ConstOsmMapConsumer.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/visitors/ConstElementVisitor.h>

namespace hoot
{

int RemoveRef2Visitor::logWarnCount = 0;

HOOT_FACTORY_REGISTER(ElementVisitor, RemoveRef2Visitor)

QStringList RemoveRef2Visitor::_ref2Keys;
std::mutex RemoveRef2Visitor::_mutex;

/**
 * Traverses the OsmMap and creates a map from uuid tags to ElementIds.
 */
class Ref1ToEidVisitor : public ConstElementVisitor
{
public:

  Ref1ToEidVisitor() = default;
  ~Ref1ToEidVisitor() override = default;

  const RemoveRef2Visitor::Ref1ToEid& getRef1ToEid() const { return _ref1ToEid; }

  void visit(const ConstElementPtr& e) override
  {
    if (e->getTags().contains(MetadataTags::Ref1()))
    {
      QString ref1 = e->getTags().get(MetadataTags::Ref1());
      _ref1ToEid[ref1] = e->getElementId();
    }
  }

  QString getDescription() const override { return ""; }
  QString getName() const override { return ""; }
  QString getClassName() const override { return ""; }

private:

  RemoveRef2Visitor::Ref1ToEid _ref1ToEid;
};

RemoveRef2Visitor::RemoveRef2Visitor()
{
  // make sure we're re-entrant.
  std::lock_guard<std::mutex> ml(_mutex);
  if (_ref2Keys.empty())
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
    throw IllegalArgumentException("Expected only a single criterion in RemoveRef2Visitor.");

  _criterion = e;
}

void RemoveRef2Visitor::_checkAndDeleteRef2(ElementPtr e, QString key)
{
  // get the associated REF1 element
  QStringList refs = e->getTags().getList(key);
  QStringList refsCopy = refs;

  for (const auto& r : qAsConst(refsCopy))
  {
    // if it isn't a valid ref, carry on.
    if (r == "todo" || r == "none" || r.isEmpty())
      continue;

    ElementId eid = _ref1ToEid[r];

    if (eid.isNull())
    {
      const QString errMsg = QString("Found a %1 that references a non-existing %2: %3").arg(MetadataTags::Ref2(), MetadataTags::Ref1(), r);
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
      if (refs.empty() && key == MetadataTags::Ref2())
        refs.append("none");
    }
    else
    {
      ElementPtr element = _map->getElement(eid);
      // if the REF1 element meets the criterion.
      if (ref1CriterionSatisfied(element))
      {
        // remove the specified REF2 from the appropriate REF2 field.
        refs.removeAll(r);
        if (refs.empty() && key == MetadataTags::Ref2())
          refs.append("none");
      }
    }
  }

  if (!refs.empty())
    e->getTags().setList(key, refs);
  else
    e->getTags().remove(key);
}

bool RemoveRef2Visitor::_hasRef2Tag(ElementPtr e) const
{
  for (const auto& key : qAsConst(_ref2Keys))
  {
    if (e->getTags().contains(key))
    {
      QString v = e->getTags().get(key);
      if (!v.isEmpty() && v != "none")
        return true;
    }
  }
  return false;
}

void RemoveRef2Visitor::setOsmMap(OsmMap* map)
{
  OsmMapConsumerBase::setOsmMap(map);
  // traverse the map and create a REF1 to ElementId map.
  Ref1ToEidVisitor v;
  _map->visitRo(v);
  _ref1ToEid = v.getRef1ToEid();
}

void RemoveRef2Visitor::visit(const ElementPtr& e)
{
  if (!_criterion)
    throw IllegalArgumentException("You must specify a criterion before calling RemoveRef2Visitor.");

  // if e has a REF2 and meets the criterion
  if (_hasRef2Tag(e) && ref2CriterionSatisfied(e))
  {
    // go through each REF2 and evaluate for deletion
    for (const auto& key : qAsConst(_ref2Keys))
      _checkAndDeleteRef2(e, key);

    _numProcessed++;
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
