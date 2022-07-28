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
 * @copyright Copyright (C) 2022 Maxar (http://www.maxar.com/)
 */
#include "AttributeBuildingMerger.h"

// hoot
#include <hoot/core/conflate/polygon/BuildingMatch.h>
#include <hoot/core/schema/PreserveTypesTagMerger.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/util/Factory.h>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(Merger, AttributeBuildingMerger)

AttributeBuildingMerger::AttributeBuildingMerger(const set<pair<ElementId, ElementId>>& pairs)
  : MergerBase(pairs),
    _manyToManyMatch(false)
{
}

void AttributeBuildingMerger::apply(const OsmMapPtr& map, vector<pair<ElementId, ElementId>>& /*replaced*/)
{
  //  This merger doesn't change any geometry or merge anything but tags from REF2 to REF1 of the match
  set<ElementId> firstPairs;
  set<ElementId> secondPairs;
  set<ElementId> combined;
  LOG_VART(_pairs);

  // check to see if it is many-to-many
  for (const auto& p : _pairs)
  {
    firstPairs.emplace(p.first);
    secondPairs.emplace(p.second);
    combined.emplace(p.first);
    combined.emplace(p.second);
  }
  _manyToManyMatch = firstPairs.size() > 1 && secondPairs.size() > 1;

  // check to see if it is many-to-many
  for (const auto& p : _pairs)
  {
    ElementPtr ref1 = map->getElement(p.first);
    ElementPtr ref2 = map->getElement(p.second);
    if (!ref1 || !ref2)
      continue;
    //  Make sure that ref2's tags goto ref1
    if (ref1->getStatus() == Status::Unknown2 && ref2->getStatus() == Status::Unknown1)
      ref1.swap(ref2);

    ref1->setTags(_getMergedTags(ref1, ref2));
    ref1->setStatus(Status::Conflated);
    ConfigOptions conf;
    if (conf.getWriterIncludeDebugTags() && conf.getWriterIncludeMatchedByTag())
      ref1->setTag(MetadataTags::HootMatchedBy(), BuildingMatch::MATCH_NAME);
  }
}

Tags AttributeBuildingMerger::_getMergedTags(const ElementPtr& e1, const ElementPtr& e2)
{
  Tags mergedTags;
  LOG_TRACE("e1 tags before merging and after built building tag merge: " << e1->getTags());
  LOG_TRACE("e2 tags before merging and after built building tag merge: " << e2->getTags());
  if (_manyToManyMatch)  // preserve type tags
    mergedTags = PreserveTypesTagMerger().mergeTags(e1->getTags(), e2->getTags(), ElementType::Way);
  else  // use the default tag merging mechanism
    mergedTags = TagMergerFactory::getInstance().getMergerPtr(ConfigOptions().getTagMergerDefault())->mergeTags(e1->getTags(), e2->getTags(), ElementType::Way);
  LOG_TRACE("tags after merging: " << mergedTags);

  QStringList ref1;
  e1->getTags().readValues(MetadataTags::Ref1(), ref1);
  QStringList ref2;
  e2->getTags().readValues(MetadataTags::Ref2(), ref2);

  ref1.sort();
  ref2.sort();

  if (!ref1.empty() || !ref2.empty())
  {
    if (ref1 == ref2)
      mergedTags[MetadataTags::HootBuildingMatch()] = "true";
    else
      mergedTags[MetadataTags::HootBuildingMatch()] = "false";
  }

  LOG_VART(mergedTags);
  return mergedTags;
}

QString AttributeBuildingMerger::toString() const
{
  return QString("AttributeBuildingMerger %1").arg(hoot::toString(_pairs));
}

}
