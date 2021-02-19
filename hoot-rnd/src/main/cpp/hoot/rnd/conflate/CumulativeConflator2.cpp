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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */

#include "CumulativeConflator2.h"

// Hoot
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/conflate/ConflateExecutor.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/conflate/merging/MergerFactory.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/conflate/matching/MatchFactory.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/visitors/SetTagValueVisitor.h>
#include <hoot/core/visitors/KeepTagsVisitor.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>
#include <hoot/core/criterion/StatusCriterion.h>
#include <hoot/core/criterion/OneWayCriterion.h>
#include <hoot/rnd/ops/DualHighwayMarker.h>
#include <hoot/core/criterion/TagCriterion.h>
#include <hoot/core/util/ConfigUtils.h>
#include <hoot/rnd/ops/SmallDisconnectedWayRemover.h>
#include <hoot/core/algorithms/extractors/FeatureExtractor.h>
#include <hoot/core/scoring/BaseComparator.h>
#include <hoot/rnd/ops/UnlikelyRoadRemover.h>

namespace hoot
{

CumulativeConflator2::CumulativeConflator2() :
_reverseInputs(false),
_leaveTransferredTags(false),
_maxIterations(-1),
_keepIntermediateOutputs(false)
{
}

void CumulativeConflator2::conflate(const QDir& input, const QString& output)
{
  QDir::SortFlags sortFlags;
  if (!_reverseInputs)
  {
    sortFlags = QDir::Name;
  }
  else
  {
    sortFlags = QDir::Name | QDir::Reversed;
  }
  QStringList inputs = input.entryList(QDir::Files, sortFlags);
  QFileInfo outputInfo(output);

  const bool transferTags = !_transferTagsInput.isEmpty();
  if (transferTags)
  {
    // We're trying to minimize the conflation of divided highways due to their difficulty. Use
    // attribute conflate to transfer OSM road tags over to our input. We'll later use that
    // information to exclude adding any additional divided roads to output beyond what's in the
    // first input.
    _transferTagsToInputs(input, inputs, output);
    _initDropDividedRoadsConfig();
  }

  QString tempOutput;
  QString outId;
  const QString inDir = transferTags ? outputInfo.path() : input.path();
  QString input1 = inDir + "/" + inputs.at(0);
  const int numIterations = _getNumIterations(inputs);
  for (int i = 1; i < numIterations + 1; i++)
  {
    const QString input2 = inDir + "/" + inputs.at(i);
    if (i < numIterations)
    {
      if (outId.isEmpty())
      {
        outId =
          StringUtils::padFrontOfNumberStringWithZeroes(i, FILE_NUMBER_PAD_SIZE) + "-" +
          StringUtils::padFrontOfNumberStringWithZeroes(i + 1, FILE_NUMBER_PAD_SIZE);
      }
      else
      {
        outId =
          StringUtils::padFrontOfNumberStringWithZeroes(
            outId.split("-")[0].toInt(), FILE_NUMBER_PAD_SIZE) +
          "-" + StringUtils::padFrontOfNumberStringWithZeroes(i + 1, FILE_NUMBER_PAD_SIZE);
      }
      tempOutput = outputInfo.path() + "/out-" + outId + ".osm";
    }
    else
    {
      tempOutput = output;
    }
    QFileInfo input1Info(input1);
    QFileInfo input2Info(input2);
    QFileInfo tempOutputInfo(tempOutput);

    _conflateTimer.restart();
    LOG_STATUS("******************************************************");
    LOG_STATUS(
      "Conflating (" << i << "/" << numIterations << ") " << input1Info.fileName() <<
      " with " << input2Info.fileName() << " and writing output to " <<
      tempOutputInfo.fileName() << "...");
    ConflateExecutor().conflate(input1, input2, tempOutput);
    LOG_STATUS("Conflation took: " << StringUtils::millisecondsToDhms(_conflateTimer.elapsed()));

    if (i != 1 && !_keepIntermediateOutputs)
    {
      QFile intermediateOut(input1);
      intermediateOut.remove();
    }

    input1 = tempOutput;

    _resetInitConfig(_args);
    if (transferTags)
    {
      _initDropDividedRoadsConfig();
    }
  }

  if (transferTags && !_leaveTransferredTags)
  {
    _resetInitConfig(_args);  // This gets us back to our initial conflate settings.
    // TODO: hack specific to current use case; generalize
    _removeTransferredTags(output);
  }
}

int CumulativeConflator2::_getNumIterations(const QStringList& inputs) const
{
  int numIterations = inputs.size() - 1;
  if (_maxIterations != -1 && (_maxIterations + 1) < inputs.size())
  {
    numIterations = _maxIterations;
  }
  return numIterations;
}

void CumulativeConflator2::_resetInitConfig(const QStringList& args)
{
  MatchFactory::getInstance().reset();
  MergerFactory::getInstance().reset();
  TagMergerFactory::getInstance().reset();
  conf().clear();
  ConfigOptions::populateDefaults(conf());
  QStringList tempArgs = args;
  Settings::parseCommonArguments(tempArgs);
  conf().set("HOOT_HOME", getenv("HOOT_HOME")); 
  LOG_VARD(ConfigOptions().getMatchCreators());
  LOG_VARD(ConfigOptions().getWayJoiner());
}

void CumulativeConflator2::_initDropDividedRoadsConfig()
{
  // Set the conflate config up to tag all divided roads first. Then, drop all secondary roads
  // tagged as divided from input before conflation. That leaves us just with divided roads from the
  // first input.
  //
  // Tried doing this with an ElementCriterion but ended up with some stability issues, so ended up
  // doing it with a tagging pre-op.

  // Add this to the map cleaner transforms just after the initial op, which reprojects to ortho,
  // since DualHighwayMarker needs the map to be in ortho. Strange that if you try to pass in a
  // pre-built string list here, the list doesn't actually get updated.
  ConfigUtils::insertListOpEntry(
    ConfigOptions::getMapCleanerTransformsKey(), DualHighwayMarker::className(), 1);
  ConfigUtils::insertListOpEntry(
    ConfigOptions::getMapCleanerTransformsKey(), RemoveElementsVisitor::className(), 2);
  conf().set(
    ConfigOptions::getRemoveElementsVisitorElementCriteriaKey(),
    TagCriterion::className() + ";" + StatusCriterion::className());
  conf().set(ConfigOptions::getStatusCriterionStatusKey(), "Unknown2");
  conf().set(ConfigOptions::getRemoveElementsVisitorChainElementCriteriaKey(), true);
  conf().set(ConfigOptions::getRemoveElementsVisitorRecursiveKey(), true);
  conf().set(
    ConfigOptions::getTagCriterionKvpsKey(),
    MetadataTags::HootDualHighway() + "=yes;" + MetadataTags::HootDualHighwayCrossing() + "=yes");

  LOG_VARD(ConfigOptions().getMapCleanerTransforms());
}

void CumulativeConflator2::_transferTagsToInputs(
  const QDir& input, QStringList& inputs, const QString& output)
{
  QStringList args = _args;
  // TODO: use case specific
  args.replaceInStrings("ReferenceConflation.conf", "AttributeConflation.conf");
  _resetInitConfig(args);
  // If this is in the pre ops, its cleaner if we don't use it during AC and just use it during the
  // ref conflate. It will go back into the pre ops for ref conflate when the original config is
  // re-established after AC.
  // TODO: kludgy
  ConfigUtils::removeListOpEntry(
    ConfigOptions::getConflatePreOpsKey(), SmallDisconnectedWayRemover::className());
  ConfigUtils::removeListOpEntry(
    ConfigOptions::getConflatePreOpsKey(), UnlikelyRoadRemover::className());
  LOG_VARD(ConfigOptions().getWayJoiner());
  LOG_VARD(ConfigOptions().getConflatePreOps());

  QFileInfo tagInputInfo(_transferTagsInput);
  QFileInfo outputInfo(output);
  QStringList modifiedInputs;
  const int numIterations = _getNumIterations(inputs);
  for (int i = 0; i < numIterations; i++)
  {
    const QString outId =
      StringUtils::padFrontOfNumberStringWithZeroes(i + 1, FILE_NUMBER_PAD_SIZE);
    const QString tagTransferredInput = "in-tag-transfer-" + outId + ".osm";
    QString tagTransferredInputFullPath = outputInfo.path() + "/" + tagTransferredInput;

    LOG_STATUS("******************************************************");
    LOG_STATUS(
      "Performing tag transfer (" << (i + 1) << "/" << numIterations << ") for " << inputs.at(i) <<
      " from " << tagInputInfo.fileName() << " to " << tagTransferredInput << "...");

    _conflateTimer.restart();
    ConflateExecutor().conflate(
      input.path() + "/" + inputs.at(i), _transferTagsInput, tagTransferredInputFullPath);

    modifiedInputs.append(tagTransferredInput);

    LOG_STATUS("Transfer took: " << StringUtils::millisecondsToDhms(_conflateTimer.elapsed()));
  }
  inputs = modifiedInputs;

  _resetInitConfig(_args);  // This gets us back to our initial conflate settings.
}

void CumulativeConflator2::_removeTransferredTags(const QString& url)
{
  const int maxFilePrintLength = ConfigOptions().getProgressVarPrintLengthMax();
  LOG_STATUS("Removing transferred tags from ..." << url.right(maxFilePrintLength) << "...");

  OsmMapPtr map(new OsmMap());
  OsmMapReaderFactory::read(map, true, true, url);

  SetTagValueVisitor roadUpdater("highway", "road");
  map->visitWaysRw(roadUpdater);

  // TODO: derive this list from the input files or read from config
  QStringList keysToKeep;
  keysToKeep.append("highway");
  keysToKeep.append("hoot:layername");
  keysToKeep.append("inferred_speed_mph");
  keysToKeep.append("length_m");
  keysToKeep.append("source:ingest:datetime");
  keysToKeep.append("travel_time_s");
  KeepTagsVisitor tagPreserver(keysToKeep);
  map->visitWaysRw(tagPreserver);

  OsmMapWriterFactory::write(map, url);
}

}
