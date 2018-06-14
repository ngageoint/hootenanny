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
 * @copyright Copyright (C) 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "DiffConflator.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/conflate/matching/MatchFactory.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/conflate/match-graph/GreedyConstrainedMatches.h>
#include <hoot/core/conflate/match-graph/OptimalConstrainedMatches.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/ops/NamedOp.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/MetadataTags.h>
#include <hoot/core/conflate/matching/MatchClassification.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/filters/TagKeyCriterion.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>

// standard
#include <algorithm>

// tgs
#include <tgs/System/SystemInfo.h>
#include <tgs/System/Time.h>
#include <tgs/System/Timer.h>

using namespace std;
using namespace Tgs;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, DiffConflator)

DiffConflator::DiffConflator() :
  _matchFactory(MatchFactory::getInstance()),
  _settings(Settings::getInstance())
{
  _reset();
}

DiffConflator::DiffConflator(boost::shared_ptr<MatchThreshold> matchThreshold) :
  _matchFactory(MatchFactory::getInstance()),
  _settings(Settings::getInstance())
{
  _matchThreshold = matchThreshold;
  _reset();
}


DiffConflator::~DiffConflator()
{
  _reset();
}

void DiffConflator::apply(OsmMapPtr& map)
{
  Timer timer;
  _reset();

  // Store the map - we might need it for tag diff later.
  _pMap = map;

  LOG_INFO("Applying pre diff-conflation operations...");
  NamedOp(ConfigOptions().getUnifyPreOps()).apply(_pMap);

  _stats.append(SingleStat("Apply Pre Ops Time (sec)", timer.getElapsedAndRestart()));

  // will reproject if necessary.
  MapProjector::projectToPlanar(_pMap);

  _stats.append(SingleStat("Project to Planar Time (sec)", timer.getElapsedAndRestart()));

  // find all the matches in this _pMap
  if (_matchThreshold.get())
  {
    _matchFactory.createMatches(_pMap, _matches, _bounds, _matchThreshold);
  }
  else
  {
    _matchFactory.createMatches(_pMap, _matches, _bounds);
  }
  LOG_DEBUG("Match count: " << _matches.size());
  LOG_TRACE(SystemInfo::getMemoryUsageString());

  double findMatchesTime = timer.getElapsedAndRestart();
  _stats.append(SingleStat("Find Matches Time (sec)", findMatchesTime));
  _stats.append(SingleStat("Number of Matches Found", _matches.size()));
  _stats.append(SingleStat("Number of Matches Found per Second",
    (double)_matches.size() / findMatchesTime));

  // Use matches to calculate and store tag diff
  // We must do this before we create the map diff
  MapProjector::projectToWgs84(_pMap);
  _calcAndStoreTagChanges();

  // Now we have matches. Here's what we are going to do, because our _pMap contains All of input1
  // and input2: we are going to delete everthing from the match pairs. Then we will delete all
  // remaining input1 items.
  for (std::vector<const Match*>::iterator mit = _matches.begin(); mit != _matches.end(); ++mit)
  {
    std::set< std::pair<ElementId, ElementId> > pairs = (*mit)->getMatchPairs();

    for (std::set< std::pair<ElementId, ElementId> >::iterator pit = pairs.begin();
         pit != pairs.end(); ++pit)
    {
      RecursiveElementRemover(pit->first).apply(_pMap);
      RecursiveElementRemover(pit->second).apply(_pMap);
    }
  }

  // Now remove input1 elements
  boost::shared_ptr<ElementCriterion> pTagKeyCrit(new TagKeyCriterion(MetadataTags::Ref1()));
  RemoveElementsVisitor removeRef1Visitor(pTagKeyCrit);
  removeRef1Visitor.setRecursive(true);
  _pMap->visitRw(removeRef1Visitor);


  LOG_INFO("Applying post-diff conflation operations...");
  NamedOp(ConfigOptions().getUnifyPostOps()).apply(_pMap);

  _stats.append(SingleStat("Apply Post Ops Time (sec)", timer.getElapsedAndRestart()));
}

void DiffConflator::setConfiguration(const Settings &conf)
{
  _settings = conf;

  _matchThreshold.reset();
  _reset();
}

MemChangesetProviderPtr DiffConflator::getTagDiff()
{
  return _pTagChanges;
}

