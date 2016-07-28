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
#include "PoiPolygonMatch.h"

// geos
#include <geos/geom/Geometry.h>

// hoot
#include <hoot/core/algorithms/LevenshteinDistance.h>
#include <hoot/core/algorithms/MeanWordSetDistance.h>
#include <hoot/core/conflate/polygon/extractors/NameExtractor.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/schema/TranslateStringDistance.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/conflate/MatchThreshold.h>

namespace hoot
{

QString PoiPolygonMatch::_matchName = "POI to Polygon";

PoiPolygonMatch::PoiPolygonMatch(const ConstOsmMapPtr& map, const ElementId& eid1,
                                 const ElementId& eid2, ConstMatchThresholdPtr threshold) :
Match(threshold)
{
  ConstElementPtr e1 = map->getElement(eid1);
  ConstElementPtr e2 = map->getElement(eid2);
  ConstElementPtr poi, poly;
  if (isPoiIsh(e1) && isBuildingIsh(e2))
  {
    _poiEid = eid1;
    _polyEid = eid2;
    poi = e1;
    poly = e2;
  }
  else if (isPoiIsh(e2) && isBuildingIsh(e1))
  {
    _poiEid = eid2;
    _polyEid = eid1;
    poi = e2;
    poly = e1;
  }
  else
  {
    LOG_WARN(e1->toString());
    LOG_WARN(e2->toString());
    throw IllegalArgumentException("Expected a POI & polygon, got: " + eid1.toString() + " " +
                                   eid2.toString());
  }

  shared_ptr<Geometry> gpoly = ElementConverter(map).convertToGeometry(poly);
  shared_ptr<Geometry> gpoi = ElementConverter(map).convertToGeometry(poi);

  bool typeMatch = _calculateTypeMatch(poi, poly);
  double nameScore = _calculateNameScore(poi, poly);
  bool nameMatch = nameScore >= ConfigOptions().getPoiPolygonMatchNameThreshold();
  double distance = gpoly->distance(gpoi.get());

  // calculate the 2 sigma for the distance between the two objects
  double sigma1 = e1->getCircularError() / 2.0;
  double sigma2 = e1->getCircularError() / 2.0;
  double ce = sqrt(sigma1 * sigma1 + sigma2 * sigma2) * 2;

  double reviewDistance = ConfigOptions().getPoiPolygonMatchReviewDistance() + ce;
  bool closeMatch = distance <= reviewDistance;

  int evidence = 0;
  evidence += typeMatch ? 1 : 0;
  evidence += nameMatch ? 1 : 0;
  evidence += distance <= ConfigOptions().getPoiPolygonMatchDistance() ? 2 : 0;

  if (!closeMatch)
  {
    _c.setMiss();
    LOG_DEBUG("poipoly miss");
  }
  else if (evidence >= 3)
  {
    _c.setMatch();
    LOG_DEBUG("poipoly match");
  }
  else if (evidence >= 1)
  {
    _c.setReview();
    LOG_DEBUG("poipoly review");
  }
  else
  {
    _c.setMiss();
    LOG_DEBUG("poipoly miss");
  }

  LOG_VARD(eid1);
  //LOG_VARD(e1->toString());
  LOG_VARD(eid2);
  //LOG_VARD(e2->toString());
  LOG_VARD(typeMatch);
  LOG_VARD(nameScore);
  LOG_VARD(nameMatch);
  LOG_VARD(e1->getCircularError());
  LOG_VARD(e2->getCircularError());
  LOG_VARD(distance);
  LOG_VARD(reviewDistance);
  LOG_VARD(closeMatch);
  LOG_VARD(evidence);
}

double PoiPolygonMatch::_calculateNameScore(ConstElementPtr e1, ConstElementPtr e2) const
{
  // found experimentally when doing building name comparisons
  return
    NameExtractor(
      new TranslateStringDistance(
        new MeanWordSetDistance(new LevenshteinDistance(1.45)))).extract(e1, e2);
}

bool PoiPolygonMatch::_calculateTypeMatch(ConstElementPtr e1, ConstElementPtr e2) const
{
  bool result = false;

  const Tags& t1 = e1->getTags();
  const Tags& t2 = e2->getTags();
  for (Tags::const_iterator it = t1.begin(); it != t1.end(); it++)
  {
    // if it is a use or POI category
    if ((OsmSchema::getInstance().getCategories(it.key(), it.value()) &
         (OsmSchemaCategory::building() | OsmSchemaCategory::use() | OsmSchemaCategory::poi()))
          != OsmSchemaCategory::Empty)
    {
      result = t2.get(it.key()) == it.value();
      if (result)
      {
        return result;
      }
    }
  }

  return result;
}

set< pair<ElementId, ElementId> > PoiPolygonMatch::getMatchPairs() const
{
  set< pair<ElementId, ElementId> > result;
  result.insert(pair<ElementId, ElementId>(_poiEid, _polyEid));
  return result;
}

bool PoiPolygonMatch::isBuildingIsh(ConstElementPtr e)
{
  return OsmSchema::getInstance().isArea(e->getTags(), e->getElementType()) &&
    OsmSchema::getInstance().getCategories(e->getTags()).intersects(
        OsmSchemaCategory::building() | OsmSchemaCategory::poi());
}

bool PoiPolygonMatch::isPoiIsh(ConstElementPtr e)
{
  return e->getElementType() == ElementType::Node &&
    (OsmSchema::getInstance().getCategories(e->getTags()).intersects(
        OsmSchemaCategory::building() | OsmSchemaCategory::poi()) ||
     e->getTags().getNames().size() > 0);
}

QString PoiPolygonMatch::toString() const
{
  return QString("PoiPolygonMatch %1 %2 P: %3").arg(_poiEid.toString()).
      arg(_polyEid.toString()).arg(_c.toString());
}

}

