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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "PertyMatchScorer.h"

// hoot
#include <hoot/core/conflate/Conflator.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/conflate/MapCleaner.h>
#include <hoot/core/conflate/MatchThreshold.h>
#include <hoot/core/conflate/RubberSheet.h>
#include <hoot/core/conflate/UnifyingConflator.h>
#include <hoot/core/ops/BuildingOutlineUpdateOp.h>
#include <hoot/core/scoring/MatchScoringMapPreparer.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/MetadataTags.h>
#include <hoot/core/util/OsmUtils.h>
#include <hoot/core/visitors/AddRef1Visitor.h>
#include <hoot/core/visitors/SetTagVisitor.h>
#include <hoot/core/visitors/TagCountVisitor.h>
#include <hoot/core/visitors/TagRenameKeyVisitor.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QFileInfo>
#include <QDir>

#include "PertyOp.h"

namespace hoot
{

PertyMatchScorer::PertyMatchScorer() :
  _settings(conf())
{
  ConfigOptions configOptions;
  setSearchDistance(configOptions.getPertySearchDistance());
  setApplyRubberSheet(configOptions.getPertyApplyRubberSheet());
}

QString PertyMatchScorer::toString()
{
  QString str = "_searchDistance: " + QString::number(_searchDistance) + ", _applyRubberSheet: ";
  if (_applyRubberSheet)
  {
    str += "true";
  }
  else
  {
    str += "false";
  }
  return str;
}

boost::shared_ptr<MatchComparator> PertyMatchScorer::scoreMatches(const QString referenceMapInputPath,
                                                           const QString outputPath)
{
  LOG_INFO(toString());

  QDir().mkpath(outputPath);
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
  OsmUtils::saveMap(combinedMap, combinedMapOutputPath);

  return _conflateAndScoreMatches(combinedMap, conflatedMapOutputPath);
}

OsmMapPtr PertyMatchScorer::_loadReferenceMap(const QString referenceMapInputPath,
                                                       const QString referenceMapOutputPath)
{
  LOG_DEBUG("Loading the reference data with status " << MetadataTags::Unknown1() << " and adding " << MetadataTags::Ref1() <<
            " tags to it; Saving a copy to " << referenceMapOutputPath << "...");

 OsmMapPtr referenceMap(new OsmMap());
  OsmUtils::loadMap(referenceMap, referenceMapInputPath, false, Status::Unknown1);
  MapCleaner().apply(referenceMap);

 boost::shared_ptr<AddRef1Visitor> addRef1Visitor(new AddRef1Visitor());
  referenceMap->visitRw(*addRef1Visitor);
 boost::shared_ptr<SetTagVisitor> setAccuracyVisitor(
    new SetTagVisitor(MetadataTags::ErrorCircular(), QString::number(_searchDistance)));
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
  OsmUtils::saveMap(referenceMapCopy, referenceMapOutputPath);

  return referenceMap;
}

void PertyMatchScorer::_loadPerturbedMap(const QString perturbedMapInputPath,
                                         const QString perturbedMapOutputPath)
{
  LOG_DEBUG("Loading the reference data to be used by the data to be perturbed; renaming " <<
            MetadataTags::Ref1() << " tags to " << MetadataTags::Ref2() << "...");

  //load from the modified reference data output to get the added ref1 tags; don't copy the map,
  //since updates to the names of the ref tags on this map will propagate to the map copied from
 OsmMapPtr perturbedMap(new OsmMap());
  OsmUtils::loadMap(perturbedMap, perturbedMapInputPath, false, Status::Unknown2);
  MapCleaner().apply(perturbedMap);

 boost::shared_ptr<TagRenameKeyVisitor> tagRenameKeyVisitor(new TagRenameKeyVisitor(MetadataTags::Ref1(), MetadataTags::Ref2()));
  perturbedMap->visitRw(*tagRenameKeyVisitor);
  // This could be replaced with a SetTagVisitor passed in from the command line
  // instead.
 boost::shared_ptr<SetTagVisitor> setAccuracyVisitor(
    new SetTagVisitor(MetadataTags::ErrorCircular(), QString::number(_searchDistance)));
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
  LOG_DEBUG("Details: " << pertyOp.toString());
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
  OsmUtils::saveMap(perturbedMap, perturbedMapOutputPath);
}

OsmMapPtr PertyMatchScorer::_combineMapsAndPrepareForConflation(
 OsmMapPtr referenceMap, const QString perturbedMapInputPath)
{
  LOG_DEBUG("Combining the reference and perturbed data into a single file ...");

//  QFileInfo fileInfo(perturbedMapInputPath);
//  QString combinedOutputPath = fileInfo.path() + "/ref-after-combination.osm";
//  LOG_DEBUG("saving a debug copy to " << combinedOutputPath << " ...");

 OsmMapPtr combinedMap(referenceMap);
  OsmUtils::loadMap(combinedMap, perturbedMapInputPath, false, Status::Unknown2);
  LOG_VARD(combinedMap->getNodes().size());
  LOG_VARD(combinedMap->getWays().size());
  if (Log::getInstance().getLevel() <= Log::Debug)
  {
    TagCountVisitor tagCountVisitor;
    combinedMap->visitRo(tagCountVisitor);
    const long numTotalTags = (long)tagCountVisitor.getStat();
    LOG_VARD(numTotalTags);
  }

// OsmMapPtr combinedMapCopy(combinedMap);
//  MapProjector::reprojectToWgs84(combinedMapCopy);
//  OsmUtils::saveMap(combinedMapCopy, combinedOutputPath);

//  LOG_DEBUG("Preparing the reference data for conflation ...");
//  QString combinedOutputPath2 = fileInfo.path() + "/ref-after-prep.osm";
//  LOG_DEBUG("saving a debug copy to " << combinedOutputPath2 << " ...");

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

// OsmMapPtr combinedMapCopy2(combinedMap);
//  MapProjector::reprojectToWgs84(combinedMapCopy2);
//  OsmUtils::saveMap(combinedMapCopy2, combinedOutputPath2);

  if (_applyRubberSheet)
  {
    //  LOG_DEBUG("Applying rubber sheet to pre-conflated data to move perturbed data towards " <<
    //            "reference data ...");
    //  QString combinedOutputPath3 = fileInfo.path() + "/ref-after-rubber-sheet.osm";
    //  LOG_DEBUG("saving a debug copy to " << combinedOutputPath3 << " ...");

    //move Unknown2 toward Unknown1
    conf().set(RubberSheet::refKey(), true);
   boost::shared_ptr<RubberSheet> rubberSheetOp(new RubberSheet());
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

    // OsmMapPtr combinedMapCopy3(combinedMapCopy2);
    //  MapProjector::reprojectToWgs84(combinedMapCopy3);
    //  OsmUtils::saveMap(combinedMapCopy3, combinedOutputPath3);
  }

  return combinedMap;
}

boost::shared_ptr<MatchComparator> PertyMatchScorer::_conflateAndScoreMatches(
   OsmMapPtr combinedDataToConflate, const QString conflatedMapOutputPath)
{
  LOG_DEBUG("Conflating the reference data with the perturbed data, scoring the matches, and " <<
            "saving the conflated output to: " << conflatedMapOutputPath);

 boost::shared_ptr<MatchComparator> comparator(new MatchComparator());
  //shared_ptr<MatchThreshold> matchThreshold;
  OsmMapPtr conflationCopy(new OsmMap(combinedDataToConflate));

  ConfigOptions configOptions(_settings);
  if (configOptions.getConflateEnableOldRoads())
  {
    // call the old road conflation routine
    Conflator conflator;
    conflator.loadSource(conflationCopy);
    conflator.conflate();
    conflationCopy.reset(new OsmMap(conflator.getBestMap()));
  }

  UnifyingConflator conflator/*(matchThreshold)*/;
  conflator.setConfiguration(_settings);
  conflator.apply(conflationCopy);

  try
  {
    comparator->evaluateMatches(combinedDataToConflate, conflationCopy);
  }
  catch (const HootException&)
  {
    // save map modifies the map so we want to make sure comparator runs first. 'finally' would be
    // nice.
    _saveMap(conflationCopy, conflatedMapOutputPath);
    throw;
  }

  _saveMap(conflationCopy, conflatedMapOutputPath);

  return comparator;
}

void PertyMatchScorer::_saveMap(OsmMapPtr map, QString path)
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
  OsmUtils::saveMap(map, path);
}

}
