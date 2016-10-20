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
#include "ReportMissingElementsVisitor.h"

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/elements/Relation.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, ReportMissingElementsVisitor)

ReportMissingElementsVisitor::ReportMissingElementsVisitor(bool removeMissing, int maxReport)
{
  _removeMissing = removeMissing;
  _maxReport = maxReport;
  _missingCount = 0;
}

void ReportMissingElementsVisitor::_reportMissing(ElementId referer, ElementId missing)
{
  if (_missingCount < _maxReport)
  {
    if (_removeMissing)
    {
      LOG_WARN("Removing missing " << missing.toString() << " in " <<
               referer.toString() << ".");
    }
    else
    {
      LOG_WARN("Missing " << missing.toString() << " in " <<
               referer.toString() << ".");
    }
  }
  _missingCount++;
  if (_missingCount == _maxReport)
  {
    LOG_WARN("Reached maximum number of missing reports. No longer reporting.");
  }
}

void ReportMissingElementsVisitor::visit(const ConstElementPtr& e)
{
  if (_removeMissing)
  {
    _visitRw(e->getElementType(), e->getId());
  }
  else
  {
    _visitRo(e->getElementType(), e->getId());
  }
}

void ReportMissingElementsVisitor::_visitRo(ElementType type, long id)
{
  if (type == ElementType::Way)
  {
    const shared_ptr<const Way>& w = _map->getWay(id);
    for (size_t i = 0; i < w->getNodeCount(); i++)
    {
      if (_map->containsNode(w->getNodeIds()[i]) == false)
      {
        _reportMissing(ElementId(type, id), ElementId::node(w->getNodeIds()[i]));
      }
    }
  }
  else if (type == ElementType::Relation)
  {
    const shared_ptr<const Relation>& r = _map->getRelation(id);
    for (size_t i = 0; i < r->getMembers().size(); i++)
    {
      const RelationData::Entry& e = r->getMembers()[i];
      if (_map->containsElement(e.getElementId()) == false)
      {
        _reportMissing(ElementId(type, id), e.getElementId());
      }
    }
  }
}

void ReportMissingElementsVisitor::_visitRw(ElementType type, long id)
{
  if (type == ElementType::Way)
  {
    const shared_ptr<Way>& w = _map->getWay(id);
    vector<long> newNids;
    newNids.reserve(w->getNodeCount());
    for (size_t i = 0; i < w->getNodeCount(); i++)
    {
      if (_map->containsNode(w->getNodeIds()[i]) == false)
      {
        _reportMissing(ElementId(type, id), ElementId::node(w->getNodeIds()[i]));
      }
      else
      {
        newNids.push_back(w->getNodeIds()[i]);
      }
    }
    if (newNids.size() != w->getNodeCount())
    {
      w->setNodes(newNids);
    }
  }
  else if (type == ElementType::Relation)
  {
    const shared_ptr<Relation>& r = _map->getRelation(id);
    vector<RelationData::Entry> newEntries;
    newEntries.reserve(r->getMembers().size());
    for (size_t i = 0; i < r->getMembers().size(); i++)
    {
      const RelationData::Entry& e = r->getMembers()[i];
      if (_map->containsElement(e.getElementId()) == false)
      {
        _reportMissing(ElementId(type, id), e.getElementId());
      }
      else
      {
        newEntries.push_back(e);
      }
    }
    if (newEntries.size() != r->getMembers().size())
    {
      r->setMembers(newEntries);
    }
  }
}

}
