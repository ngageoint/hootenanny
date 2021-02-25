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

#include "ConflateUtils.h"

// Hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>
#include <hoot/core/criterion/NonConflatableCriterion.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/ops/RemoveRoundabouts.h>
#include <hoot/core/ops/ReplaceRoundabouts.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/ops/NamedOp.h>
#include <hoot/core/visitors/RemoveMissingElementsVisitor.h>
#include <hoot/core/conflate/DiffConflator.h>
#include <hoot/core/conflate/matching/MatchFactory.h>
#include <hoot/core/schema/OsmSchema.h>

// Qt
#include <QElapsedTimer>

namespace hoot
{

thread_local QHash<QString, ElementCriterionPtr> ConflateUtils::_conflatableCritCache;
thread_local QHash<ElementId, bool> ConflateUtils::_conflatableElementCache;
thread_local QHash<QString, bool> ConflateUtils::_conflatableCritActiveCache;

int ConflateUtils::writeNonConflatable(const ConstOsmMapPtr& map, const QString& output)
{
  LOG_STATUS("Writing non-conflatable data to: ..." << output.right(25) << " ...");
  OsmMapPtr nonConflatableMap(new OsmMap(map));
  std::shared_ptr<RemoveElementsVisitor> elementRemover(new RemoveElementsVisitor(true));
  elementRemover->setRecursive(true);
  std::shared_ptr<ElementCriterion> nonConflatableCrit(
    new NonConflatableCriterion(nonConflatableMap));
  elementRemover->addCriterion(nonConflatableCrit);
  nonConflatableMap->visitRw(*elementRemover);
  if (nonConflatableMap->size() > 0)
  {
    OsmMapWriterFactory::write(nonConflatableMap, output);
  }
  return nonConflatableMap->size();
}

void ConflateUtils::writeDiff(const QString& mapUrl1, const QString& mapUrl2,
                              const geos::geom::Envelope& bounds, const QString& output)
{
  QElapsedTimer timer;
  timer.start();

  // By default rubbersheeting has no filters. When conflating, we need to add the ones from the
  // config.
  conf().set(
    ConfigOptions::getRubberSheetElementCriteriaKey(),
    ConfigOptions().getConflateRubberSheetElementCriteria());
  // don't remove/replace roundabouts during diff conflate
  QStringList preConflateOps = ConfigOptions().getConflatePreOps();
  const QString removeRoundaboutsClassName = RemoveRoundabouts::className();
  if (preConflateOps.contains(removeRoundaboutsClassName))
  {
    preConflateOps.removeAll(removeRoundaboutsClassName);
    conf().set(ConfigOptions::getConflatePreOpsKey(), preConflateOps);
  }
  QStringList postConflateOps = ConfigOptions().getConflatePostOps();
  const QString replaceRoundaboutsClassName = ReplaceRoundabouts::className();
  if (postConflateOps.contains(replaceRoundaboutsClassName))
  {
    postConflateOps.removeAll(replaceRoundaboutsClassName);
    conf().set(ConfigOptions::getConflatePostOpsKey(), postConflateOps);
  }

  LOG_STATUS("Loading data for diff calc from: ..." << mapUrl1.right(25) << "...");
  OsmMapPtr diffMap(new OsmMap());
  IoUtils::loadMap(diffMap, mapUrl1, true, Status::Unknown1);
  if (!bounds.isNull())
  {
    IoUtils::cropToBounds(diffMap, bounds);
  }
  const int replacementMapSize = diffMap->size();
  LOG_STATUS(
    "Data from ..." << mapUrl1.right(25) << " for diff calc loaded in: " <<
    StringUtils::millisecondsToDhms(timer.elapsed()));
  timer.restart();

  LOG_STATUS("Loading data for diff calc from: ..." << mapUrl2.right(25) << "...");
  IoUtils::loadMap(diffMap, mapUrl2, false, Status::Unknown2);
  if (!bounds.isNull())
  {
    IoUtils::cropToBounds(diffMap, bounds);
  }
  LOG_STATUS(
    "Data from ..." << mapUrl2.right(25) << " for diff calc loaded in: " <<
    StringUtils::millisecondsToDhms(timer.elapsed()));
  timer.restart();

  // had to do this cleaning to get the relations to behave
  RemoveMissingElementsVisitor missingElementRemover;
  diffMap->visitRw(missingElementRemover);
  LOG_STATUS(missingElementRemover.getCompletedStatusMessage());
  OsmMapWriterFactory::writeDebugMap(diffMap, "task-grid-replacer-diff-input");

  LOG_STATUS(
    "Calculating the diff between replaced data of size: " <<
    StringUtils::formatLargeNumber(diffMap->size() - replacementMapSize) <<
    " and replacement data of size: " << StringUtils::formatLargeNumber(replacementMapSize)  <<
    "...");
  NamedOp(ConfigOptions().getConflatePreOps()).apply(diffMap);
  DiffConflator diffGen;
  diffGen.apply(diffMap);
  NamedOp(ConfigOptions().getConflatePostOps()).apply(diffMap);
  LOG_STATUS(
    "Calculated a diff with: " << StringUtils::formatLargeNumber(diffMap->size()) <<
    " features in: " << StringUtils::millisecondsToDhms(timer.elapsed()) << " (skipped " <<
    StringUtils::formatLargeNumber(diffGen.getNumUnconflatableElementsDiscarded()) <<
    " unconflatable)");
  timer.restart();

  LOG_STATUS(
    "Writing the diff output of size: " << StringUtils::formatLargeNumber(diffMap->size()) <<
    " to: ..." << output.right(25) << "...");
  IoUtils::saveMap(diffMap, output);
  LOG_STATUS(
    "Wrote the diff output of size: " << StringUtils::formatLargeNumber(diffMap->size()) <<
    " in: " << StringUtils::millisecondsToDhms(timer.elapsed()));
  timer.restart();
}

bool ConflateUtils::elementCanBeConflatedByActiveMatcher(
  const ConstElementPtr& element, const ConstOsmMapPtr& map)
{
  //LOG_VARD(element->getElementId());
  //LOG_VARD(_conflatableElementCache.contains(element->getElementId()));

  if (_conflatableElementCache.contains(element->getElementId()))
  {
    return _conflatableElementCache[element->getElementId()];
  }

  LOG_VARD(element->getElementId());
  LOG_VART(map.get());

  // Get all the configured matchers.
  const std::vector<std::shared_ptr<MatchCreator>> activeMatchCreators =
    MatchFactory::getInstance().getCreators();
  for (std::vector<std::shared_ptr<MatchCreator>>::const_iterator itr = activeMatchCreators.begin();
       itr != activeMatchCreators.end(); ++itr)
  {
    // Get the element criterion this matcher uses for matching elements.
    std::shared_ptr<MatchCreator> activeMatchCreator = *itr;
    LOG_VART(activeMatchCreator->getName());
    const QStringList supportedCriteriaClassNames = activeMatchCreator->getCriteria();
    for (int i = 0; i < supportedCriteriaClassNames.size(); i++)
    {
      const QString criterionClassName = supportedCriteriaClassNames.at(i);
      LOG_VART(criterionClassName);

      ElementCriterionPtr crit;
      std::shared_ptr<ConflatableElementCriterion> conflatableCrit;
      ElementCriterionPtr childCrit;
      std::shared_ptr<ConflatableElementCriterion> conflatableChildCrit;
      LOG_VART(_conflatableCritCache.contains(criterionClassName));
      if (_conflatableCritCache.contains(criterionClassName)) // Check the cache for the criterion.
      {
        crit = _conflatableCritCache[criterionClassName];
      }
      else
      {
        crit.reset(
          Factory::getInstance().constructObject<ElementCriterion>(criterionClassName));
        conflatableCrit = std::dynamic_pointer_cast<ConflatableElementCriterion>(crit);
        if (conflatableCrit)
        {
          // Crit creation can be expensive, so cache those created.
          _conflatableCritCache[criterionClassName] = crit;

          // All ElementCriterion that are map consumers inherit from ConstOsmMapConsumer, so this
          // works.
          std::shared_ptr<ConstOsmMapConsumer> mapConsumer =
            std::dynamic_pointer_cast<ConstOsmMapConsumer>(crit);
          if (mapConsumer)
          {
            mapConsumer->setOsmMap(map.get());
          }

          LOG_VARD(crit->isSatisfied(element));
          // If any matcher's crit matches the element, return true.
          if (crit->isSatisfied(element))
          {
            _conflatableElementCache[element->getElementId()] = true;
            return true;
          }
          else if (!conflatableCrit->getChildCriteria().isEmpty())
          {
            // Also, check for conflate "child" criteria (e.g. RailwayWayNodeCriterion).
            const QStringList childCritClassNames = conflatableCrit->getChildCriteria();
            for (int i = 0; i < childCritClassNames.size(); i++)
            {
              const QString childCritClassName = childCritClassNames.at(i);
              LOG_VARD(childCritClassName);
              LOG_VARD(_conflatableCritCache.contains(childCritClassName));
              if (_conflatableCritCache.contains(childCritClassName))
              {
                childCrit = _conflatableCritCache[criterionClassName];
              }
              else
              {
                childCrit.reset(
                  Factory::getInstance().constructObject<ElementCriterion>(childCritClassName));
                if (childCrit)
                {
                  _conflatableCritCache[childCritClassName] = childCrit;

                  std::shared_ptr<ConstOsmMapConsumer> mapConsumer =
                    std::dynamic_pointer_cast<ConstOsmMapConsumer>(childCrit);
                  if (mapConsumer)
                  {
                    mapConsumer->setOsmMap(map.get());
                  }
                }
              }

              // These don't inherit from ConflatableElementCriterion, since matchers look for
              // their parents only. So, don't check that it casts to ConflatableElementCriterion.
              if (childCrit)
              {
                _conflatableCritCache[childCritClassName] = childCrit;

                LOG_VARD(childCrit->isSatisfied(element));
                // If any matcher's crit matches the element, return true.
                if (childCrit->isSatisfied(element))
                {
                  _conflatableElementCache[element->getElementId()] = true;
                  return true;
                }
              }
            }
          }
        }
      }
    }
  }

  _conflatableElementCache[element->getElementId()] = false;
  LOG_VARD(element->getElementId());
  LOG_VARD(OsmSchema::getInstance().mostSpecificType(element->getTags()));
  return false;
}

bool ConflateUtils::elementCriterionInUseByActiveMatcher(
  const QString& criterionClassName, const ConstOsmMapPtr& map)
{
  LOG_VART(criterionClassName);

  if (_conflatableCritActiveCache.contains(criterionClassName))
  {
    return _conflatableCritActiveCache[criterionClassName];
  }

  // Get all the configured matchers.
  const std::vector<std::shared_ptr<MatchCreator>> activeMatchCreators =
    MatchFactory::getInstance().getCreators();
  for (std::vector<std::shared_ptr<MatchCreator>>::const_iterator itr = activeMatchCreators.begin();
       itr != activeMatchCreators.end(); ++itr)
  {
    // Get the element criterion this matcher uses for matching elements.
    std::shared_ptr<MatchCreator> activeMatchCreator = *itr;
    const QStringList supportedCriteriaClassNames = activeMatchCreator->getCriteria();
    for (int i = 0; i < supportedCriteriaClassNames.size(); i++)
    {
      const QString matcherCriterionClassName = supportedCriteriaClassNames.at(i);
      LOG_VART(matcherCriterionClassName);
      if (matcherCriterionClassName == criterionClassName)
      {
        _conflatableCritActiveCache[criterionClassName] = true;
        return true;
      }
      else
      {
        ElementCriterionPtr crit;
        if (_conflatableCritCache.contains(matcherCriterionClassName))
        {
          // Check the cache for the criterion.
          crit = _conflatableCritCache[matcherCriterionClassName];
        }
        else
        {
          crit.reset(
            Factory::getInstance().constructObject<ElementCriterion>(matcherCriterionClassName));
          std::shared_ptr<ConflatableElementCriterion> conflatableCrit =
            std::dynamic_pointer_cast<ConflatableElementCriterion>(crit);
          if (conflatableCrit)
          {
            // All ElementCriterion that are map consumers inherit from ConstOsmMapConsumer, so this
            // works.
            std::shared_ptr<ConstOsmMapConsumer> mapConsumer =
              std::dynamic_pointer_cast<ConstOsmMapConsumer>(crit);
            LOG_VART(mapConsumer.get());
            if (mapConsumer)
            {
              mapConsumer->setOsmMap(map.get());
            }

            // Crit creation can be expensive, so cache those created.
            _conflatableCritCache[matcherCriterionClassName] = crit;

            if (!conflatableCrit->getChildCriteria().isEmpty())
            {
              // Also, check for conflate "child" criteria (e.g. RailwayWayNodeCriterion). These
              // don't inherit from ConflatableElementCriterion, since matchers look for their
              // parents only.
              const QStringList childCritClassNames = conflatableCrit->getChildCriteria();
              for (int i = 0; i < childCritClassNames.size(); i++)
              {
                const QString childCritClassName = childCritClassNames.at(i);
                LOG_VART(childCritClassName);
                if (childCritClassName == criterionClassName)
                {
                  _conflatableCritActiveCache[criterionClassName] = true;
                  return true;
                }
              }
            }
          }
        }
      }
    }
  }
  _conflatableCritActiveCache[criterionClassName] = false;
  return false;
}

}
