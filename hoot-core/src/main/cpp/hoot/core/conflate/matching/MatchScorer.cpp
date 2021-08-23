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
#include "MatchScorer.h"

// hoot
#include <hoot/core/conflate/UnifyingConflator.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/ops/BuildingOutlineUpdateOp.h>
#include <hoot/core/ops/ManualMatchValidator.h>
#include <hoot/core/ops/OpExecutor.h>
#include <hoot/core/scoring/MatchComparator.h>
#include <hoot/core/scoring/MatchScoringMapPreparer.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ConfigUtils.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/visitors/CountManualMatchesVisitor.h>

// Qt
#include <QElapsedTimer>

namespace hoot
{

class ScoreFunction : public Tgs::NelderMead::Function
{
public:

  double f(Tgs::Vector v) override
  {
    double score;
    std::shared_ptr<MatchThreshold> mt = std::make_shared<MatchThreshold>(v[0], v[1], v[2], false);
    _scorer->evaluateThreshold(_maps, "", mt, _showConfusion, score);
    return score;
  }

  MatchScorer* _scorer;
  std::vector<OsmMapPtr> _maps;
  bool _showConfusion;
};

MatchScorer::MatchScorer() :
_printConfusion(false),
_optimizeThresholds(false),
_validateManualMatches(true)
{
}

void MatchScorer::score(
  const QStringList& ref1Inputs, const QStringList& ref2Inputs, const QString output)
{
  if (_optimizeThresholds && !output.isEmpty())
  {
    throw IllegalArgumentException("Output path not valid when threshold optimization is enabled.");
  }
  if (ref1Inputs.size() != ref2Inputs.size())
  {
    throw IllegalArgumentException(
      "The number of REF1 inputs: " + QString::number(ref1Inputs.size()) +
      " does not equal the number of REF2 inputs: " + QString::number(ref2Inputs.size()) + ".");
  }

  QElapsedTimer timer;
  timer.start();

  ConfigUtils::checkForTagValueTruncationOverride();
  QStringList allOps = ConfigOptions().getConflatePreOps();
  allOps += ConfigOptions().getConflatePostOps();
  ConfigUtils::checkForDuplicateElementCorrectionMismatch(allOps);

  std::vector<OsmMapPtr> maps;
  for (int i = 0; i < ref1Inputs.size(); i++)
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    const QString map1Path = ref1Inputs.at(i);
    const QString map2Path = ref2Inputs.at(i);
    LOG_VART(map1Path);
    LOG_VART(map2Path);

    LOG_STATUS(
      "Scoring matches for ..." << FileUtils::toLogFormat(map1Path, 25) << " and ..." <<
      FileUtils::toLogFormat(map2Path, 25) << "...");

    IoUtils::loadMap(map, map1Path, false, Status::Unknown1);
    IoUtils::loadMap(map, map2Path, false, Status::Unknown2);

    // If any of the map files have errors, we'll print some out and terminate.
    if (_validateManualMatches && _validateMatches(map, map1Path, map2Path))
    {
      return;
    }

    MatchScoringMapPreparer().prepMap(map, ConfigOptions().getScoreMatchesRemoveNodes());
    maps.push_back(map);
  }
  LOG_VARD(maps.size());

  if (_optimizeThresholds)
  {
    _optimize(maps, _printConfusion);
  }
  else
  {
    double score;
    // We *do not* want to init the match threshold here, as that will send us down the wrong code
    // path. Found that out the hard way.
    std::shared_ptr<MatchThreshold> mt;
    const QString result = evaluateThreshold(maps, output, mt, _printConfusion, score);
    std::cout << result;
  }

  LOG_STATUS(
    "Match scoring ran in " << StringUtils::millisecondsToDhms(timer.elapsed()) << " total.");
}

