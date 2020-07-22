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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "PoiPolygonMerger.h"

// hoot
#include <hoot/core/conflate/polygon/BuildingMerger.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/criterion/poi-polygon/PoiPolygonPoiCriterion.h>
#include <hoot/core/criterion/poi-polygon/PoiPolygonPolyCriterion.h>
#include <hoot/core/visitors/UniqueElementIdVisitor.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/StatusUpdateVisitor.h>
#include <hoot/core/schema/PreserveTypesTagMerger.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonMatch.h>
#include <hoot/core/io/OsmMapWriterFactory.h>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(Merger, PoiPolygonMerger)

int PoiPolygonMerger::logWarnCount = 0;

PoiPolygonMerger::PoiPolygonMerger() :
MergerBase(),
_autoMergeManyPoiToOnePolyMatches(ConfigOptions().getPoiPolygonAutoMergeManyPoiToOnePolyMatches()),
_tagMergerClass("")
{
}

PoiPolygonMerger::PoiPolygonMerger(const set<pair<ElementId, ElementId>>& pairs) :
_pairs(pairs),
_autoMergeManyPoiToOnePolyMatches(ConfigOptions().getPoiPolygonAutoMergeManyPoiToOnePolyMatches()),
_tagMergerClass("")
{
  assert(_pairs.size() >= 1);
}

std::shared_ptr<TagMerger> PoiPolygonMerger::_getTagMerger()
{
  if (!_tagMerger)
  {
    LOG_VART(ConfigOptions().getHighwayMergeTagsOnly());
    LOG_VART(_autoMergeManyPoiToOnePolyMatches);
    LOG_VART(_tagMergerClass);
    LOG_VART(ConfigOptions().getPoiPolygonTagMerger());
    LOG_VART(ConfigOptions().getTagMergerDefault());

    std::string tagMergerClass;
    // We force this setting always preserve types when merging many POIs in. It works with
    // Attribute Conflation as well, via tag.merger.overwrite.exclude.
    if (_autoMergeManyPoiToOnePolyMatches)
    {
      tagMergerClass = PreserveTypesTagMerger::className();
    }
    // Otherwise, allow for calling class to specify the tag merger outside of Configurable.
    else if (!_tagMergerClass.trimmed().isEmpty())
    {
      tagMergerClass = _tagMergerClass.toStdString();
    }
    // Otherwise, let's see if the tag merger was set specifically for poi/poly.
    else if (!ConfigOptions().getPoiPolygonTagMerger().trimmed().isEmpty())
    {
      tagMergerClass = ConfigOptions().getPoiPolygonTagMerger().trimmed().toStdString();
    }
    // Otherwise, let's try the default configured tag merger.
    else if (!ConfigOptions().getTagMergerDefault().trimmed().isEmpty())
    {
      tagMergerClass = ConfigOptions().getTagMergerDefault().trimmed().toStdString();
    }
    else
    {
      throw IllegalArgumentException("No tag merger specified for POI/Polygon conflation.");
    }
    LOG_VART(tagMergerClass);

    _tagMerger.reset(Factory::getInstance().constructObject<TagMerger>(tagMergerClass));

    std::shared_ptr<Configurable> critConfig = std::dynamic_pointer_cast<Configurable>(_tagMerger);
    if (critConfig.get())
    {
      critConfig->setConfiguration(conf());
    }
  }
  LOG_VART(_tagMerger->getClassName());
  return _tagMerger;
}

