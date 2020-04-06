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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "OptionsValidator.h"

// hoot
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/HootException.h>

namespace hoot
{

bool OptionsValidator::_isGenericMatcher(const QString& matcher)
{
  // obviously this isn't extensible, but we'll just have to deal with that for now...probably not
  // going to change for awhile, though
  QStringList genericMatchScripts;
  genericMatchScripts.append("Line.js");
  genericMatchScripts.append("Point.js");
  genericMatchScripts.append("Polygon.js");
  genericMatchScripts.append("PointPolygon.js");
  return StringUtils::containsAny(genericMatchScripts, QStringList(matcher));
}

bool OptionsValidator::_containsGenericMatcher(const QStringList& matchers)
{
  for (int i = 0; i < matchers.size(); i++)
  {
    if (_isGenericMatcher(matchers.at(i)))
    {
      return true;
    }
  }
  return false;
}

void OptionsValidator::fixMisc()
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
    conf().set("merger.creators", fixedMergerCreators.join(";"));
  }

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

void OptionsValidator::fixGenericMatcherOrdering()
{
  // fix matchers/mergers - https://github.com/ngageoint/hootenanny-ui/issues/972,
  // https://github.com/ngageoint/hootenanny-ui/issues/1764

  // At this time we always want the generic matchers at the end, just before CollectionRelation.js,
  // if they're present. So, don't even check to see if they're in the right order...just move them
  // anyway. There overall order will be maintained, but they'll just be towards the end. We'll
  // assume the corresponding merger for each is in the correct order and move it as well.
  if (_containsGenericMatcher(matchCreators))
  {
    QStringList fixedMatchCreators = matchCreators;
    QStringList fixedMergerCreators = mergerCreators;
    for (int i = 0; i < matchCreators.size(); i++)
    {
      const QString matchCreator = matchCreators.at(i);
      if (_isGenericMatcher(matchCreator))
      {
        fixedMatchCreators.move(i, fixedMatchCreators.size() - 1);
        fixedMergerCreators.move(i, fixedMergerCreators.size() - 1);
      }
    }
    conf().set("match.creators", fixedMatchCreators.join(";"));
    conf().set("merger.creators", fixedMergerCreators.join(";"));
    matchCreators = ConfigOptions().getMatchCreators();
    mergerCreators = ConfigOptions().getMergerCreators();
  }
  // now move CollectionRelation.js
  const QString collectionRelationScript = "CollectionRelation.js";
  if (StringUtils::containsAny(matchCreators, QStringList(collectionRelationScript)))
  {
    const int collectionRelationIndex = matchCreators.indexOf(collectionRelationScript);
    matchCreators.move(collectionRelationIndex, matchCreators.size() - 1);
    mergerCreators.move(collectionRelationIndex, mergerCreators.size() - 1);
    matchCreators = ConfigOptions().getMatchCreators();
    mergerCreators = ConfigOptions().getMergerCreators();
  }

  LOG_VART(mergerCreators);
}

void OptionsValidator::validateMatchers()
{
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
    // Currently, there is only one kind of ScriptMergerCreator, so this check is useful for finding
    // misuses of the generic conflation engine.  If we add any more script merger creators, we'll
    // need a better check.
    if (matchCreator.startsWith("hoot::ScriptMatchCreator") &&
        mergerCreator != "hoot::ScriptMergerCreator")
    {
      throw HootException(
        "Attempted to use a ScriptMatchCreator without a ScriptMergerCreator. Match creator: " +
        matchCreator + QString(" Merger creator: ")  + mergerCreator);
    }
  }
}

}
