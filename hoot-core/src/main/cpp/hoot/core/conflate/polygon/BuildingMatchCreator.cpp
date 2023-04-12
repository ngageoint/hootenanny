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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "BuildingMatchCreator.h"

// hoot
//#include <hoot/core/algorithms/extractors/OverlapExtractor.h>
//#include <hoot/core/conflate/matching/MatchThreshold.h>
//#include <hoot/core/conflate/matching/MatchType.h>
#include <hoot/core/conflate/matching/MatchCreator.h>
#include <hoot/core/conflate/polygon/BuildingMatch.h>
#include <hoot/core/conflate/polygon/BuildingRfClassifier.h>
//#include <hoot/core/criterion/ArbitraryCriterion.h>
//#include <hoot/core/criterion/BuildingCriterion.h>
//#include <hoot/core/elements/OsmMap.h>
//#include <hoot/core/schema/OsmSchema.h>
//#include <hoot/core/util/CollectionUtils.h>
//#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/Factory.h>
//#include <hoot/core/util/MemoryUsageChecker.h>
//#include <hoot/core/util/Settings.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/visitors/BuildingMatchVisitor.h>
//#include <hoot/core/visitors/ConstElementVisitor.h>
//#include <hoot/core/visitors/SpatialIndexer.h>

// Standard
//#include <fstream>
//#include <functional>

// tgs
//#include <tgs/RandomForest/RandomForest.h>
//#include <tgs/RStarTree/IntersectionIterator.h>
//#include <tgs/RStarTree/MemoryPageStore.h>

// Qt
#include <QElapsedTimer>

using namespace geos::geom;
using namespace std;
using namespace Tgs;

namespace hoot
{

HOOT_FACTORY_REGISTER(MatchCreator, BuildingMatchCreator)

MatchPtr BuildingMatchCreator::createMatch(const ConstOsmMapPtr& map, ElementId eid1, ElementId eid2)
{
  std::shared_ptr<BuildingMatch> result;
  if (eid1.getType() != ElementType::Node && eid2.getType() != ElementType::Node)
  {
    ConstElementPtr e1 = map->getElement(eid1);
    ConstElementPtr e2 = map->getElement(eid2);

    // score each candidate and push it on the result vector
    if (BuildingMatchVisitor::isRelated(e1, e2))
      result = std::make_shared<BuildingMatch>(map, _getRf(), eid1, eid2, getMatchThreshold());
  }
  return result;
}

void BuildingMatchCreator::createMatches(const ConstOsmMapPtr& map, std::vector<ConstMatchPtr>& matches, ConstMatchThresholdPtr threshold)
{
  QElapsedTimer timer;
  timer.start();

  // The parent does some initialization we need.
  MatchCreator::createMatches(map, matches, threshold);

  QString searchRadiusStr;
  const double searchRadius = ConfigOptions().getSearchRadiusBuilding();
  if (searchRadius < 0)
    searchRadiusStr = "within a feature dependent search radius";
  else
    searchRadiusStr = "within a search radius of " + QString::number(searchRadius, 'g', 2) + " meters";
  LOG_INFO("Looking for matches with: " << className() << " " << searchRadiusStr << "...");
  LOG_VARD(*threshold);
  const int matchesSizeBefore = static_cast<int>(matches.size());

  BuildingMatchVisitor v(map, matches, _getRf(), threshold, _filter, Status::Unknown1);
  map->visitWaysRo(v);
  map->visitRelationsRo(v);
  const int matchesSizeAfter = static_cast<int>(matches.size());

  LOG_STATUS(
    "\tFound " << StringUtils::formatLargeNumber(v.getNumMatchCandidatesFound()) <<
    " building match candidates and " <<
    StringUtils::formatLargeNumber(matchesSizeAfter - matchesSizeBefore) <<
    " total matches in: " << StringUtils::millisecondsToDhms(timer.elapsed()) << ".");
}

std::vector<CreatorDescription> BuildingMatchCreator::getAllCreators() const
{
  std::vector<CreatorDescription> result;
  result.emplace_back(className(), "Generates matchers that match buildings", CreatorDescription::Building, false);
  return result;
}

std::shared_ptr<BuildingRfClassifier> BuildingMatchCreator::loadBuildingClassifier()
{
  QString path = ConfPath::search(ConfigOptions().getConflateMatchBuildingModel());
  LOG_DEBUG("Loading model from: " << path);

  QFile file(path.toLatin1().data());
  if (!file.open(QIODevice::ReadOnly))
    throw HootException("Error opening file: " + path);

  QDomDocument doc("");
  if (!doc.setContent(&file))
  {
    file.close();
    throw HootException("Error opening file: " + path);
  }
  file.close();

  std::shared_ptr<BuildingRfClassifier> rf = std::make_shared<BuildingRfClassifier>();
  QDomElement docRoot = doc.elementsByTagName("RandomForest").at(0).toElement();
  rf->importTree(docRoot);

  return rf;
}

std::shared_ptr<BuildingRfClassifier> BuildingMatchCreator::_getRf()
{
  if (!_rf)
    _rf = BuildingMatchCreator::loadBuildingClassifier();

  return _rf;
}

bool BuildingMatchCreator::isMatchCandidate(ConstElementPtr element, const ConstOsmMapPtr& map)
{
  std::vector<ConstMatchPtr> matches;
  return BuildingMatchVisitor(map, matches, _filter).isMatchCandidate(element);
}

std::shared_ptr<MatchThreshold> BuildingMatchCreator::getMatchThreshold()
{
  if (!_matchThreshold.get())
  {
    LOG_VART(ConfigOptions().getBuildingMatchThreshold());
    _matchThreshold = std::make_shared<MatchThreshold>(ConfigOptions().getBuildingMatchThreshold(),
                                                       ConfigOptions().getBuildingMissThreshold(),
                                                       ConfigOptions().getBuildingReviewThreshold());
  }
  return _matchThreshold;
}

QStringList BuildingMatchCreator::getCriteria() const
{
  return QStringList(BuildingCriterion::className());
}

}
