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

// Qt
#include <QElapsedTimer>

namespace hoot
{

CumulativeConflator2::CumulativeConflator2() :
_reverseInputs(false),
_dropSecondaryReviewElements(false),
_scoreOutput(false),
_isDifferential(false),
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
  const QStringList inputs = input.entryList(QDir::Files, sortFlags);

//  if (!_addTagsInput.isEmpty())
//  {

//  }

//  if (_inputSortScoreType != ScoreType::None)
//  {

//  }

  QFileInfo outputInfo(output);

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
//    else
//    {

//    }

//    if (_dropSecondaryReviewElements)
//    {

//    }

//    if (_runEnsemble)
//    {

//    }

//    if (_scoreOutput)
//    {

//    }

    if (i != 1 && !_keepIntermediateOutputs)
    {
      QFile intermediateOut(input1);
      intermediateOut.remove();
    }

    input1 = tempOutput;

    _resetInitConfig();
  }
}

void CumulativeConflator2::_resetInitConfig()
{
  MatchFactory::getInstance().reset();
  MergerFactory::getInstance().reset();
  TagMergerFactory::getInstance().reset();
  conf().clear();
  ConfigOptions::populateDefaults(conf());
  QStringList tempArgs = _args;
  Settings::parseCommonArguments(tempArgs);
  conf().set("HOOT_HOME", getenv("HOOT_HOME"));
}

CumulativeConflator2::ScoreType CumulativeConflator2::scoreTypeFromString(QString& scoreTypeStr)
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

}
