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
 * @copyright Copyright (C) 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "LinearDiffMerger.h"

// hoot
#include <hoot/core/algorithms/linearreference/WaySublineRemover.h>
#include <hoot/core/conflate/highway/HighwayMatch.h>
#include <hoot/core/elements/RelationMemberUtils.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/ops/RemoveElementByEid.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(Merger, LinearDiffMerger)

LinearDiffMerger::LinearDiffMerger()
  : LinearMergerAbstract(),
    _treatReviewsAsMatches(false)
{
}

LinearDiffMerger::LinearDiffMerger(const std::set<std::pair<ElementId, ElementId>>& pairs,
                                   const std::shared_ptr<SublineStringMatcher>& sublineMatcher)
  : LinearMergerAbstract(pairs, sublineMatcher),
    _treatReviewsAsMatches(false)
{
}

void LinearDiffMerger::apply(const OsmMapPtr& map, std::vector<std::pair<ElementId, ElementId>>& replaced)
{
  LOG_TRACE("Applying LinearDiffMerger...");

  _map = map;

  // We're overriding some of LinearMergerAbstract's behavior here for diff purposes.

  // This is the same as LinearMergerAbstract, except we don't require the associated element be
  // part of the map in order to be merged (why?).
  std::vector<std::pair<ElementId, ElementId>> pairs;
  pairs.reserve(_pairs.size());
  for (const auto& id_pair : _pairs)
    pairs.emplace_back(id_pair.first, id_pair.second);

  // This has the same behavior as LinearMergerAbstract.
  _mergeShortestPairsFirst(pairs, replaced);
}

bool LinearDiffMerger::_mergePair(const ElementId& eid1, const ElementId& eid2, std::vector<std::pair<ElementId, ElementId>>& replaced)
{
  // Check for missing elements.
  ElementPtr e1 = _map->getElement(eid1);
  ElementPtr e2 = _map->getElement(eid2);
  LOG_VART(e1.get());
  LOG_VART(e2.get());
  if (!e1 || !e2)
    return false;

  bool needsReview = false;
  assert(e1->getElementType() == ElementType::Way);
  WayPtr way1 = std::dynamic_pointer_cast<Way>(e1);
  // Find the match between the ref and sec way, and remove the section from the sec way that
  // matches the ref way.
  if (e2->getElementType() == ElementType::Way)
  {
    WayPtr way2 = std::dynamic_pointer_cast<Way>(e2);
    LOG_TRACE("Processing match for way sec: " << way2->getElementId() << "...");
    bool matched = false;
    needsReview = _findAndProcessMatch(way1, way2, replaced, matched);
    LOG_VART(matched);
  }
  else
  {
    // For each relation member sec way, find the match between the ref and sec way, and remove the
    // section from the sec way that matches the ref way.
    assert(e2->getElementType() == ElementType::Relation);
    RelationPtr relation2 = std::dynamic_pointer_cast<Relation>(e2);
    LOG_VART(relation2->getMemberCount());

    const std::vector<RelationData::Entry> members = relation2->getMembers();
    for (const auto& member : members)
    {
      LOG_VART(member.getElementId().getType());
      assert(member.getElementId().getType() == ElementType::Way);
      ElementPtr memberElement = _map->getElement(member.getElementId());
      WayPtr way2 = std::dynamic_pointer_cast<Way>(memberElement);
      LOG_VART(RelationMemberUtils::getContainingRelationCount(way2->getElementId(), _map));
      LOG_TRACE("Processing match for relation member sec: " << way2->getElementId() << "...");
      bool matched = false;
      if (_findAndProcessMatch(way1, way2, replaced, matched))
        needsReview = true;
      LOG_VART(matched);
      if (matched)
        relation2->removeElement(member.getElementId());
      LOG_VART(RelationMemberUtils::getContainingRelationCount(way2->getElementId(), _map));
    }
    if (relation2->getMemberCount() == 0)
      RemoveElementByEid(relation2->getElementId()).apply(_map);
  }
  return needsReview;
}

bool LinearDiffMerger::_findAndProcessMatch(const WayPtr& way1, const WayPtr& way2,
                                            std::vector<std::pair<ElementId, ElementId>>& replaced, bool& matched)
{
  LOG_TRACE("Finding matching subline between: " << way1->getElementId() << " and " << way2->getElementId() << "...");
  matched = false;
  WaySublineMatchString match;
  try
  {
    match = _matchSubline(way1, way2);
  }
  catch (const NeedsReviewException& e)
  {
    LOG_VART(e.getWhat());
    if (!_treatReviewsAsMatches)
    {
      // See similar note in LinearSnapMerger.
      _markNeedsReview(way1, way2, e.getWhat(), HighwayMatch::getHighwayMatchName());
      return true;
    }
  }
  LOG_VART(match.isValid());
  if (!match.isValid())
    return false;
  matched = true;

  // Get the portion of the sec way that matched the ref way.
  WaySublineMatchString::MatchCollection matches = match.getMatches();
  LOG_VART(matches.size());
  WaySubline subline2 = matches.at(0).getSubline2();
  LOG_VART(subline2.getWay()->getElementId());
  LOG_VART(subline2);
  LOG_VART(subline2.getWay() == way2);
  LOG_VART(subline2.getWay()->getElementId() == way2->getElementId());

  // We don't want to remove the nodes in this way's sublines, since they may belong to other
  // portions of the way or other ways. The caller of this class will be responsible for cleaning
  // them up, or they may be handled by conflate post ops.
  const std::vector<ElementId> newWayIds = WaySublineRemover::removeSubline(way2, subline2, _map);
  LOG_VART(newWayIds.size());
  if (newWayIds.size() == 1)
  {
    // Do bookkeeping to show the new way that replaced the old sec way.
    LOG_TRACE("Replacing " << way2->getElementId() << " with " << newWayIds.at(0) << "...");
    replaced.emplace_back(way2->getElementId(), newWayIds.at(0));
  }
  else if (newWayIds.size() == 2)
  {
    // If the split resulted in multiple way, put them temporarily into a relation, which we'll
    // collapse later. Be sure to give this relation a type, or it won't get collapsed.
    RelationPtr relation =
      std::make_shared<Relation>(
        Status::Unknown2, _map->createNextRelationId(), way2->getCircularError(),
        MetadataTags::RelationMultilineString());
    relation->setTag(MetadataTags::HootMultilineString(), "yes");
    for (const auto& elementId : newWayIds)
    {
      LOG_TRACE("Adding " << elementId << " to " << relation->getElementId() << "...");
      relation->addElement("", elementId);
    }
    LOG_VART(relation->getMemberIds());
    _map->addRelation(relation);
    // Do bookkeeping to show the new relation that replaced the old sec way.
    LOG_TRACE("Replacing " << way2->getElementId() << " with " << relation->getElementId() << "...");
    replaced.emplace_back(way2->getElementId(), relation->getElementId());
  }
  if (ConfigOptions().getDebugMapsWriteDetailed())
  {
    OsmMapWriterFactory::writeDebugMap(
      _map, className(),
      "after-merge-" + way1->getElementId().toString() + "-" + way2->getElementId().toString());
  }

  return false;
}

WaySublineMatchString LinearDiffMerger::_matchSubline(ElementPtr e1, ElementPtr e2)
{
  LOG_TRACE("Matching sublines for: " << e1->getElementId() << " and " << e2->getElementId() << "...");
  // Some attempts were made to use cached subline matches pased in from LinearSnapMergerJs for
  // performance reasons, but the results were unstable. See branch 3969b.
  return _sublineMatcher->findMatch(_map, e1, e2);
}

}
