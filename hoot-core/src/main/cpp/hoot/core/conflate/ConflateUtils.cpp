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
#include <hoot/core/conflate/DiffConflator.h>
#include <hoot/core/conflate/network/NetworkMatchCreator.h>
#include <hoot/core/criterion/ConflatableElementCriterion.h>
#include <hoot/core/criterion/NonConflatableCriterion.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/ops/OpExecutor.h>
#include <hoot/core/ops/RemoveRoundabouts.h>
#include <hoot/core/ops/ReplaceRoundabouts.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>
#include <hoot/core/visitors/RemoveMissingElementsVisitor.h>

// Qt
#include <QElapsedTimer>

namespace hoot
{

int ConflateUtils::writeNonConflatable(
  const ConstOsmMapPtr& map, const QString& output, const bool ignoreGenericConflators)
{
  LOG_STATUS("Writing non-conflatable data to: ..." << output.right(25) << " ...");

  OsmMapPtr nonConflatableMap(new OsmMap(map));
  LOG_VART(nonConflatableMap->size());
  std::shared_ptr<RemoveElementsVisitor> elementRemover(new RemoveElementsVisitor(true));
  elementRemover->setRecursive(true);
  std::shared_ptr<NonConflatableCriterion> nonConflatableCrit(
    new NonConflatableCriterion(nonConflatableMap));
  nonConflatableCrit->setIgnoreGenericConflators(ignoreGenericConflators);
  elementRemover->addCriterion(nonConflatableCrit);
  nonConflatableMap->visitRw(*elementRemover);

  LOG_VART(nonConflatableMap->size());
  if (nonConflatableMap->size() > 0)
  {
    OsmMapWriterFactory::write(nonConflatableMap, output);
  }

  return nonConflatableMap->size();
}

void ConflateUtils::writeDiff(const QString& mapUrl1, const QString& mapUrl2,
                              const geos::geom::Envelope& bounds, const QString& output)
{
  //conf().set(ConfigOptions::getDifferentialRemoveLinearPartialMatchesAsWholeKey(), true);

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
  OpExecutor(ConfigOptions().getConflatePreOps()).apply(diffMap);
  DiffConflator diffGen;
  diffGen.apply(diffMap);
  OpExecutor(ConfigOptions().getConflatePostOps()).apply(diffMap);
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

bool ConflateUtils::isNetworkConflate()
{
  // This could be brittle to future changes to how the Network alg is applied.
  return
    conf().getList(ConfigOptions::getMatchCreatorsKey()).contains(NetworkMatchCreator::className());
}

}
