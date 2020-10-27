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
#include "MergerFactory.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/OsmMapConsumer.h>
#include <hoot/core/conflate/matching/Match.h>
#include <hoot/core/conflate/matching/MatchType.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/conflate/merging/MarkForReviewMerger.h>

using namespace std;

namespace hoot
{

int MergerFactory::logWarnCount = 0;

std::shared_ptr<MergerFactory> MergerFactory::_theInstance;

MergerFactory::MergerFactory()
{
}

MergerFactory::~MergerFactory()
{
  reset();
}

void MergerFactory::reset()
{
  _creators.clear();
}

void MergerFactory::markInterMatcherOverlappingMatchesAsReviews(
  MatchSetVector& matchSets, std::vector<MergerPtr>& mergers,
  const QStringList& matchNameFilter)
{
  LOG_DEBUG(
    "Marking overlapping matches across matchers as reviews for " <<
    StringUtils::formatLargeNumber(matchSets.size()) << " match sets...");
  LOG_VARD(matchNameFilter);

  // Get a mapping of all element IDs to the match type they belong to. The same element may belong
  // to matches of multiple types.
  QMultiHash<ElementId, QString> elementIdsToMatchTypes;
  for (MatchSetVector::const_iterator matchSetsItr = matchSets.begin();
       matchSetsItr != matchSets.end(); ++matchSetsItr)
  {
    MatchSet matchSet = *matchSetsItr;
    for (MatchSet::const_iterator matchSetItr = matchSet.begin(); matchSetItr != matchSet.end();
         ++matchSetItr)
    {
      ConstMatchPtr match = *matchSetItr;
      const QString matchName = match->getMatchName();
      if (matchNameFilter.isEmpty() || matchNameFilter.contains(matchName))
      {
        const std::set<std::pair<ElementId, ElementId>> matchPairs = match->getMatchPairs();
        for (std::set<std::pair<ElementId, ElementId>>::const_iterator matchPairItr =
               matchPairs.begin();
             matchPairItr != matchPairs.end(); ++matchPairItr)
        {
          const std::pair<ElementId, ElementId> elementPair = *matchPairItr;
          if (!elementIdsToMatchTypes.contains(elementPair.first, matchName))
          {
              elementIdsToMatchTypes.insert(elementPair.first, matchName);
          }
          if (!elementIdsToMatchTypes.contains(elementPair.second, matchName))
          {
            elementIdsToMatchTypes.insert(elementPair.second, matchName);
          }
        }
      }
    }
  }
  LOG_VARD(elementIdsToMatchTypes.size());
  if (elementIdsToMatchTypes.isEmpty())
  {
    return;
  }

  // Find all elements involved in matches of more than one type.
  QSet<ElementId> elementIdsInvolvedInOverlappingMatch;
  const QList<ElementId> elementIds = elementIdsToMatchTypes.keys();
  for (QList<ElementId>::const_iterator elementIdsItr = elementIds.begin();
       elementIdsItr != elementIds.end();  ++elementIdsItr)
  {
    const ElementId elementId = *elementIdsItr;
    if (elementIdsToMatchTypes.values(elementId).size() > 1)
    {
      elementIdsInvolvedInOverlappingMatch.insert(elementId);
    }
  }
  LOG_VARD(elementIdsInvolvedInOverlappingMatch.size());
  if (elementIdsInvolvedInOverlappingMatch.isEmpty())
  {
    return;
  }

  // For all elements found to be in overlapping matches, add a review merger for the associated
  // match pair, and exclude the match set that match pair is in from the output match set so the
  // match set doesn't get processed more than once.
  MatchSetVector filteredMatchSets;
  for (MatchSetVector::const_iterator matchSetsItr = matchSets.begin();
       matchSetsItr != matchSets.end(); ++matchSetsItr)
  {
    MatchSet filteredMatchSet;
    MatchSet matchSet = *matchSetsItr;
    for (MatchSet::const_iterator matchSetItr = matchSet.begin(); matchSetItr != matchSet.end();
         ++matchSetItr)
    {
      ConstMatchPtr match = *matchSetItr;
        const std::set<std::pair<ElementId, ElementId>> matchPairs = match->getMatchPairs();
      for (std::set<std::pair<ElementId, ElementId>>::const_iterator matchPairItr =
             matchPairs.begin();
           matchPairItr != matchPairs.end(); ++matchPairItr)
      {
        const std::pair<ElementId, ElementId> elementPair = *matchPairItr;
        if (match->getMatchName() == "POI to Polygon" &&
            (elementIdsInvolvedInOverlappingMatch.contains(elementPair.first) ||
             elementIdsInvolvedInOverlappingMatch.contains(elementPair.second)))
        {
          LOG_TRACE(
            "Adding review for inter-dataset conflict; type: " << match->getMatchName() <<
            ", ids: " << matchPairs << "...");
          mergers.push_back(
            MergerPtr(
              new MarkForReviewMerger(
                matchPairs, "Inter-matcher overlapping matches", match->getMatchName(), 1.0)));
        }
        else
        {
          filteredMatchSet.insert(match);
        }
      }
      if (filteredMatchSet.size() != 0)
      {
        filteredMatchSets.push_back(matchSet);
      }
    }
  }
  LOG_VARD(mergers.size());
  LOG_VARD(filteredMatchSets.size());
  matchSets = filteredMatchSets;
}

void MergerFactory::createMergers(
  const OsmMapPtr& map, const MatchSet& matches, vector<MergerPtr>& result) const
{
  LOG_TRACE(
    "Creating merger group for " <<
    StringUtils::formatLargeNumber(matches.size()) << " matches...");
  for (size_t i = 0; i < _creators.size(); i++)
  {
    PROGRESS_DEBUG(
      "Creating merger group " << i + 1 << " / " << _creators.size() << " for " <<
      StringUtils::formatLargeNumber(matches.size()) << " match(es)...");

    OsmMapConsumer* omc = dynamic_cast<OsmMapConsumer*>(_creators[i].get());
    if (omc)
    {
      omc->setOsmMap(map.get());
    }
    if (_creators[i]->createMergers(matches, result))
    {
      return;
    }
    // We don't want the creators to hold onto a map pointer that will go out of scope.
    if (omc)
    {
      omc->setOsmMap((OsmMap*)0);
    }
  }

  // In #2069, a ScriptMatch and a NetworkMatch are being grouped together, which ultimately causes
  // the exception below to be thrown. Now, instead of an error we're only logging a warning. This
  // also required additional error handling in ScriptMerger (see ScriptMerger::_applyMergePair).
  if (logWarnCount < Log::getWarnMessageLimit())
  {
    LOG_WARN("Unable to create merger for the provided set of matches: " << matches);
  }
  else if (logWarnCount == Log::getWarnMessageLimit())
  {
    LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
  }
  logWarnCount++;
}

vector<CreatorDescription> MergerFactory::getAllAvailableCreators() const
{
  vector<CreatorDescription> result;

  // get all match creators from the factory
  vector<std::string> names =
    Factory::getInstance().getObjectNamesByBase(MergerCreator::className());

  for (size_t i = 0; i < names.size(); i++)
  {
    // get all names known by this creator.
    std::shared_ptr<MergerCreator> mc(
      Factory::getInstance().constructObject<MergerCreator>(names[i]));

    vector<CreatorDescription> d = mc->getAllCreators();
    result.insert(result.end(), d.begin(), d.end());
  }

  return result;
}

MergerFactory& MergerFactory::getInstance()
{
  if (!_theInstance.get())
  {
    _theInstance.reset(new MergerFactory());
  }

  if (_theInstance->_creators.size() == 0)
  {
    _theInstance->registerDefaultCreators();
  }

  return *_theInstance;
}

bool MergerFactory::isConflicting(
  const ConstOsmMapPtr& map, const ConstMatchPtr& m1, const ConstMatchPtr& m2,
  const QHash<QString, ConstMatchPtr>& matches) const
{
  // if any creator considers a match conflicting then it is a conflict
  for (size_t i = 0; i < _creators.size(); i++)
  {
    if (_creators[i]->isConflicting(map, m1, m2, matches))
    {
      LOG_TRACE("Conflicting matches: " << m1 << ", " << m2);
      return true;
    }
  }
  return false;
}

void MergerFactory::registerDefaultCreators()
{  
  const QStringList mergerCreators = ConfigOptions().getMergerCreators();
  LOG_VARD(mergerCreators);
  for (int i = 0; i < mergerCreators.size(); i++)
  {
    QString c = mergerCreators[i];
    QStringList args = c.split(",");
    QString className = args[0];
    if (className.length() > 0)
    {
      args.removeFirst();
      MergerCreatorPtr mc(Factory::getInstance().constructObject<MergerCreator>(className));
      registerCreator(mc);

      if (args.size() > 0)
      {
        mc->setArguments(args);
      }
    }
  }
}

}

