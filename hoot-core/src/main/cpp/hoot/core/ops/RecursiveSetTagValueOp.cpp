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
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "RecursiveSetTagValueOp.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/criterion/NotCriterion.h>

// geos
#include <geos/geom/Geometry.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, RecursiveSetTagValueOp)

RecursiveSetTagValueOp::RecursiveSetTagValueOp()
{
  _tagger = std::make_shared<SetTagValueVisitor>();
  _tagger->setConfiguration(conf());
}

RecursiveSetTagValueOp::RecursiveSetTagValueOp(
  const QStringList& keys, const QStringList& values, ElementCriterionPtr elementCriterion,
  bool appendToExistingValue, const bool overwriteExistingTag) :
_crit(elementCriterion),
_negateCriterion(false),
_tagger(
  std::make_shared<SetTagValueVisitor>(
    keys, values, appendToExistingValue, QStringList(), overwriteExistingTag))
{
}

RecursiveSetTagValueOp::RecursiveSetTagValueOp(
  const QString& key, const QString& value, ElementCriterionPtr elementCriterion,
  bool appendToExistingValue, const bool overwriteExistingTag) :
_crit(elementCriterion),
_negateCriterion(false),
_tagger(
  std::make_shared<SetTagValueVisitor>(
    key, value, appendToExistingValue, QStringList(), overwriteExistingTag))
{
}

void RecursiveSetTagValueOp::addCriterion(const ElementCriterionPtr& e)
{
  if (!_negateCriterion)
  {
    _crit = e;
  }
  else
  {
    _crit = std::make_shared<NotCriterion>(e);
  }
}

void RecursiveSetTagValueOp::_setCriterion(const QString& criterionName)
{
  if (!criterionName.trimmed().isEmpty())
  {
    LOG_VART(criterionName);
    addCriterion(
      Factory::getInstance().constructObject<ElementCriterion>(criterionName.trimmed()));
  }
}

void RecursiveSetTagValueOp::apply(std::shared_ptr<OsmMap>& map)
{
  if (!_tagger->isValid())
  {
    throw IllegalArgumentException(SetTagValueVisitor::className() + " not configured properly.");
  }

  const RelationMap& relations = map->getRelations();
  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
  {
    RelationPtr relation = it->second;
    LOG_VART(relation->getElementId());
    if (relation && (!_crit || _crit->isSatisfied(relation)))
    {
      _tagger->visit(relation);

      for (size_t i = 0; i < relation->getMembers().size(); i++)
      {
        ElementPtr relationMember = map->getElement(relation->getMembers()[i].getElementId());
        if (relationMember)
        {
          _tagger->visit(relationMember);
        }
      }
    }
  }

  const WayMap& ways = map->getWays();
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    WayPtr way = it->second;
    LOG_VART(way->getElementId());
    if (way && (!_crit || _crit->isSatisfied(way)))
    {
      _tagger->visit(way);

      const std::vector<long>& nodeIds = way->getNodeIds();
      for (std::vector<long>::const_iterator it2 = nodeIds.begin(); it2 != nodeIds.end(); ++it2)
      {
        NodePtr wayNode = map->getNode(*it2);
        if (wayNode)
        {
          _tagger->visit(wayNode);
        }
      }
    }
  }

  const NodeMap& nodes = map->getNodes();
  for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
  {
    NodePtr node = it->second;
    LOG_VART(node->getElementId());
    if (node && (!_crit || _crit->isSatisfied(node)))
    {
      _tagger->visit(node);
    }
  }
}

}
