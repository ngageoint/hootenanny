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
#include "PoiPolygonEvidenceScorer.h"

// hoot
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/conflate/AlphaShapeGenerator.h>
#include <hoot/core/schema/OsmSchema.h>

#include "PoiPolygonTypeMatch.h"
#include "PoiPolygonNameMatch.h"
#include "PoiPolygonAddressMatch.h"
#include "PoiPolygonMatchDistanceCalculator.h"

namespace hoot
{

PoiPolygonEvidenceScorer::PoiPolygonEvidenceScorer(double matchDistance, double reviewDistance,
                                                   double distance, double typeScoreThreshold,
                                                   double nameScoreThreshold,
                                                   unsigned int matchEvidenceThreshold,
                                                   shared_ptr<Geometry> poiGeom,
                                                   ConstOsmMapPtr map,
                                                   QString testUuid) :
_matchDistance(matchDistance),
_reviewDistance(reviewDistance),
_distance(distance),
_typeScoreThreshold(typeScoreThreshold),
_nameScoreThreshold(nameScoreThreshold),
_matchEvidenceThreshold(matchEvidenceThreshold),
_poiGeom(poiGeom),
_map(map),
_testUuid(testUuid)
{
}

unsigned int PoiPolygonEvidenceScorer::_getDistanceEvidence(ConstElementPtr poi,
                                                            ConstElementPtr poly)
{
  // calculate the 2 sigma for the distance between the two objects
  const double sigma1 = poi->getCircularError() / 2.0;
  const double sigma2 = poly->getCircularError() / 2.0;
  const double ce = sqrt(sigma1 * sigma1 + sigma2 * sigma2) * 2;
  const double reviewDistancePlusCe = _reviewDistance + ce;
  _closeMatch = _distance <= reviewDistancePlusCe;
  if (_testFeatureFound)
  {
    LOG_VARD(_closeMatch);
    LOG_VARD(_distance);
    LOG_VARD(_matchDistance);
    LOG_VARD(_reviewDistance);
    LOG_VARD(reviewDistancePlusCe);
    LOG_VARD(ce);
    LOG_VARD(poi->getCircularError());
    LOG_VARD(poly->getCircularError());
  }
  //close match is a requirement, regardless of the evidence count
  if (!_closeMatch)
  {
    return 0;
  }
  PoiPolygonMatchDistanceCalculator distanceCalc(_matchDistance, _reviewDistance, poly->getTags());
  _matchDistance =
    max(
      distanceCalc.getMatchDistanceForType(_t1BestKvp),
      distanceCalc.getMatchDistanceForType(_t2BestKvp));
  _reviewDistance =
    max(
      distanceCalc.getReviewDistanceForType(_t1BestKvp),
      distanceCalc.getReviewDistanceForType(_t2BestKvp));
  unsigned int evidence = _distance <= _matchDistance ? 2 : 0;

  return evidence;
}

unsigned int PoiPolygonEvidenceScorer::_getConvexPolyDistanceEvidence(ConstElementPtr poly)
{
  unsigned int evidence = 0;
  OsmMapPtr polyMap(new OsmMap());
  ElementPtr polyTemp(poly->clone());
  polyMap->addElement(polyTemp);
  //TODO: move alpha shape init values to config?
  shared_ptr<Geometry> polyAlphaShape =
      AlphaShapeGenerator(1000.0, 0.0).generateGeometry(polyMap);
  const double alphaShapeDist = polyAlphaShape->distance(_poiGeom.get());
  evidence += alphaShapeDist <= _matchDistance ? 2 : 0;
  if (_testFeatureFound)
  {
    LOG_VARD(alphaShapeDist);
    const bool withinAlphaShapeDist = alphaShapeDist <= _matchDistance;
    LOG_VARD(withinAlphaShapeDist);
  }
  return evidence;
}

unsigned int PoiPolygonEvidenceScorer::_getTypeEvidence(ConstElementPtr poi, ConstElementPtr poly)
{
  PoiPolygonTypeMatch typeScorer(_typeScoreThreshold, _testUuid);
  _typeScore = typeScorer.getTypeScore(poi, poly, _t1BestKvp, _t2BestKvp);
  if (poi->getTags().get("historic") == "monument")
  {
    //monuments can represent just about any poi type, so lowering this some to account for that
    _typeScoreThreshold = 0.3; //TODO: move to config
  }
  _typeMatch = _typeScore >= _typeScoreThreshold;
  unsigned int evidence = _typeMatch ? 1 : 0;
  if (_testFeatureFound)
  {
    LOG_VARD(_typeScore);
    LOG_VARD(_typeMatch);
    LOG_VARD(_t1BestKvp);
    LOG_VARD(_t2BestKvp);
  }
  return evidence;
}

unsigned int PoiPolygonEvidenceScorer::_getNameEvidence(ConstElementPtr poi, ConstElementPtr poly)
{
  PoiPolygonNameMatch nameScorer(_nameScoreThreshold);
  _nameScore = nameScorer.getNameScore(poi, poly);
  _nameMatch = _nameScore >= _nameScoreThreshold;
  _exactNameMatch = nameScorer.getExactNameScore(poi, poly) == 1.0;
  unsigned int evidence = _nameMatch ? 1 : 0;
  if (_testFeatureFound)
  {
    LOG_VARD(_nameMatch);
    LOG_VARD(_exactNameMatch);
    LOG_VARD(_nameScore);
  }
  return evidence;
}

unsigned int PoiPolygonEvidenceScorer::_getAddressEvidence(ConstElementPtr poi,
                                                           ConstElementPtr poly)
{
  const bool addressMatch = PoiPolygonAddressMatch(_map, _testUuid).calculateMatch(poly, poi);
  unsigned int evidence = addressMatch ? 1 : 0;
  if (_testFeatureFound)
  {
    LOG_VARD(addressMatch);
  }
  return evidence;
}

unsigned int PoiPolygonEvidenceScorer::calculateEvidence(ConstElementPtr poi, ConstElementPtr poly)
{
  _testFeatureFound =
    poly->getTags().get("uuid") == _testUuid || poi->getTags().get("uuid") == _testUuid;

  unsigned int evidence = 0;

  evidence += _getDistanceEvidence(poi, poly);

  //close match is a requirement, regardless of the evidence count
  if (!_closeMatch && !ConfigOptions().getPoiPolygonPrintMatchDistanceTruth())
  {
    //don't exit early here if printing truths, b/c we need to calculate type match for that first
    //before exiting
    return 0;
  }

  evidence += _getTypeEvidence(poi, poly);

  //second chance to exit early if printing distance truths
  if (!_closeMatch)
  {
    return 0;
  }

  evidence += _getNameEvidence(poi, poly);

  //ADDRESS

  //no point in calc'ing the address match if we already have a match from the other evidence
  //LOG_VARD(_matchEvidenceThreshold);
  if (evidence < _matchEvidenceThreshold)
  {
    evidence += _getAddressEvidence(poi, poly);
    //TODO: move to config
    if (evidence < _matchEvidenceThreshold && _distance <= 35.0 &&
        poi->getTags().get("amenity") == "school" && OsmSchema::getInstance().isBuilding(poly))
    {
      evidence += _getConvexPolyDistanceEvidence(poly);
    }
    else
    {
      LOG_DEBUG("Skipped distance matching 2.");
    }
  }
  else
  {
    LOG_DEBUG("Skipped address matching.");
  }

  if (_testFeatureFound)
  {
    LOG_VARD(evidence);
  }
  return evidence;
}

}