QString MatchScorer::evaluateThreshold(
  const std::vector<OsmMapPtr>& maps, const QString& output,
  const std::shared_ptr<MatchThreshold>& matchThreshold,
  const bool showConfusion, double& score) const
{
  MatchComparator comparator;

  QString result;

  long numManualMatches = 0;
  for (size_t i = 0; i < maps.size(); i++)
  {
    OsmMapPtr copy = std::make_shared<OsmMap>(maps[i]);

    std::shared_ptr<CountManualMatchesVisitor> manualMatchVisitor =
      std::make_shared<CountManualMatchesVisitor>();

    maps[i]->visitRo(*manualMatchVisitor);
    numManualMatches += (long)manualMatchVisitor->getStat();
    LOG_VARD(numManualMatches);

    LOG_INFO("Applying pre conflation operations...");
    LOG_VART(ConfigOptions().getConflatePreOps());
    OpExecutor(ConfigOptions().getConflatePreOps()).apply(copy);
    UnifyingConflator(matchThreshold).apply(copy);
    LOG_INFO("Applying post conflation operations...");
    OpExecutor(ConfigOptions().getConflatePostOps()).apply(copy);

    comparator.evaluateMatches(maps[i], copy);

    if (i == 0 && !output.isEmpty())
    {
      BuildingOutlineUpdateOp().apply(copy);
      MapProjector::projectToWgs84(copy);
      IoUtils::saveMap(copy, output);
    }
  }

  LOG_VARD(showConfusion);
  if (showConfusion)
  {
    if (matchThreshold)
    {
      std::cout << "Threshold: " << matchThreshold->toString() << std::endl;
    }
    std::cout << comparator.toString();
    std::cout << QString("number of manual matches made: %1\n").arg(numManualMatches) << std::endl;
  }
  QString line =
      QString("%1,%2,%3,%4\n").arg(-1)
      .arg(comparator.getPercentCorrect())
      .arg(comparator.getPercentWrong())
      .arg(comparator.getPercentUnnecessaryReview());
  result += line;

  score = -comparator.getPercentWrong() * 5 - comparator.getPercentUnnecessaryReview();
  std::cout << "Score: " << score << std::endl;

  return result;
}

bool MatchScorer::_validateMatches(
  const OsmMapPtr& map, const QString& map1Path, const QString& map2Path) const
{
  QElapsedTimer timer;
  timer.start();
  LOG_INFO("Validating manual matches...");

  ManualMatchValidator inputValidator;
  inputValidator.setRequireRef1(ConfigOptions().getScoreMatchesRequireRef1());
  inputValidator.setAllowUuidManualMatchIds(ConfigOptions().getScoreMatchesAllowUuidsAsIds());
  inputValidator.setFullDebugOutput(ConfigOptions().getScoreMatchesFullDebugOutput());
  inputValidator.getInitStatusMessage();
  inputValidator.apply(map);
  inputValidator.getCompletedStatusMessage();

  LOG_INFO("Validated manual matches in: " << StringUtils::millisecondsToDhms(timer.elapsed()));

  _printIssues(inputValidator.getWarnings(), "warnings", map1Path, map2Path);
  _printIssues(inputValidator.getErrors(), "errors", map1Path, map2Path);

  return !inputValidator.getErrors().isEmpty();
}

void MatchScorer::_printIssues(
  const QMap<ElementId, QString>& issues, const QString& type, const QString& map1Path,
  const QString& map2Path) const
{
  if (!issues.isEmpty())
  {
    QFileInfo fileInfo1(map1Path);
    QFileInfo fileInfo2(map2Path);
    std::cout << "There are " << StringUtils::formatLargeNumber(issues.size()) <<
                 " manual match " << type << " for inputs " <<
                 FileUtils::toLogFormat(fileInfo1.completeBaseName(), 30) <<
                 " and " << FileUtils::toLogFormat(fileInfo2.completeBaseName(), 30) << ":\n\n";
    int issueCount = 0;
    for (QMap<ElementId, QString>::const_iterator itr = issues.begin(); itr != issues.end(); ++itr)
    {
      std::cout << itr.key().toString() + ": " + itr.value() + "\n";

      issueCount++;
      if (issueCount >= 10)
      {
        std::cout << "Printing " << type << " for the first 10 elements only..." << std::endl;
        break;
      }
    }
  }
}

void MatchScorer::_optimize(const std::vector<OsmMapPtr>& maps, const bool showConfusion)
{
  std::shared_ptr<ScoreFunction> sf = std::make_shared<ScoreFunction>();
  sf->_scorer = this;
  sf->_maps = maps;
  sf->_showConfusion = showConfusion;

  Tgs::NelderMead optimizer(3, sf, 0.0001);

  Tgs::Vector result;

  result = Tgs::Vector(.01, .01, 0.01);
  optimizer.step(result, sf->f(result));
  result = Tgs::Vector(0, 1, 0.01);
  optimizer.step(result, sf->f(result));
  result = Tgs::Vector(.22, 1, .33);
  optimizer.step(result, sf->f(result));
  result = Tgs::Vector(0, 0.8, 0.01);
  optimizer.step(result, sf->f(result));

  while (optimizer.done() == false)
  {
    result = optimizer.step(result, sf->f(result));
    result[0] = std::min(1., std::max(0., result[0]));
    result[1] = std::min(1., std::max(0., result[1]));
    result[2] = std::min(1., std::max(0., result[2]));
    LOG_VAR(result.getVector());
  }
}

}
