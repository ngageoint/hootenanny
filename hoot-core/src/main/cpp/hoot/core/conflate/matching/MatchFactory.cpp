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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "MatchFactory.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/ops/Boundable.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/util/Log.h>

//Qt
#include <QString>

using namespace geos::geom;
using namespace std;

namespace hoot
{

boost::shared_ptr<MatchFactory> MatchFactory::_theInstance;

MatchFactory::~MatchFactory()
{
}

MatchFactory::MatchFactory()
{
}

Match* MatchFactory::createMatch(const ConstOsmMapPtr& map, ElementId eid1, ElementId eid2) const
{
  for (size_t i = 0; i < _creators.size(); ++i)
  {
    Match* m = _creators[i]->createMatch(map, eid1, eid2);

    if (m)
    {
      return m;
    }
  }

  return 0;
}

void MatchFactory::createMatches(const ConstOsmMapPtr& map, vector<const Match*>& matches,
  const Envelope& bounds, boost::shared_ptr<const MatchThreshold> threshold) const
{
  for (size_t i = 0; i < _creators.size(); ++i)
  {
    boost::shared_ptr<MatchCreator> matchCreator = _creators[i];
    _checkMatchCreatorBoundable(matchCreator, bounds);
    if (threshold.get())
    {
      matchCreator->createMatches(map, matches, threshold);
    }
    else
    {
      matchCreator->createMatches(map, matches, matchCreator->getMatchThreshold());
    }
  }
}

void MatchFactory::_checkMatchCreatorBoundable(boost::shared_ptr<MatchCreator> matchCreator,
                                               const Envelope& bounds) const
{
  if (bounds.isNull() == false)
  {
    boost::shared_ptr<Boundable> boundable = boost::dynamic_pointer_cast<Boundable>(matchCreator);
    if (boundable == 0)
    {
      throw HootException("One or more match creators is not boundable and cannot be used with "
                          "boundable match operations.");
    }
    boundable->setBounds(bounds);
  }
}

vector<MatchCreator::Description> MatchFactory::getAllAvailableCreators() const
{
  vector<MatchCreator::Description> result;

  // get all match creators from the factory
  vector<std::string> names =
    Factory::getInstance().getObjectNamesByBase(MatchCreator::className());

  for (size_t i = 0; i < names.size(); i++)
  {
    // get all names known by this creator.
    boost::shared_ptr<MatchCreator> mc(Factory::getInstance().constructObject<MatchCreator>(names[i]));

    vector<MatchCreator::Description> d = mc->getAllCreators();
    result.insert(result.end(), d.begin(), d.end());
  }

  return result;
}

void MatchFactory::registerCreator(QString c)
{
  QStringList args = c.split(",");
  QString className = args[0];
  LOG_VART(className);
  if (className.length() > 0)
  {
    args.removeFirst();
    boost::shared_ptr<MatchCreator> mc(Factory::getInstance().constructObject<MatchCreator>(className));
    _theInstance->registerCreator(mc);

    if (args.size() > 0)
    {
      mc->setArguments(args);
    }
  }
}

void MatchFactory::_setMatchCreators(QStringList matchCreatorsList)
{
  LOG_DEBUG("MatchFactory creators: " << matchCreatorsList);

  for (int i = 0; i < matchCreatorsList.size(); i++)
  {
    _theInstance->registerCreator(matchCreatorsList[i]);
  }
}

void MatchFactory::_tempFixDefaults()
{
  QStringList matchCreators = ConfigOptions().getMatchCreators().split(";");
  QStringList mergerCreators = ConfigOptions().getMergerCreators().split(";");
  LOG_VARD(matchCreators);
  LOG_VARD(mergerCreators);

  if ((matchCreators.size() == 0 || mergerCreators.size() == 0) &&
      !ConfigOptions().getConflateEnableOldRoads())
  {
    throw HootException(
      "Empty match/merger creators only allowed when conflate.enable.old.roads is enabled.");
  }

  //fix matchers/mergers - https://github.com/ngageoint/hootenanny-ui/issues/972
  if (matchCreators.size() != mergerCreators.size())
  {
    //going to make the mergers match whatever the matchers are
    QStringList fixedMergerCreators;
    for (int i = 0; i < matchCreators.size(); i++)
    {
      const QString matchCreator = matchCreators.at(i);
      if (matchCreator == "hoot::BuildingMatchCreator")
      {
        fixedMergerCreators.append("hoot::BuildingMergerCreator");
      }
      else if (matchCreator.contains("hoot::ScriptMatchCreator"))
      {
        fixedMergerCreators.append("hoot::ScriptMergerCreator");
      }
      else if (matchCreator == "hoot::HighwayMatchCreator")
      {
        fixedMergerCreators.append("hoot::HighwaySnapMergerCreator");
      }
      else if (matchCreator == "hoot::NetworkMatchCreator")
      {
        fixedMergerCreators.append("hoot::NetworkMergerCreator");
      }
      else if (matchCreator == "hoot::PoiPolygonMatchCreator")
      {
        fixedMergerCreators.append("hoot::PoiPolygonMergerCreator");
      }
    }
    LOG_DEBUG("Temp fixing merger.creators...");
    conf().set("merger.creators", fixedMergerCreators.join(";"));
  }
  LOG_VARD(mergerCreators);

  //fix way subline matcher options - https://github.com/ngageoint/hootenanny-ui/issues/970
  if (matchCreators.contains("hoot::NetworkMatchCreator") &&
      ConfigOptions().getWaySublineMatcher() != "hoot::FrechetSublineMatcher" &&
      ConfigOptions().getWaySublineMatcher() != "hoot::MaximalSublineMatcher")
  {
    LOG_DEBUG("Temp fixing way.subline.matcher...");
    conf().set("way.subline.matcher", "hoot::MaximalSublineMatcher");
  }
  else if (matchCreators.contains("hoot::HighwayMatchCreator") &&
           ConfigOptions().getWaySublineMatcher() != "hoot::FrechetSublineMatcher" &&
           ConfigOptions().getWaySublineMatcher() != "hoot::MaximalNearestSublineMatcher")
  {
    LOG_DEBUG("Temp fixing way.subline.matcher...");
    conf().set("way.subline.matcher", "hoot::MaximalNearestSublineMatcher");
  }
  LOG_VARD(ConfigOptions().getWaySublineMatcher());

  //fix highway classifier - https://github.com/ngageoint/hootenanny-ui/issues/971
  if (matchCreators.contains("hoot::NetworkMatchCreator") &&
      ConfigOptions().getConflateMatchHighwayClassifier() != "hoot::HighwayExpertClassifier")
  {
    LOG_DEBUG("Temp fixing conflate.match.highway.classifier...");
    conf().set("conflate.match.highway.classifier", "hoot::HighwayExpertClassifier");
  }
  else if (matchCreators.contains("hoot::HighwayMatchCreator") &&
           ConfigOptions().getConflateMatchHighwayClassifier() != "hoot::HighwayRfClassifier")
  {
    LOG_DEBUG("Temp fixing conflate.match.highway.classifier...");
    conf().set("conflate.match.highway.classifier", "hoot::HighwayRfClassifier ");
  }
  LOG_VARD(ConfigOptions().getConflateMatchHighwayClassifier());

  //fix use of rubber sheeting and corner splitter - default value coming in from UI with network
  //will be correct, so just fix for unifying - https://github.com/ngageoint/hootenanny-ui/issues/969
  QStringList mapCleanerTransforms = ConfigOptions().getMapCleanerTransforms();
  if (matchCreators.contains("hoot::HighwayMatchCreator") &&
      (mapCleanerTransforms.contains("hoot::CornerSplitter") ||
       mapCleanerTransforms.contains("hoot::RubberSheet")))
  {
    if (mapCleanerTransforms.contains("hoot::CornerSplitter"))
    {
      mapCleanerTransforms.removeAll("hoot::CornerSplitter");
    }
    if (mapCleanerTransforms.contains("hoot::RubberSheet"))
    {
      mapCleanerTransforms.removeAll("hoot::RubberSheet");
    }
    LOG_DEBUG("Temp fixing map.cleaner.transforms...");
    conf().set("map.cleaner.transforms", mapCleanerTransforms.join(";"));
  }
  LOG_VARD(ConfigOptions().getMapCleanerTransforms());
}

MatchFactory& MatchFactory::getInstance()
{
  /* TODO: remove this hack after the following UI issues are fixed:
   *
   * https://github.com/ngageoint/hootenanny-ui/issues/969
   * https://github.com/ngageoint/hootenanny-ui/issues/970
   * https://github.com/ngageoint/hootenanny-ui/issues/971
   * https://github.com/ngageoint/hootenanny-ui/issues/972
   * */
  if (ConfigOptions().getAutocorrectOptions())
  {
    MatchFactory::_tempFixDefaults();
  }

  const QStringList matchCreators = ConfigOptions().getMatchCreators().split(";");
  const QStringList mergerCreators = ConfigOptions().getMergerCreators().split(";");
  LOG_VARD(matchCreators);
  LOG_VARD(mergerCreators);

  //ConflateAverageTest is configured with old roads and specifies empty strings for both matchers
  //and mergers.  I don't completely understand why it explicitly needs to specify an empty config
  //strings for those, though.  The old roads option will be removed in #2133.
  if ((matchCreators.size() == 0 || mergerCreators.size() == 0) &&
      !ConfigOptions().getConflateEnableOldRoads())
  {
    throw HootException(
      "Empty match/merger creators only allowed when conflate.enable.old.roads is enabled.");
  }

  if (matchCreators.size() != mergerCreators.size())
  {
    throw HootException(
      "The number of configured match creators (" + QString::number(matchCreators.size()) +
      ") does not equal the number of configured merger creators (" +
      QString::number(mergerCreators.size()) + ")");
  }

  for (int i = 0; i < matchCreators.size(); i++)
  {
    const QString matchCreator = matchCreators.at(i);
    const QString mergerCreator = mergerCreators.at(i);
    //Currently, there is only one kind of ScriptMergerCreator, so this check is useful for finding
    //misuses of the generic conflation engine.  If we add any more script merger creators, we'll
    //need a better check.
    if (matchCreator.startsWith("hoot::ScriptMatchCreator") &&
        mergerCreator != "hoot::ScriptMergerCreator")
    {
      throw HootException(
        "Attempted to use a ScriptMatchCreator without a ScriptMergerCreator.  Match creator: " +
        matchCreator + QString(" Merger creator: ")  + mergerCreator);
    }
  }

  if (!_theInstance.get())
  {
    _theInstance.reset(new MatchFactory());
  }

  if (_theInstance->_creators.size() == 0)
  {
    //only get the match creators that are specified in the config
    _setMatchCreators(matchCreators);
  }
  return *_theInstance;
}

}