void PoiPolygonMerger::apply(const OsmMapPtr& map, vector<pair<ElementId, ElementId>>& replaced)
{
  /// See
  /// https://github.com/ngageoint/hootenanny/files/607197/Hootenanny.-.POI.to.Polygon.2016-11-15.pptx
  /// for more details.

  // Merge all POI tags first, but keep Unknown1 and Unknown2 separate. It is implicitly assumed
  // that since they're in a single group they all represent the same entity.
  Tags poiTags1 = _mergePoiTags(map, Status::Unknown1);
  // This debug map writing is very expensive, so just turn it on when debugging small datasets.
  //OsmMapWriterFactory::writeDebugMap(map, "PoiPolygonMerger-after-poi-tags-merge-1");
  Tags poiTags2 = _mergePoiTags(map, Status::Unknown2);
  //OsmMapWriterFactory::writeDebugMap(map, "PoiPolygonMerger-after-poi-tags-merge-2");

  // Get all the building parts for each status
  vector<ElementId> buildings1 = _getBuildingParts(map, Status::Unknown1);
  vector<ElementId> buildings2 = _getBuildingParts(map, Status::Unknown2);

  // Merge all the building parts together into a single building entity using the typical building
  // merge process.
  ElementId finalBuildingEid = _mergeBuildings(map, buildings1, buildings2, replaced);
  LOG_VART(finalBuildingEid);
  //OsmMapWriterFactory::writeDebugMap(map, "PoiPolygonMerger-after-building-merge");

  ElementPtr finalBuilding = map->getElement(finalBuildingEid);
  if (!finalBuilding.get())
  {
    // Building merger must not have been able to merge...maybe need an earlier check for this
    // and also handle it differently...

    if (logWarnCount < Log::getWarnMessageLimit())
    {
      LOG_WARN("Building merger unable to merge.");
      LOG_VART(buildings1);
      LOG_VART(buildings2);
      LOG_VART(replaced);
    }
    else if (logWarnCount == Log::getWarnMessageLimit())
    {
      LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
    }
    logWarnCount++;
    return;
  }
  assert(finalBuilding.get());

  // merge the tags

  // We're always keeping the building geometry, but the tags kept depends on the source status
  // of the features or the conflation workflow chosen.
  Tags finalBuildingTags = finalBuilding->getTags();
  LOG_VART(finalBuildingTags);
 // Tags poiBuildingMergedTags;
  if (poiTags1.size() > 0)
  {
    // If this is a ref POI, we'll keep its tags and replace the building tags.
    LOG_TRACE("Merging POI tags with building tags for POI status Unknown1...");
    finalBuildingTags =
      _getTagMerger()->mergeTags(poiTags1, finalBuildingTags, finalBuilding->getElementType());
    LOG_VART(finalBuildingTags);
    //OsmMapWriterFactory::writeDebugMap(map, "PoiPolygonMerger-after-building-tags-merge-1");
  }
  if (poiTags2.size() > 0)
  {
    LOG_TRACE("Merging POI tags with building tags for POI status Unknown2...");
    // If this is a sec POI, we'll keep the buildings tags and replace its tags.
    finalBuildingTags =
      _getTagMerger()->mergeTags(finalBuildingTags, poiTags2, finalBuilding->getElementType());
    LOG_VART(finalBuildingTags);
    //OsmMapWriterFactory::writeDebugMap(map, "PoiPolygonMerger-after-building-tags-merge-2");
  }

  // Do some book keeping to remove the POIs and mark them as replaced.
  long poisMerged = 0;
  for (set<pair<ElementId, ElementId>>::const_iterator it = _pairs.begin(); it != _pairs.end();
       ++it)
  {
    const pair<ElementId, ElementId>& p = *it;
    if (p.first.getType() == ElementType::Node)
    {
      replaced.push_back(pair<ElementId, ElementId>(p.first, finalBuildingEid));
      // clear the tags just in case it is part of a way
      if (map->containsElement(p.first))
      {
        map->getElement(p.first)->getTags().clear();
        RecursiveElementRemover(p.first).apply(map);
      }
      poisMerged++;
    }

    if (p.second.getType() == ElementType::Node)
    {
      replaced.push_back(pair<ElementId, ElementId>(p.second, finalBuildingEid));
      // clear the tags just in case it is part of a way
      if (map->containsElement(p.second))
      {
        map->getElement(p.second)->getTags().clear();
        RecursiveElementRemover(p.second).apply(map);
      }
      poisMerged++;
    }
  }
  LOG_VART(poisMerged);
  OsmMapWriterFactory::writeDebugMap(map, "PoiPolygonMerger-after-poi-removal");

  if (poisMerged > 0)
  {
    // If the poly merge target was previously conflated, let's add to the total number of POIs that
    // have been merged.
    if (finalBuildingTags.contains(MetadataTags::HootPoiPolygonPoisMerged()))
    {
      poisMerged += finalBuildingTags[MetadataTags::HootPoiPolygonPoisMerged()].toLong();
    }
    LOG_VART(poisMerged);
    finalBuildingTags.set(MetadataTags::HootPoiPolygonPoisMerged(), QString::number(poisMerged));
    finalBuilding->setStatus(Status::Conflated);
    ConfigOptions conf;
    if (conf.getWriterIncludeDebugTags() && conf.getWriterIncludeMatchedByTag())
    {
      finalBuilding->setTag(MetadataTags::HootMatchedBy(), PoiPolygonMatch::MATCH_NAME);
    }
  }

  finalBuilding->setTags(finalBuildingTags);
  LOG_VART(finalBuilding);
}

