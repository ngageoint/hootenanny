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
  // obviously this isn't extensible, but we'll just have to deal with that for now...the list may
  // never change, though
  QStringList genericMatchScripts;
  genericMatchScripts.append("Point.js");
  genericMatchScripts.append("Line.js");
  genericMatchScripts.append("Polygon.js");
  genericMatchScripts.append("PointPolygon.js");
  return StringUtils::containsSubstrings(QStringList(matcher), genericMatchScripts);
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
  LOG_DEBUG("Fixing miscellaneous...");

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

  // We could also do something here even when the sizes are equal but not doing that yet, since
  // the issue should be fixed in the UI soon.
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
  LOG_VART(matchCreators);
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

void OptionsValidator::fixGenericMatcherOrdering()
{
  LOG_DEBUG("Fixing generic matcher ordering...");

  QStringList matchCreators = ConfigOptions().getMatchCreators();
  QStringList mergerCreators = ConfigOptions().getMergerCreators();
  LOG_VART(matchCreators);
  LOG_VART(mergerCreators);
  assert(matchCreators.size() == mergerCreators.size());

  // fix matchers/mergers - https://github.com/ngageoint/hootenanny-ui/issues/972,
  // https://github.com/ngageoint/hootenanny-ui/issues/1764

  // At this time we always want the generic matchers at the end, just before Relation.js,
  // if they're present. So, don't even check to see if they're in the right order...just move them
  // anyway. There overall order will be maintained, but they'll just be towards the end. We'll
  // assume the corresponding merger for each is in the correct order and move it as well.
  if (_containsGenericMatcher(matchCreators))
  {
    QStringList fixedMatchCreators = matchCreators;
    QStringList fixedMergerCreators = mergerCreators;
    LOG_VART(fixedMatchCreators);
    LOG_VART(fixedMergerCreators);
    for (int i = 0; i < matchCreators.size(); i++)
    {
      const QString matchCreator = matchCreators.at(i);
      LOG_VART(matchCreator);
      if (_isGenericMatcher(matchCreator))
      {
        LOG_VART(i);
        const int fixedCreatorIndex = fixedMatchCreators.indexOf(matchCreator);
        LOG_VART(fixedCreatorIndex);
        fixedMatchCreators.move(fixedCreatorIndex, fixedMatchCreators.size() - 1);
        fixedMergerCreators.move(fixedCreatorIndex, fixedMergerCreators.size() - 1);
        LOG_VART(fixedMatchCreators);
        LOG_VART(fixedMergerCreators);
      }  
    }
    conf().set("match.creators", fixedMatchCreators.join(";"));
    conf().set("merger.creators", fixedMergerCreators.join(";"));
    matchCreators = ConfigOptions().getMatchCreators();
    mergerCreators = ConfigOptions().getMergerCreators();  
  }
  LOG_VART(matchCreators);
  LOG_VART(mergerCreators);

  // now move Relation.js
  const QString relationScript = "Relation.js";
  if (StringUtils::containsSubstring(matchCreators, relationScript))
  {
    const int relationScriptIndex =
      StringUtils::indexOfSubstring(matchCreators, relationScript);
    LOG_VART(relationScriptIndex);
    if (relationScriptIndex < (matchCreators.size() - 1))
    {
      matchCreators.move(relationScriptIndex, matchCreators.size() - 1);
      mergerCreators.move(relationScriptIndex, mergerCreators.size() - 1);
      conf().set("match.creators", matchCreators.join(";"));
      conf().set("merger.creators", mergerCreators.join(";"));
      LOG_VART(matchCreators);
      LOG_VART(mergerCreators);
    }
  }
  LOG_VART(matchCreators);
  LOG_VART(mergerCreators);
}

void OptionsValidator::validateMatchers()
{
  LOG_DEBUG("Validating matchers...");

  const QStringList matchCreators = ConfigOptions().getMatchCreators();
  const QStringList mergerCreators = ConfigOptions().getMergerCreators();
  LOG_VART(matchCreators);
  LOG_VART(mergerCreators);

  if (matchCreators.isEmpty() || mergerCreators.isEmpty())
  {
    // This gets temp fixed in fixMisc, if needed.
    throw IllegalArgumentException("Empty matcher or merger list specified.");
  }
  else if (matchCreators.size() != mergerCreators.size())
  {
    // This gets temp fixed in fixMisc, if needed.
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
