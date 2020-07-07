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
#include "PertyMatchScorer.h"

// hoot
#include <hoot/core/algorithms/perty/PertyOp.h>
#include <hoot/core/algorithms/rubber-sheet/RubberSheet.h>
#include <hoot/core/conflate/UnifyingConflator.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/ops/BuildingOutlineUpdateOp.h>
#include <hoot/core/ops/MapCleaner.h>
#include <hoot/core/ops/NamedOp.h>
#include <hoot/core/algorithms/rubber-sheet/RubberSheet.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/scoring/MatchScoringMapPreparer.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/visitors/AddRef1Visitor.h>
#include <hoot/core/visitors/SetTagValueVisitor.h>
#include <hoot/core/visitors/TagCountVisitor.h>
#include <hoot/core/visitors/TagRenameKeyVisitor.h>

// Qt
#include <QFileInfo>
#include <QDir>

namespace hoot
{

PertyMatchScorer::PertyMatchScorer() :
_settings(conf())
{
  ConfigOptions configOptions;
  setSearchDistance(configOptions.getPertySearchDistance());
}

QString PertyMatchScorer::toString()
{
  return "_searchDistance: " + QString::number(_searchDistance);
}

std::shared_ptr<MatchComparator> PertyMatchScorer::scoreMatches(const QString& referenceMapInputPath,
                                                                const QString& outputPath)
{
  LOG_INFO(toString());

  FileUtils::makeDir(outputPath);
  QFileInfo inputFileInfo(referenceMapInputPath);
  const QString referenceMapOutputPath =
    outputPath + "/" + inputFileInfo.baseName() + "-reference-out.osm";
  _referenceMapOutput = referenceMapOutputPath;
  const QString perturbedMapOutputPath =
    outputPath + "/" + inputFileInfo.baseName() + "-perturbed-out.osm";
  _perturbedMapOutput = perturbedMapOutputPath;
  const QString combinedMapOutputPath =
    outputPath + "/" + inputFileInfo.baseName() + "-combined-out.osm";
  const QString conflatedMapOutputPath =
    outputPath + "/" + inputFileInfo.baseName() + "-conflated-out.osm";
  _conflatedMapOutput = conflatedMapOutputPath;

  OsmMapPtr referenceMap = _loadReferenceMap(referenceMapInputPath, referenceMapOutputPath);
  _loadPerturbedMap(referenceMapOutputPath, perturbedMapOutputPath);
  OsmMapPtr combinedMap =
    _combineMapsAndPrepareForConflation(referenceMap, perturbedMapOutputPath);

  MapProjector::projectToWgs84(combinedMap);
  IoUtils::saveMap(combinedMap, combinedMapOutputPath);

  return _conflateAndScoreMatches(combinedMap, conflatedMapOutputPath);
}

OsmMapPtr PertyMatchScorer::_loadReferenceMap(const QString& referenceMapInputPath,
                                              const QString& referenceMapOutputPath)
{
  LOG_DEBUG(
    "Loading the reference data with status " << MetadataTags::Unknown1() << " and adding " <<
    MetadataTags::Ref1() << " tags to it; Saving a copy to " << referenceMapOutputPath << "...");

  OsmMapPtr referenceMap(new OsmMap());
  IoUtils::loadMap(referenceMap, referenceMapInputPath, false, Status::Unknown1);
  MapCleaner().apply(referenceMap);

  std::shared_ptr<AddRef1Visitor> addRef1Visitor(new AddRef1Visitor());
  referenceMap->visitRw(*addRef1Visitor);
  std::shared_ptr<SetTagValueVisitor> setAccuracyVisitor(
    new SetTagValueVisitor(MetadataTags::ErrorCircular(), QString::number(_searchDistance)));
  referenceMap->visitRw(*setAccuracyVisitor);
  LOG_VARD(referenceMap->getNodes().size());
  LOG_VARD(referenceMap->getWays().size());
  if (Log::getInstance().getLevel() <= Log::Debug)
  {
    TagCountVisitor tagCountVisitor;
    referenceMap->visitRo(tagCountVisitor);
    const long numTotalTags = (long)tagCountVisitor.getStat();
    LOG_VARD(numTotalTags);
  }

  OsmMapPtr referenceMapCopy(referenceMap);
  MapProjector::projectToWgs84(referenceMapCopy);
  IoUtils::saveMap(referenceMapCopy, referenceMapOutputPath);

  return referenceMap;
}

void PertyMatchScorer::_loadPerturbedMap(const QString& perturbedMapInputPath,
                                         const QString& perturbedMapOutputPath)
{
  LOG_DEBUG("Loading the reference data to be used by the data to be perturbed; renaming " <<
            MetadataTags::Ref1() << " tags to " << MetadataTags::Ref2() << "...");

  // load from the modified reference data output to get the added ref1 tags; don't copy the map,
  // since updates to the names of the ref tags on this map will propagate to the map copied from
  OsmMapPtr perturbedMap(new OsmMap());
  IoUtils::loadMap(perturbedMap, perturbedMapInputPath, false, Status::Unknown2);
  MapCleaner().apply(perturbedMap);

  std::shared_ptr<TagRenameKeyVisitor> tagRenameKeyVisitor(
    new TagRenameKeyVisitor(MetadataTags::Ref1(), MetadataTags::Ref2()));
  perturbedMap->visitRw(*tagRenameKeyVisitor);
  // This could be replaced with a SetTagValueVisitor passed in from the command line
  // instead.
  std::shared_ptr<SetTagValueVisitor> setAccuracyVisitor(
    new SetTagValueVisitor(MetadataTags::ErrorCircular(), QString::number(_searchDistance)));
  perturbedMap->visitRw(*setAccuracyVisitor);
  LOG_VARD(perturbedMap->getNodes().size());
  LOG_VARD(perturbedMap->getWays().size());
  if (Log::getInstance().getLevel() <= Log::Debug)
  {
    TagCountVisitor tagCountVisitor;
    perturbedMap->visitRo(tagCountVisitor);
    const long numTotalTags = (long)tagCountVisitor.getStat();
    LOG_VARD(numTotalTags);
  }

  LOG_DEBUG("Perturbing the copied reference data and saving it to: " << perturbedMapOutputPath);

  PertyOp pertyOp;
  pertyOp.setConfiguration(_settings);
  pertyOp.apply(perturbedMap);
  LOG_VARD(perturbedMap->getNodes().size());
  LOG_VARD(perturbedMap->getWays().size());
  if (Log::getInstance().getLevel() <= Log::Debug)
  {
    TagCountVisitor tagCountVisitor;
    perturbedMap->visitRo(tagCountVisitor);
    const long numTotalTags = (long)tagCountVisitor.getStat();
    LOG_VARD(numTotalTags);
  }

  MapProjector::projectToWgs84(perturbedMap);
  IoUtils::saveMap(perturbedMap, perturbedMapOutputPath);
}

OsmMapPtr PertyMatchScorer::_combineMapsAndPrepareForConflation(
  const OsmMapPtr& referenceMap, const QString& perturbedMapInputPath)
{
  LOG_DEBUG("Combining the reference and perturbed data into a single file ...");

  OsmMapPtr combinedMap(referenceMap);
  IoUtils::loadMap(combinedMap, perturbedMapInputPath, false, Status::Unknown2);
  LOG_VARD(combinedMap->getNodes().size());
  LOG_VARD(combinedMap->getWays().size());
  if (Log::getInstance().getLevel() <= Log::Debug)
  {
    TagCountVisitor tagCountVisitor;
    combinedMap->visitRo(tagCountVisitor);
    const long numTotalTags = (long)tagCountVisitor.getStat();
    LOG_VARD(numTotalTags);
  }

  MatchScoringMapPreparer().prepMap(combinedMap, true);
  LOG_VARD(combinedMap->getNodes().size());
  LOG_VARD(combinedMap->getWays().size());
  if (Log::getInstance().getLevel() <= Log::Debug)
  {
    TagCountVisitor tagCountVisitor;
    combinedMap->visitRo(tagCountVisitor);
    const long numTotalTags = (long)tagCountVisitor.getStat();
    LOG_VARD(numTotalTags);
  }

  if (ConfigOptions().getConflatePreOps().contains(QString::fromStdString(RubberSheet::className())))
  {
    // move Unknown2 toward Unknown1
    conf().set(RubberSheet::refKey(), true);
    std::shared_ptr<RubberSheet> rubberSheetOp(new RubberSheet());
    rubberSheetOp->setConfiguration(conf());
    rubberSheetOp->apply(combinedMap);

    LOG_VARD(combinedMap->getNodes().size());
    LOG_VARD(combinedMap->getWays().size());
    if (Log::getInstance().getLevel() <= Log::Debug)
    {
      TagCountVisitor tagCountVisitor;
      combinedMap->visitRo(tagCountVisitor);
      const long numTotalTags = (long)tagCountVisitor.getStat();
      LOG_VARD(numTotalTags);
    }
  }

  return combinedMap;
}

std::shared_ptr<MatchComparator> PertyMatchScorer::_conflateAndScoreMatches(
  const OsmMapPtr& combinedDataToConflate, const QString& conflatedMapOutputPath)
{
  LOG_DEBUG("Conflating the reference data with the perturbed data, scoring the matches, and " <<
            "saving the conflated output to: " << conflatedMapOutputPath);

  std::shared_ptr<MatchComparator> comparator(new MatchComparator());
  OsmMapPtr conflationCopy(new OsmMap(combinedDataToConflate));

  // TODO: We're not applying pre/post conflate ops here, since they tank scores. Should we be?

  //NamedOp preOps(ConfigOptions().getConflatePreOps());
  //preOps.apply(conflationCopy);

  UnifyingConflator conflator;
  conflator.setConfiguration(_settings);
  conflator.apply(conflationCopy);

  //NamedOp postOps(ConfigOptions().getConflatePostOps());
  //postOps.apply(conflationCopy);

  try
  {
    comparator->evaluateMatches(combinedDataToConflate, conflationCopy);
  }
  catch (const HootException&)
  {
    // save map modifies the map so we want to make sure comparator runs first. 'finally' would be
    // nice here.
    _saveMap(conflationCopy, conflatedMapOutputPath);
    throw;
  }

  _saveMap(conflationCopy, conflatedMapOutputPath);

  return comparator;
}

void PertyMatchScorer::_saveMap(OsmMapPtr& map, const QString& path)
{
  BuildingOutlineUpdateOp().apply(map);

  LOG_VARD(map->getNodes().size());
  LOG_VARD(map->getWays().size());
  if (Log::getInstance().getLevel() <= Log::Debug)
  {
    TagCountVisitor tagCountVisitor;
    map->visitRo(tagCountVisitor);
    const long numTotalTags = (long)tagCountVisitor.getStat();
    LOG_VARD(numTotalTags);
  }

  MapProjector::projectToWgs84(map);
  IoUtils::saveMap(map, path);
}

}
