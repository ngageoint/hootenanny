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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "MatchFactory.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Boundable.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/criterion/TagAdvancedCriterion.h>

//Qt
#include <QString>

using namespace geos::geom;
using namespace std;

namespace hoot
{

std::shared_ptr<MatchFactory> MatchFactory::_theInstance;

MatchFactory::~MatchFactory()
{
}

MatchFactory::MatchFactory()
{
  setConfiguration(conf());
}

MatchPtr MatchFactory::createMatch(const ConstOsmMapPtr& map, ElementId eid1, ElementId eid2) const
{
  LOG_VART(eid1);
  LOG_VART(eid2);
  LOG_VART(_creators.size());

  for (size_t i = 0; i < _creators.size(); ++i)
  {
    const QString name = _creators[i]->getName();
    LOG_VART(name);
    MatchPtr m = _creators[i]->createMatch(map, eid1, eid2);
    if (m)
    {
      return m;
    }
  }

  return 0;
}

void MatchFactory::createMatches(const ConstOsmMapPtr& map, std::vector<ConstMatchPtr>& matches,
  const Envelope& bounds, std::shared_ptr<const MatchThreshold> threshold) const
{
  for (size_t i = 0; i < _creators.size(); ++i)
  { 
    std::shared_ptr<MatchCreator> matchCreator = _creators[i];
    LOG_STATUS("Launching matcher: " << i + 1 << " / " << _creators.size() << "...");
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

void MatchFactory::_checkMatchCreatorBoundable(const std::shared_ptr<MatchCreator>& matchCreator,
                                               const Envelope& bounds) const
{
  if (bounds.isNull() == false)
  {
    std::shared_ptr<Boundable> boundable = std::dynamic_pointer_cast<Boundable>(matchCreator);
    if (boundable == 0)
    {
      throw HootException("One or more match creators is not boundable and cannot be used with "
                          "boundable match operations.");
    }
    boundable->setBounds(bounds);
  }
}

vector<CreatorDescription> MatchFactory::getAllAvailableCreators() const
{
  vector<CreatorDescription> result;

  // get all match creators from the factory
  vector<std::string> names =
    Factory::getInstance().getObjectNamesByBase(MatchCreator::className());

  for (size_t i = 0; i < names.size(); i++)
  {
    // get all names known by this creator.
    std::shared_ptr<MatchCreator> mc(
      Factory::getInstance().constructObject<MatchCreator>(names[i]));

    vector<CreatorDescription> d = mc->getAllCreators();
    result.insert(result.end(), d.begin(), d.end());
  }

  return result;
}

void MatchFactory::registerCreator(const QString& c)
{
  QStringList args = c.split(",");
  QString className = args[0];
  LOG_VART(className);
  if (className.length() > 0)
  {
    args.removeFirst();
    std::shared_ptr<MatchCreator> mc(
      Factory::getInstance().constructObject<MatchCreator>(className));

    if (!_tagFilter.trimmed().isEmpty())
    {
      // We're specifically checking for an option to feed this tag criterion.  Additional combined
      // criteria can be added to this match creator if needed.
      std::shared_ptr<TagAdvancedCriterion> filter(new TagAdvancedCriterion(_tagFilter));
      mc->setCriterion(filter);
    }

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
  QStringList matchCreators = ConfigOptions().getMatchCreators();
  QStringList mergerCreators = ConfigOptions().getMergerCreators();
  LOG_VART(matchCreators);
  LOG_VART(mergerCreators);

  if ((matchCreators.size() == 0 || mergerCreators.size() == 0))
  {
    LOG_WARN("Match or merger creators empty.  Setting to defaults.");
    matchCreators = ConfigOptions::getMatchCreatorsDefaultValue();
    mergerCreators = ConfigOptions::getMergerCreatorsDefaultValue();
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
        fixedMergerCreators.append("hoot::HighwayMergerCreator");
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
    LOG_TRACE("Temp fixing merger.creators...");
    conf().set("merger.creators", fixedMergerCreators.join(";"));
  }
  LOG_VART(mergerCreators);

  //fix way subline matcher options - https://github.com/ngageoint/hootenanny-ui/issues/970
  if (matchCreators.contains("hoot::NetworkMatchCreator") &&
      ConfigOptions().getWaySublineMatcher() != "hoot::FrechetSublineMatcher" &&
      ConfigOptions().getWaySublineMatcher() != "hoot::MaximalSublineMatcher")
  {
    LOG_TRACE("Temp fixing way.subline.matcher...");
    conf().set("way.subline.matcher", "hoot::MaximalSublineMatcher");
  }
  else if (matchCreators.contains("hoot::HighwayMatchCreator") &&
           ConfigOptions().getWaySublineMatcher() != "hoot::FrechetSublineMatcher" &&
           ConfigOptions().getWaySublineMatcher() != "hoot::MaximalNearestSublineMatcher")
  {
    LOG_TRACE("Temp fixing way.subline.matcher...");
    conf().set("way.subline.matcher", "hoot::MaximalNearestSublineMatcher");
  }
  LOG_VART(ConfigOptions().getWaySublineMatcher());

  //fix highway classifier - https://github.com/ngageoint/hootenanny-ui/issues/971
  if (matchCreators.contains("hoot::NetworkMatchCreator") &&
      ConfigOptions().getConflateMatchHighwayClassifier() != "hoot::HighwayExpertClassifier")
  {
    LOG_TRACE("Temp fixing conflate.match.highway.classifier...");
    conf().set("conflate.match.highway.classifier", "hoot::HighwayExpertClassifier");
  }
  else if (matchCreators.contains("hoot::HighwayMatchCreator") &&
           ConfigOptions().getConflateMatchHighwayClassifier() != "hoot::HighwayRfClassifier")
  {
    LOG_TRACE("Temp fixing conflate.match.highway.classifier...");
    conf().set("conflate.match.highway.classifier", "hoot::HighwayRfClassifier");
  }
  LOG_VART(ConfigOptions().getConflateMatchHighwayClassifier());
}

void MatchFactory::setConfiguration(const Settings& s)
{
  _tagFilter = ConfigOptions(s).getConflateTagFilter();
}

MatchFactory& MatchFactory::getInstance()
{
  /* TODO: remove this hack after the following UI issues are fixed:
   *
   * https://github.com/ngageoint/hootenanny-ui/issues/969
   * https://github.com/ngageoint/hootenanny-ui/issues/970
   * https://github.com/ngageoint/hootenanny-ui/issues/971
   * https://github.com/ngageoint/hootenanny-ui/issues/972
   *
   * UPDATE: 8/21/19 - Believe the above issues should all have been fixed in v2 version of the UI.
   * So, maybe replace these fixes with error checking and throw exceptions when bad inputs come in.
   * */
  if (ConfigOptions().getAutocorrectOptions())
  {
    MatchFactory::_tempFixDefaults();
  }

  const QStringList matchCreators = ConfigOptions().getMatchCreators();
  const QStringList mergerCreators = ConfigOptions().getMergerCreators();
  LOG_VART(matchCreators);
  LOG_VART(mergerCreators);

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

void MatchFactory::reset()
{
  _creators.clear();
  _tagFilter = "";
}

QString MatchFactory::getCreatorsStr() const
{
  QString str;
  for (size_t i = 0; i < _creators.size(); ++i)
  {
    const QString name = _creators[i]->getName();
    str += name + ";";
  }
  str.chop(1);
  return str;
}


}