Tags PoiPolygonMerger::_mergePoiTags(const OsmMapPtr& map, Status s)
{
  LOG_TRACE("Merging POI tags for status: " << s << "...");

  Tags result;

  LOG_VART(_autoMergeManyPoiToOnePolyMatches);
  for (set<pair<ElementId, ElementId>>::const_iterator it = _pairs.begin(); it != _pairs.end();
       ++it)
  {
    const pair<ElementId, ElementId>& p = *it;
    LOG_VART(p);
    ElementPtr e1 = map->getElement(p.first);
    ElementPtr e2 = map->getElement(p.second);
    if (e1->getStatus() == s && e1->getElementType() == ElementType::Node)
    {
      LOG_VART(e1->getElementId());
      //LOG_VART(e1);
      result = _getTagMerger()->mergeTags(result, e1->getTags(), e1->getElementType());
    }
    if (e2->getStatus() == s && e2->getElementType() == ElementType::Node)
    {
      LOG_VART(e2->getElementId());
      //LOG_VART(e2);
      result = _getTagMerger()->mergeTags(result, e2->getTags(), e2->getElementType());
    }
  }

  LOG_TRACE("Merged POI tags: " << result);
  return result;
}

vector<ElementId> PoiPolygonMerger::_getBuildingParts(const OsmMapPtr& map, Status s) const
{
  LOG_TRACE("Getting building parts for status: " << s << "...");

  vector<ElementId> result;
  for (set<pair<ElementId, ElementId>>::const_iterator it = _pairs.begin(); it != _pairs.end();
       ++it)
  {
    const pair<ElementId, ElementId>& p = *it;
    ElementPtr e1 = map->getElement(p.first);
    ElementPtr e2 = map->getElement(p.second);
    if (e1->getStatus() == s && e1->getElementType() != ElementType::Node)
    {
      result.push_back(e1->getElementId());
    }
    if (e2->getStatus() == s && e2->getElementType() != ElementType::Node)
    {
      result.push_back(e2->getElementId());
    }
  }

  LOG_TRACE("Building part IDs: " << result);
  return result;
}

ElementId PoiPolygonMerger::_mergeBuildings(const OsmMapPtr& map,
  vector<ElementId>& buildings1, vector<ElementId>& buildings2,
  vector<pair<ElementId, ElementId>>& replaced)
{
  LOG_TRACE("Merging buildings...");

  LOG_VART(buildings1.size());
  LOG_VART(buildings2.size());
  LOG_VART(replaced.size());

  set<pair<ElementId, ElementId>> pairs;

  assert(buildings1.size() != 0 || buildings2.size() != 0);
  // If there is only one set of buildings, then there is no need to merge. Group all the building
  // parts into a single building.
  if (buildings1.size() == 0)
  {
    set<ElementId> eids;
    eids.insert(buildings2.begin(), buildings2.end());
    LOG_VART(eids.size());
    return BuildingMerger::buildBuilding(map, eids)->getElementId();
  }
  else if (buildings2.size() == 0)
  {
    set<ElementId> eids;
    eids.insert(buildings1.begin(), buildings1.end());
    LOG_VART(eids.size());
    return BuildingMerger::buildBuilding(map, eids)->getElementId();
  }

  // the exact pairing doesn't matter for the building merger, it just breaks it back out into
  // two groups
  for (size_t i = 0; i < max(buildings1.size(), buildings2.size()); i++)
  {
    size_t i1 = min(i, buildings1.size() - 1);
    size_t i2 = min(i, buildings2.size() - 1);

    pair<ElementId, ElementId> p(buildings1[i1], buildings2[i2]);
    pairs.insert(p);
  }

  BuildingMerger(pairs).apply(map, replaced);

  set<ElementId> newElement;
  for (size_t i = 0; i < replaced.size(); i++)
  {
    newElement.insert(replaced[i].second);
  }

  return *newElement.begin();
}

