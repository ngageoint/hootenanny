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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "OptionsValidator.h"

// hoot
#include <hoot/core/algorithms/subline-matching/FrechetSublineMatcher.h>
#include <hoot/core/algorithms/subline-matching/MaximalNearestSublineMatcher.h>
#include <hoot/core/algorithms/subline-matching/MaximalSublineMatcher.h>
#include <hoot/core/conflate/polygon/BuildingMatchCreator.h>
#include <hoot/core/conflate/polygon/BuildingMergerCreator.h>
#include <hoot/core/conflate/highway/HighwayExpertClassifier.h>
#include <hoot/core/conflate/highway/HighwayRfClassifier.h>
#include <hoot/core/conflate/highway/HighwayMatchCreator.h>
#include <hoot/core/conflate/highway/HighwayMergerCreator.h>
#include <hoot/core/conflate/network/NetworkMatchCreator.h>
#include <hoot/core/conflate/network/NetworkMergerCreator.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonMatchCreator.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonMergerCreator.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/schema/MetadataTags.h>

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
  if (matchCreators.empty() || mergerCreators.empty())
  {
    LOG_WARN("Match or merger creators empty. Setting to defaults.");
    matchCreators = ConfigOptions::getMatchCreatorsDefaultValue();
    mergerCreators = ConfigOptions::getMergerCreatorsDefaultValue();
  }
  StringUtils::removeEmptyStrings(matchCreators);
  StringUtils::removeEmptyStrings(mergerCreators);
  // Legacy namespace prefixes are still supported, but internally we ignore them.
  StringUtils::removePrefixes(MetadataTags::HootNamespacePrefix(), matchCreators);
  StringUtils::removePrefixes(MetadataTags::HootNamespacePrefix(), mergerCreators);
  // Merger creators may have duplicate ScriptMergerCreator instances, but match creators should
  // not have any duplicates.
  matchCreators.removeDuplicates();
  LOG_VART(matchCreators);
  LOG_VART(mergerCreators);

  // aligning the mergers with the matchers
  QStringList fixedMergerCreators;
  for (int i = 0; i < matchCreators.size(); i++)
  {
    QString matchCreator = matchCreators.at(i);
    if (matchCreator == BuildingMatchCreator::className())
    {
      fixedMergerCreators.append(BuildingMergerCreator::className());
    }
    else if (matchCreator.contains("ScriptMatchCreator"))
    {
      fixedMergerCreators.append("ScriptMergerCreator");
    }
    else if (matchCreator == HighwayMatchCreator::className())
    {
      fixedMergerCreators.append(HighwayMergerCreator::className());
    }
    else if (matchCreator == NetworkMatchCreator::className())
    {
      fixedMergerCreators.append(NetworkMergerCreator::className());
    }
    else if (matchCreator == PoiPolygonMatchCreator::className())
    {
      fixedMergerCreators.append(PoiPolygonMergerCreator::className());
    }
  }
  mergerCreators = fixedMergerCreators;

  assert(matchCreators.size() == mergerCreators.size());
  LOG_VART(matchCreators);
  LOG_VART(mergerCreators);
  conf().set(ConfigOptions::getMatchCreatorsKey(), matchCreators.join(";"));
  conf().set(ConfigOptions::getMergerCreatorsKey(), mergerCreators.join(";"));
  LOG_VART(ConfigOptions().getMatchCreators());
  LOG_VART(ConfigOptions().getMergerCreators());

  // fix way subline matcher options - https://github.com/ngageoint/hootenanny-ui/issues/970
  QString highwaySublineMatcher =
    ConfigOptions().getHighwaySublineMatcher().remove(MetadataTags::HootNamespacePrefix());
  if (matchCreators.contains(NetworkMatchCreator::className()) &&
      highwaySublineMatcher != FrechetSublineMatcher::className() &&
      highwaySublineMatcher != MaximalSublineMatcher::className())
  {
    conf().set(ConfigOptions::getHighwaySublineMatcherKey(), MaximalSublineMatcher::className());
  }
  else if (matchCreators.contains(HighwayMatchCreator::className()) &&
           highwaySublineMatcher != FrechetSublineMatcher::className() &&
           highwaySublineMatcher != MaximalNearestSublineMatcher::className())
  {
    conf().set(
      ConfigOptions::getHighwaySublineMatcherKey(), MaximalNearestSublineMatcher::className());
  }
  LOG_VART(ConfigOptions().getHighwaySublineMatcher());

  // fix highway classifier - https://github.com/ngageoint/hootenanny-ui/issues/971
  QString conflateMatchHighwayClassifier =
    ConfigOptions().getConflateMatchHighwayClassifier().remove(MetadataTags::HootNamespacePrefix());
  if (matchCreators.contains(NetworkMatchCreator::className()) &&
      conflateMatchHighwayClassifier != HighwayExpertClassifier::className())
  {
    conf().set(
      ConfigOptions::getConflateMatchHighwayClassifierKey(), HighwayExpertClassifier::className());
  }
  else if (matchCreators.contains(HighwayMatchCreator::className()) &&
           conflateMatchHighwayClassifier != HighwayRfClassifier::className())
  {
    conf().set(
      ConfigOptions::getConflateMatchHighwayClassifierKey(), HighwayRfClassifier::className());
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

  // At this time we always want the generic matchers at the end, just before Relation.js, if
  // they're present. So, don't even check to see if they're in the right order...just move them
  // anyway. Their overall order will be maintained, but they'll just be towards the end. We'll
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
    conf().set(ConfigOptions::getMatchCreatorsKey(), fixedMatchCreators.join(";"));
    conf().set(ConfigOptions::getMergerCreatorsKey(), fixedMergerCreators.join(";"));
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
      conf().set(ConfigOptions::getMatchCreatorsKey(), matchCreators.join(";"));
      conf().set(ConfigOptions::getMergerCreatorsKey(), mergerCreators.join(";"));
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

  QStringList matchCreators = ConfigOptions().getMatchCreators();
  QStringList mergerCreators = ConfigOptions().getMergerCreators();
  LOG_VART(matchCreators);
  LOG_VART(mergerCreators);
  if (matchCreators.isEmpty() || mergerCreators.isEmpty())
  {
    // This gets fixed in fixMisc, if needed.
    throw IllegalArgumentException("Empty matcher or merger list specified.");
  }
  else if (matchCreators.size() != mergerCreators.size())
  {
    // This gets fixed in fixMisc, if needed.
    throw HootException(
      "The number of configured match creators (" + QString::number(matchCreators.size()) +
      ") does not equal the number of configured merger creators (" +
      QString::number(mergerCreators.size()) + "); match creators: " + matchCreators.join(";") +
      ", merger creators: " + mergerCreators.join(";"));
  }

  for (int i = 0; i < matchCreators.size(); i++)
  {
    const QString matchCreator = matchCreators.at(i);
    const QString mergerCreator = mergerCreators.at(i);
    // Currently, there is only one kind of ScriptMergerCreator, so this check is useful for finding
    // misuses of the generic conflation engine.  If we add any more script merger creators, we'll
    // need a better check.
    if (matchCreator.startsWith("ScriptMatchCreator") && mergerCreator != "ScriptMergerCreator")
    {
      throw HootException(
        "Attempted to use a ScriptMatchCreator without a ScriptMergerCreator. Match creator: " +
        matchCreator + QString(" Merger creator: ")  + mergerCreator);
    }
  }
}

}
