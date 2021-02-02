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

// Qt
#include <QElapsedTimer>

namespace hoot
{

CumulativeConflator2::CumulativeConflator2() :
_reverseInputs(false),
_scoreOutput(false),
_isDifferential(false),
_leaveAddedTags(false),
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

  bool conflateDividedRoadsOnlyOnce = !_addTagsInput.isEmpty();
  if (conflateDividedRoadsOnlyOnce)
  {
    _transferTagsToFirstInput(input, inputs, QDir(outputInfo.path()));
    //return;
  }

  QString tempOutput;
  QString outId;
  const int padSize = 3;
  QString input1 = input.path() + "/" + inputs.at(0);
  int numIterations = inputs.size() - 1;
  if (_maxIterations != -1 && (_maxIterations + 1) < inputs.size())
  {
    numIterations = _maxIterations;
  }

  QElapsedTimer conflateTimer;
  Conflator conflator;
  for (int i = 1; i < numIterations + 1; i++)
  {
    const QString input2 = input.path() + "/" + inputs.at(i);
    if (i < numIterations)
    {
      if (outId.isEmpty())
      {
        outId =
          StringUtils::padFrontOfNumberStringWithZeroes(i, padSize) + "-" +
          StringUtils::padFrontOfNumberStringWithZeroes(i + 1, padSize);
      }
      else
      {
        outId =
          StringUtils::padFrontOfNumberStringWithZeroes(outId.split("-")[0].toInt(), padSize) +
          "-" + StringUtils::padFrontOfNumberStringWithZeroes(i + 1, padSize);
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

    if (conflateDividedRoadsOnlyOnce)
    {
      // Create a conflate filter to prevent conflating all div roads, so we'll just end up with div
      // roads from the first input. This must be set for each loop iteration, since we're
      // rebuilding the config for each conflation.
      conf().set(
        ConfigOptions::getConflateElementCriterionKey(), DualHighwayCriterion::className());
      conf().set(ConfigOptions::getConflateElementCriterionNegateKey(), true);
    }

    if (!_isDifferential)
    {
      conflateTimer.restart();
      LOG_STATUS("******************************************************");
      LOG_STATUS(
        "Conflating (" << i << "/" << numIterations << ") " << input1Info.fileName() <<
        " with " << input2Info.fileName() << " and writing output to " <<
        tempOutputInfo.fileName() << "...");
      conflator.conflate(input1, input2, tempOutput);
      LOG_STATUS("Conflation took: " << StringUtils::millisecondsToDhms(conflateTimer.elapsed()));
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
  }

  if (conflateDividedRoadsOnlyOnce && !_leaveAddedTags)
  {
    // TODO: hack specific to current use case; generalize
    _removeAttributeAddedTags(output);
  }
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
}

void CumulativeConflator2::_transferTagsToFirstInput(
  const QDir& inputDir, QStringList& inputs, const QDir& output)
{
  // We're trying to minimize the conflation of divided highways due to their difficulty. Use
  // attribute conflate to transfer OSM road tags over to our first input.

  QFileInfo tagInputInfo(_addTagsInput);
  QString attributeConflatedOut = output.absolutePath() + "/out-attribute.osm";
  QFileInfo outInfo(attributeConflatedOut);
  LOG_STATUS(
    "Performing tag transfer step for " << inputs.at(0) << " and " << tagInputInfo.fileName() <<
    "; writing output to " << outInfo.fileName() << "...");

  QStringList args = _args;
  args.replaceInStrings("ReferenceConflation.conf", "AttributeConflation.conf");
  _resetInitConfig(args);
  LOG_VARD(ConfigOptions().getWayJoiner());
  Conflator().conflate(inputDir.path() + "/" + inputs.at(0), _addTagsInput, attributeConflatedOut);

  // Modify the location of the first input to be our conflated file with tags added.
  inputs[0] = attributeConflatedOut;

  _resetInitConfig(_args);  // This gets us back to our initial conflate settings.
}

void CumulativeConflator2::_removeAttributeAddedTags(const QString& url)
{
  OsmMapPtr map(new OsmMap());
  OsmMapReaderFactory::read(map, true, true, url);

  SetTagValueVisitor roadUpdater("highway", "road");
  map->visitWaysRw(roadUpdater);

  // TODO: derive this list from the input files or read from config
  QStringList keysToKeep;
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