ElementId PoiPolygonMerger::_getElementIdByType(OsmMapPtr map, const ElementCriterion& typeCrit)
{
  UniqueElementIdVisitor idSetVis;
  FilteredVisitor filteredVis(typeCrit, idSetVis);
  map->visitRo(filteredVis);
  const std::set<ElementId>& ids = idSetVis.getElementSet();
  if (ids.size() != 1)
  {
    throw IllegalArgumentException(
      "Exactly one POI and one polygon should be passed to POI/Polygon merging.");
  }
  return *ids.begin();
}

void PoiPolygonMerger::_fixStatuses(OsmMapPtr map, const ElementId& poiId, const ElementId& polyId)
{
  // If the features involved in the merging don't have statuses, let's arbitrarily set them to
  // Unknown1, since the building merging code used by poi/poly requires it.  Setting them
  // artificially *shouldn't* have a negative effect on the poi/poly merging, though. Below we are
  // failing when seeing a conflated status...we could rework to update that status as well.

  // If the poly has an invalid status, we'll give it an Unknown1 status instead.
  PoiPolygonPoiCriterion poiPolyPoi;
  StatusUpdateVisitor status1Vis(Status::Unknown1, true);
  FilteredVisitor filteredVis1(poiPolyPoi, status1Vis);
  map->visitRw(filteredVis1);

  // If the POI has an invalid status, we'll give it an Unknown2 status instead.
  PoiPolygonPolyCriterion poiPolyPoly;
  StatusUpdateVisitor status2Vis(Status::Unknown2, true);
  FilteredVisitor filteredVis2(poiPolyPoly, status2Vis);
  map->visitRw(filteredVis2);

  // The BuildingMerger (used by both poi/poly and building merging) assumes that all input elements
  // will either have an Unknown1 or Unknown2 status. If we are merging an element that was
  // previously merged and given a conflated status, we need to replace that status with the
  // appropriate unconflated status, if we can determine it. This logic could possibly be combined
  // with the invalid status related logic above.
  ElementPtr poi = map->getElement(poiId);
  LOG_VART(poi);
  ElementPtr poly = map->getElement(polyId);
  LOG_VART(poly);
  // pois always get merged into polys
  LOG_VART(poi->getStatus());
  LOG_VART(poly->getStatus());
  if (poi->getStatus() == Status::Conflated)
  {
    // don't think this should ever occur
    throw IllegalArgumentException(
      "POI being merged with polygon cannot must have an Unknown1 or Unknown2 status.");
  }
  else if (poly->getStatus() == Status::Conflated)
  {
    // we've already handled invalid statuses above
    if (poi->getStatus() == Status::Unknown1)
    {
      poly->setStatus(Status::Unknown2);
    }
    else if (poi->getStatus() == Status::Unknown2)
    {
      poly->setStatus(Status::Unknown1);
    }
  }
}

ElementId PoiPolygonMerger::mergeOnePoiAndOnePolygon(OsmMapPtr map)
{
  // Trying to merge more than one POI into the polygon has proven problematic due to the building
  // merging logic. Merging more than one POI isn't a requirement, so only supporting 1:1 merging
  // at this time.

  LOG_INFO("Merging one POI and one polygon...");

  // determine which element is which
  const ElementId poiId = _getElementIdByType(map, PoiPolygonPoiCriterion());
  LOG_VART(poiId);
  const ElementId polyId = _getElementIdByType(map, PoiPolygonPolyCriterion());
  LOG_VART(polyId);

  // handle invalid and conflated statuses
  _fixStatuses(map, poiId, polyId);

  // do the merging
  std::set<std::pair<ElementId, ElementId>> pairs;
  // Ordering doesn't matter here, since the poi is always merged into the poly.
  pairs.insert(std::pair<ElementId, ElementId>(polyId, poiId));
  PoiPolygonMerger merger(pairs);
  std::vector<std::pair<ElementId, ElementId>> replacedElements;
  merger.apply(map, replacedElements);

  LOG_INFO("Merged POI: " << poiId << " into polygon: " << polyId);
  LOG_DEBUG("Merged feature: " << map->getElement(polyId));

  return polyId;
}

QString PoiPolygonMerger::toString() const
{
  return QString("PoiPolygonMerger %1").arg(hoot::toString(_getPairs()));
}

}