void DiffConflator::storeOriginalIDs(OsmMapPtr& pMap)
{
  // Nodes
  const NodeMap &nodes = pMap->getNodes();
  for (auto it = nodes.begin(); it != nodes.end(); ++it)
  {
    _originalIds.insert(ElementId(ElementType::Node, it->first));
  }

  // Ways
  const WayMap &ways = pMap->getWays();
  for (auto it = ways.begin(); it != ways.end(); ++it)
  {
    _originalIds.insert(ElementId(ElementType::Way, it->first));
  }

  // Relations
  const RelationMap &relations = pMap->getRelations();
  for (auto it = relations.begin(); it != relations.end(); ++it)
  {
    _originalIds.insert(ElementId(ElementType::Relation, it->first));
  }
}

void DiffConflator::_calcAndStoreTagChanges()
{
  // Make sure we have a container for our changes
  if (!_pTagChanges)
  {
    _pTagChanges.reset(new MemChangesetProvider(_pMap->getProjection()));
  }

  for (std::vector<const Match*>::iterator mit = _matches.begin(); mit != _matches.end(); ++mit)
  {
    std::set< std::pair<ElementId, ElementId> > pairs = (*mit)->getMatchPairs();

    for (std::set< std::pair<ElementId, ElementId> >::iterator pit = pairs.begin();
         pit != pairs.end(); ++pit)
    {
      // If it's a POI-Poly match, the poi always comes first, even if it's from
      // the secondary dataset, so we can't always count on the first being
      // the old element
      ConstElementPtr pOldElement;
      ConstElementPtr pNewElement;
      if (_originalIds.end() != _originalIds.find(pit->first))
      {
        pOldElement = _pMap->getElement(pit->first);
        pNewElement = _pMap->getElement(pit->second);
      }
      else if (_originalIds.end() != _originalIds.find(pit->second))
      {
        pOldElement = _pMap->getElement(pit->second);
        pNewElement = _pMap->getElement(pit->first);
      }
      else
      {
        // How do you like me now, SonarQube?
        // Skip the work in this loop, and try again with the next pair element
        continue;
      }

      // Sometimes we get multiple matches for the same pair. We don't want to
      // make multiple changes, though.
      if (!_pTagChanges->containsChange(pOldElement->getElementId()))
      {
        if(_compareTags(pOldElement->getTags(), pNewElement->getTags()))
        {
          // Make new change
          Change newChange = _getChange(pOldElement, pNewElement);

          // Add it to our list
          _pTagChanges->addChange(newChange);
        }
      }
    }
  }
}

// See if tags are the same
bool DiffConflator::_compareTags (const Tags &oldTags, const Tags &newTags)
{
  for (Tags::const_iterator newTagIt = newTags.begin(); newTagIt != newTags.end(); ++newTagIt)
  {
    QString newTagKey = newTagIt.key();
    if (newTagKey != MetadataTags::Ref1()
        && newTagKey != "uuid"
        && newTagKey != "source:datetime"
        && newTagKey != "license"
        && newTagKey != "source:imagery")
    {
      QString newVal = newTagIt.value();
      QString oldVal = oldTags[newTagIt.key()];

      if (!oldTags.contains(newTagIt.key()) || oldTags[newTagIt.key()] != newTagIt.value())
      {
        return true;
      }
    }
  }

  return false;
}

Change DiffConflator::_getChange(ConstElementPtr pOldElement,
                                 ConstElementPtr pNewElement)
{
  // This may seem a little weird, but we want something very specific here.
  // We want the old element as it was... with new tags.

  // Copy the old one
  ElementPtr pChangeElement (pOldElement->clone());

  assert(pChangeElement->getId() == pOldElement->getId());

  // Overwrite all old tags
  pChangeElement->setTags(pNewElement->getTags());

  // Create the change
  Change newChange(Change::Modify, pChangeElement);

  return newChange;
}

void DiffConflator::_reset()
{
  _deleteAll(_matches);
  _pMap.reset();
  _pTagChanges.reset();
}

void DiffConflator::_printMatches(vector<const Match*> matches)
{
  for (size_t i = 0; i < matches.size(); i++)
  {
    LOG_DEBUG(matches[i]->toString());
  }
}

void DiffConflator::_printMatches(vector<const Match*> matches, const MatchType& typeFilter)
{
  for (size_t i = 0; i < matches.size(); i++)
  {
    const Match* match = matches[i];
    if (match->getType() == typeFilter)
    {
      LOG_DEBUG(match);
    }
  }
}

}
