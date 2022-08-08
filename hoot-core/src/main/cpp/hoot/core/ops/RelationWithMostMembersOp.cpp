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
 * @copyright Copyright (C) 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "RelationWithMostMembersOp.h"

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/ops/CopyMapSubsetOp.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/elements/OsmUtils.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, RelationWithMostMembersOp)

RelationWithMostMembersOp::RelationWithMostMembersOp() :
_maxNumCritSatisifed(0),
_totalRelations(0),
_totalRelationMembers(0),
_numRelationsSatisfyingCriterion(0),
_numRelationMembersSatisfyingCriterion(0)
{
}

void RelationWithMostMembersOp::setConfiguration(const Settings& conf)
{
  ConfigOptions config = ConfigOptions(conf);
  setCriterion(config.getRelationWithMostMembersOpRelationCriterion(), "relation", conf);
  setCriterion(config.getRelationWithMostMembersOpMemberCriterion(), "member", conf);
}

void RelationWithMostMembersOp::setCriterion(const QString& criterionClass, const QString& type,
                                             const Settings& conf)
{
  LOG_VARD(criterionClass);
  if (!criterionClass.trimmed().isEmpty())
  {
    ElementCriterionPtr crit =
      Factory::getInstance().constructObject<ElementCriterion>(criterionClass.trimmed());
    Configurable* c = dynamic_cast<Configurable*>(crit.get());
    if (c != nullptr)
    {
      c->setConfiguration(conf);
    }

    if (type == "relation")
    {
      _relationCriterion = crit;
    }
    else
    {
      _memberCriterion = crit;
    }
  }
}

long RelationWithMostMembersOp::getIdOfRelationWithMaxCritSatisfactions(const ConstOsmMapPtr& map,
                                                                        RelationPtr& outRelation)
{
  long idOfRelationWithMaxCritSatisfactions = 0;

  const RelationMap& relations = map->getRelations();
  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
  {
    int numTimesCritSatisfied = 0;
    const RelationPtr& relation = it->second;
    if (!_relationCriterion || _relationCriterion->isSatisfied(relation))
    {
      _numRelationsSatisfyingCriterion++;
      const std::vector<RelationData::Entry>& members = relation->getMembers();
      for (size_t i = 0; i < members.size(); i++)
      {
        ConstElementPtr member = map->getElement(members[i].getElementId());
        if (!_memberCriterion || _memberCriterion->isSatisfied(member))
        {
          numTimesCritSatisfied++;
          _numRelationMembersSatisfyingCriterion++;
        }
        _totalRelationMembers++;
      }

      if (numTimesCritSatisfied > _maxNumCritSatisifed)
      {
        _maxNumCritSatisifed = numTimesCritSatisfied;
        idOfRelationWithMaxCritSatisfactions = relation->getId();
        outRelation = relation;
      }
    }
    _totalRelations++;
  }
  LOG_VARD(_maxNumCritSatisifed);
  LOG_VARD(idOfRelationWithMaxCritSatisfactions);
  LOG_VARD(_numRelationsSatisfyingCriterion);
  LOG_VARD(_numRelationMembersSatisfyingCriterion);
  LOG_VARD(_totalRelations);
  LOG_VARD(_totalRelationMembers);

  return idOfRelationWithMaxCritSatisfactions;
}

void RelationWithMostMembersOp::_setOutput(const long relationId, OsmMapPtr& outputMap) const
{
  std::set<ElementId> elementIds;
  elementIds.insert(ElementId(ElementType::Relation, relationId));
  OsmMapPtr tmpMap = std::make_shared<OsmMap>();
  CopyMapSubsetOp mapCopier(outputMap, elementIds);
  mapCopier.apply(tmpMap);
  outputMap = tmpMap;
}

void RelationWithMostMembersOp::apply(std::shared_ptr<OsmMap>& map)
{
  _maxNumCritSatisifed = 0;
  _totalRelations = 0;
  _totalRelationMembers = 0;
  _numRelationsSatisfyingCriterion = 0;
  _numRelationMembersSatisfyingCriterion = 0;

  RelationPtr outRelation;
  // We're only interested in the relation with the most members satisfying both of the optional
  // criteria (ties thrown out).
  const long idOfRelationWithMaxCritSatisfactions =
    getIdOfRelationWithMaxCritSatisfactions(map, outRelation);
  if (idOfRelationWithMaxCritSatisfactions != 0)
  {
    LOG_TRACE(
      "RelationWithMostMembersOp: output relation\n" <<
      OsmUtils::getElementDetailString(outRelation, map));
    // output a map with just the relation found and its members
    _setOutput(idOfRelationWithMaxCritSatisfactions, map);
  }
  else
  {
    // output an empty map since we didn't find a relation that satisifes the criteria
    map = std::make_shared<OsmMap>();
  }
  LOG_VARD(map->getElementCount());
}

QString RelationWithMostMembersOp::getCompletedStatusMessage() const
{
  if (_maxNumCritSatisifed == 0)
  {
    return "Found no relations satisfying the specified criteria.";
  }
  return
    "Found a relation with " + StringUtils::formatLargeNumber(_maxNumCritSatisifed) +
    " criteria satisfactions.";
}

}
