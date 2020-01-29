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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ReportMissingElementsVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Relation.h>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, ReportMissingElementsVisitor)

ReportMissingElementsVisitor::ReportMissingElementsVisitor(const bool removeMissing,
                                                           const Log::WarningLevel& logLevel,
                                                           const int maxReport) :
_logLevel(logLevel),
_maxReport(maxReport),
_missingCount(0),
_removeMissing(removeMissing)
{
}

void ReportMissingElementsVisitor::setConfiguration(const Settings& conf)
{
  setMaxReport(ConfigOptions(conf).getLogWarnMessageLimit());
}

void ReportMissingElementsVisitor::_reportMissing(ElementId referer, ElementId missing)
{
  QString msg;
  if (_removeMissing)
  {
    msg = "Removing missing " + missing.toString() + " in " + referer.toString() + ".";
  }
  else
  {
    msg = "Missing " + missing.toString() + " in " + referer.toString() + ".";
  }

  if (_missingCount < _maxReport)
  {
    LOG_LEVEL(_logLevel, msg);
  }
  else
  {
    LOG_TRACE(msg);
  }
  _missingCount++;
  if (_missingCount == _maxReport)
  {
    LOG_LEVEL(
      _logLevel,
      "Reached maximum number of missing element reports (" << _maxReport <<
      "). No longer reporting.");
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
    const ConstWayPtr& w = _map->getWay(id);
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
    const ConstRelationPtr& r = _map->getRelation(id);
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
    const WayPtr& w = _map->getWay(id);
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
      LOG_TRACE("Way nodes size before: " << w->getNodeCount());
      w->setNodes(newNids);
      LOG_TRACE("Way nodes size after: " << w->getNodeCount());
    } 
  }
  else if (type == ElementType::Relation)
  {
    const RelationPtr& r = _map->getRelation(id);
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
      LOG_TRACE("Relation members size before: " << r->getMembers().size());
      r->setMembers(newEntries);
      LOG_TRACE("Relation members size after: " << r->getMembers().size());
    }
  }
}

}
