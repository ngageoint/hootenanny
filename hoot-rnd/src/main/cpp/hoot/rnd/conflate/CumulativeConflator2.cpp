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
 * @copyright Copyright (C) 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "CumulativeConflator2.h"

// Hoot
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/conflate/Conflator.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/conflate/merging/MergerFactory.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/conflate/matching/MatchFactory.h>
#include <hoot/core/criterion/DualHighwayCriterion.h>
#include <hoot/core/criterion/NotCriterion.h>
#include <hoot/core/conflate/highway/HighwayMatchCreator.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/visitors/SetTagValueVisitor.h>
#include <hoot/core/visitors/KeepTagsVisitor.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>
#include <hoot/core/criterion/StatusCriterion.h>
#include <hoot/core/criterion/OneWayCriterion.h>

namespace hoot
{

CumulativeConflator2::CumulativeConflator2() :
_reverseInputs(false),
_scoreOutput(false),
_isDifferential(false),
_leaveTransferredTags(false),
_runEnsemble(false),
_maxIterations(-1),
_keepIntermediateOutputs(false),
_inputSortScoreType(ScoreType::None)
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
    // attribute conflate to transfer OSM road tags over to our input. We'll use that information
    // to exclude adding any additional divided roads to output beyond what's in the first input.
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

    if (!_isDifferential)
    {
      _conflateTimer.restart();
      LOG_STATUS("******************************************************");
      LOG_STATUS(
        "Conflating (" << i << "/" << numIterations << ") " << input1Info.fileName() <<
        " with " << input2Info.fileName() << " and writing output to " <<
        tempOutputInfo.fileName() << "...");
      Conflator().conflate(input1, input2, tempOutput);
      LOG_STATUS("Conflation took: " << StringUtils::millisecondsToDhms(_conflateTimer.elapsed()));
    }
    else
    {
      throw IllegalArgumentException();
    }

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

CumulativeConflator2::ScoreType CumulativeConflator2::_scoreTypeFromString(QString& scoreTypeStr)
{
  scoreTypeStr = scoreTypeStr.toLower().trimmed();
  if (scoreTypeStr == "raster")
  {
    return ScoreType::Raster;
  }
  else if (scoreTypeStr == "graph")
  {
    return ScoreType::Graph;
  }
  else
  {
    throw IllegalArgumentException("Invalid score type string: " + scoreTypeStr);
  }
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
  // Set the conflate config up to drop all secondary divided roads from input before conflation,
  // which leaves us just with divided roads from the first input.
  conf().prepend(
    ConfigOptions::getConflatePreOpsKey(), QStringList(RemoveElementsVisitor::className()));
/*  conf().set(
    ConfigOptions::getRemoveElementsVisitorElementCriteriaKey(),
    DualHighwayCriterion::className() + ";" + StatusCriterion::className())*/;
  conf().set(
    ConfigOptions::getRemoveElementsVisitorElementCriteriaKey(),
    OneWayCriterion::className() + ";" + StatusCriterion::className());
  conf().set(ConfigOptions::getStatusCriterionStatusKey(), "Unknown2");
  conf().set(ConfigOptions::getRemoveElementsVisitorChainElementCriteriaKey(), true);
  conf().set(ConfigOptions::getRemoveElementsVisitorRecursiveKey(), true);

  LOG_VARD(ConfigOptions().getConflatePreOps());
}

void CumulativeConflator2::_transferTagsToInputs(
  const QDir& input, QStringList& inputs, const QString& output)
{
  QStringList args = _args;
  // TODO: use case specific
  args.replaceInStrings("ReferenceConflation.conf", "AttributeConflation.conf");
  _resetInitConfig(args);
  LOG_VARD(ConfigOptions().getWayJoiner());

  QFileInfo tagInputInfo(_transferTagsInput);
  QFileInfo outputInfo(output);
  QStringList modifiedInputs;
  const int numIterations = _getNumIterations(inputs);
  for (int i = 0; i < numIterations; i++)
  {
    LOG_STATUS("******************************************************");
    LOG_STATUS(
      "Performing tag transfer (" << (i + 1) << "/" << numIterations << ") for " << inputs.at(i) <<
      " from " << tagInputInfo.fileName() << " to " << tagTransferredInput << "...");

    const QString outId =
      StringUtils::padFrontOfNumberStringWithZeroes(i + 1, FILE_NUMBER_PAD_SIZE);
    const QString tagTransferredInput = "in-attribute-" + outId + ".osm";
    QString tagTransferredInputFullPath = outputInfo.path() + "/" + tagTransferredInput;

    _conflateTimer.restart();
    Conflator().conflate(
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
