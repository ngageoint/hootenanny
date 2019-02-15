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
#include "NetworkMatchCreator.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/conflate/matching/MatchType.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/criterion/StatusCriterion.h>
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/NotImplementedException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/conflate/network/DebugNetworkMapCreator.h>
#include <hoot/core/conflate/network/NetworkMatch.h>
#include <hoot/core/conflate/network/OsmNetworkExtractor.h>
#include <hoot/core/conflate/network/NetworkMatcher.h>

// Standard
#include <fstream>

// tgs
#include <tgs/RandomForest/RandomForest.h>

using namespace std;
using namespace Tgs;

namespace hoot
{

HOOT_FACTORY_REGISTER(MatchCreator, NetworkMatchCreator)

NetworkMatchCreator::NetworkMatchCreator()
{
  _userCriterion.reset(new HighwayCriterion());
}

Match* NetworkMatchCreator::createMatch(const ConstOsmMapPtr& /*map*/, ElementId /*eid1*/,
  ElementId /*eid2*/)
{
  Match* result = 0;

  return result;
}

const Match* NetworkMatchCreator::_createMatch(const NetworkDetailsPtr& map, NetworkEdgeScorePtr e,
  ConstMatchThresholdPtr mt)
{
  return new NetworkMatch(map, e->getEdgeMatch(), e->getScore(), mt);
}

void NetworkMatchCreator::createMatches(const ConstOsmMapPtr& map, vector<const Match*>& matches,
  ConstMatchThresholdPtr threshold)
{
  LOG_DEBUG("Creating matches with: " << className() << "...");
  LOG_VARD(threshold);

  // use another class to extract graph nodes and graph edges.
  OsmNetworkExtractor e1;
  boost::shared_ptr<ChainCriterion> c1(new ChainCriterion(
                                         ElementCriterionPtr(new StatusCriterion(Status::Unknown1)),
                                         _userCriterion));
  if (_filter)
  {
    c1->addCriterion(_filter);
  }
  e1.setCriterion(c1);
  OsmNetworkPtr n1 = e1.extractNetwork(map);
  LOG_TRACE("Extracted Network 1: " << n1->toString());

  OsmNetworkExtractor e2;
  boost::shared_ptr<ChainCriterion> c2(new ChainCriterion(
                                         ElementCriterionPtr(new StatusCriterion(Status::Unknown2)),
                                         _userCriterion));
  if (_filter)
  {
    c2->addCriterion(_filter);
  }
  e2.setCriterion(c2);
  OsmNetworkPtr n2 = e2.extractNetwork(map);
  LOG_TRACE("Extracted Network 2: " << n2->toString());

  LOG_INFO("Matching networks...");
  // call class to derive final graph node and graph edge matches
  NetworkMatcherPtr matcher(
    Factory::getInstance().constructObject<NetworkMatcher>(ConfigOptions().getNetworkMatcher()));
  matcher->matchNetworks(map, n1, n2);

  NetworkDetailsPtr details(new NetworkDetails(map, n1, n2));

  const size_t numIterations = ConfigOptions().getNetworkOptimizationIterations();
  if (numIterations < 1)
  {
    throw HootException(
      "Invalid value: " + QString::number(numIterations) + " for setting " +
      ConfigOptions::getNetworkOptimizationIterationsKey());
  }
  LOG_INFO("Optimizing network over " << numIterations << " iterations...");
  for (size_t i = 0; i < numIterations; ++i)
  {
    matcher->iterate();
    LOG_INFO("Optimization iteration: " << i + 1 << "/" << numIterations << " complete.");

    if (ConfigOptions().getNetworkMatchWriteDebugMaps())
    {
      OsmMapPtr copy(new OsmMap(map));
      DebugNetworkMapCreator(matcher->getMatchThreshold()).addDebugElements(copy,
        matcher->getAllEdgeScores(), matcher->getAllVertexScores());

      MapProjector::projectToWgs84(copy);
      conf().set(ConfigOptions::getWriterIncludeDebugTagsKey(), true);
      // TODO: use config opt for debug file path
      QString name = QString("tmp/debug-%1.osm").arg(i, 3, 10, QLatin1Char('0'));
      LOG_INFO("Writing debug map: " << name);
      OsmMapWriterFactory::write(copy, name);
    }
  }

  // Finalize
  matcher->finalize();

  // Write final debug map
  if (ConfigOptions().getNetworkMatchWriteDebugMaps())
  {
    OsmMapPtr copy(new OsmMap(map));
    DebugNetworkMapCreator(matcher->getMatchThreshold()).addDebugElements(copy,
      matcher->getAllEdgeScores(), matcher->getAllVertexScores());

    MapProjector::projectToWgs84(copy);
    conf().set(ConfigOptions::getWriterIncludeDebugTagsKey(), true);
    QString name = QString("tmp/debug-final.osm");
    LOG_INFO("Writing debug map: " << name);
    OsmMapWriterFactory::write(copy, name);
  }

  LOG_DEBUG("Retrieving edge scores...");
  // Convert graph edge matches into NetworkMatch objects.
  QList<NetworkEdgeScorePtr> edgeMatch = matcher->getAllEdgeScores();

  LOG_VART(matcher->getMatchThreshold());
  for (int i = 0; i < edgeMatch.size(); i++)
  {
    LOG_VART(edgeMatch[i]->getUid());
    LOG_VART(edgeMatch[i]->getScore());
    LOG_VART(edgeMatch[i]->getEdgeMatch());

    if (edgeMatch[i]->getScore() > matcher->getMatchThreshold())
    {
      LOG_VART(edgeMatch[i]->getEdgeMatch()->getUid());
      matches.push_back(_createMatch(details, edgeMatch[i], threshold));
    }
  }

  LOG_INFO("Found " << matches.size() << " highway match candidates.");
}

vector<CreatorDescription> NetworkMatchCreator::getAllCreators() const
{
  vector<CreatorDescription> result;
  result.push_back(
    CreatorDescription(
      className(), "Matches roads with the network algorithm",
      CreatorDescription::BaseFeatureType::Highway, false));
  return result;
}

bool NetworkMatchCreator::isMatchCandidate(ConstElementPtr element, const ConstOsmMapPtr& /*map*/)
{
  if (_filter && !_filter->isSatisfied(element))
  {
    return false;
  }
  return _userCriterion->isSatisfied(element);
}

boost::shared_ptr<MatchThreshold> NetworkMatchCreator::getMatchThreshold()
{
  if (!_matchThreshold.get())
  {
    ConfigOptions config;
    _matchThreshold.reset(
      new MatchThreshold(config.getNetworkMatchThreshold(), config.getNetworkMissThreshold(),
                         config.getNetworkReviewThreshold()));
  }
  return _matchThreshold;
}

}
