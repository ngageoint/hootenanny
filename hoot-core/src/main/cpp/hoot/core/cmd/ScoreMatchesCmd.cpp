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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/conflate/UnifyingConflator.h>
#include <hoot/core/ops/BuildingOutlineUpdateOp.h>
#include <hoot/core/ops/OpExecutor.h>
#include <hoot/core/scoring/MatchComparator.h>
#include <hoot/core/scoring/MatchScoringMapPreparer.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/visitors/CountManualMatchesVisitor.h>
#include <hoot/core/util/ConfigUtils.h>
#include <hoot/core/ops/ManualMatchValidator.h>

// tgs
#include <tgs/Optimization/NelderMead.h>

// Qt
#include <QElapsedTimer>
#include <QFileInfo>

using namespace std;
using namespace Tgs;

namespace hoot
{

class ScoreMatchesCmd : public BaseCommand
{

public:

  static QString className() { return "hoot::ScoreMatchesCmd"; }

  ScoreMatchesCmd() = default;

  virtual QString getName() const override { return "score-matches"; }

  virtual QString getDescription() const override
  { return "Scores conflation performance against a manually matched map"; }

  virtual int runSimple(QStringList& args) override
  {
    QElapsedTimer timer;
    timer.start();

    bool showConfusion = false;
    if (args.contains("--confusion"))
    {
      args.removeAll("--confusion");
      showConfusion = true;
    }
    bool optimizeThresholds = false;
    if (args.contains("--optimize"))
    {
      args.removeAll("--optimize");
      optimizeThresholds = true;
    }
    bool validateManualMatches = true;
    if (args.contains("--validation-off"))
    {
      args.removeAll("--validation-off");
      validateManualMatches = false;
    }

    if (args.size() < 3 || args.size() % 2 != 1)
    {
      LOG_VAR(args);
      cout << getHelp() << endl << endl;
      throw HootException(
        QString("%1 takes at least three parameters: two or more input maps (even number) and an output map")
          .arg(getName()));
    }

    ConfigUtils::checkForTagValueTruncationOverride();
    QStringList allOps = ConfigOptions().getConflatePreOps();
    allOps += ConfigOptions().getConflatePostOps();
    ConfigUtils::checkForDuplicateElementCorrectionMismatch(allOps);

    vector<OsmMapPtr> maps;
    QString output = args.last();
    for (int i = 0; i < args.size() - 1; i += 2)
    {
      OsmMapPtr map(new OsmMap());
      const QString map1Path = args[i];
      const QString map2Path = args[i + 1];
      IoUtils::loadMap(map, map1Path, false, Status::Unknown1);
      IoUtils::loadMap(map, map2Path, false, Status::Unknown2);

      // If any of the map files have errors, we'll print some out and terminate.
      if (validateManualMatches && _validateMatches(map, map1Path, map2Path))
      {
        return 1;
      }

      MatchScoringMapPreparer().prepMap(map, ConfigOptions().getScoreMatchesRemoveNodes());
      maps.push_back(map);
    }
    LOG_VARD(maps.size());

    if (optimizeThresholds)
    {
      _optimize(maps, showConfusion);
    }
    else
    {
      double score;
      std::shared_ptr<MatchThreshold> mt;
      const QString result = evaluateThreshold(maps, output, mt, showConfusion, score);
      cout << result;
    }

    LOG_STATUS(
      "Match scoring ran in " << StringUtils::millisecondsToDhms(timer.elapsed()) << " total.");

    return 0;
  }

  class ScoreFunction : public Tgs::NelderMead::Function
  {
  public:

    virtual double f(Tgs::Vector v)
    {
      double score;
      std::shared_ptr<MatchThreshold> mt(new MatchThreshold(v[0], v[1], v[2]));
      _cmd->evaluateThreshold(_maps, "", mt, _showConfusion, score);
      return score;
    }

    ScoreMatchesCmd* _cmd;
    vector<OsmMapPtr> _maps;
    bool _showConfusion;
  };

  QString evaluateThreshold(vector<OsmMapPtr> maps, QString output,
                            std::shared_ptr<MatchThreshold> mt, bool showConfusion,
                            double& score)
  {
    MatchComparator comparator;

    QString result;

    long numManualMatches = 0;
    for (size_t i = 0; i < maps.size(); i++)
    {
      OsmMapPtr copy(new OsmMap(maps[i]));

      std::shared_ptr<CountManualMatchesVisitor> manualMatchVisitor(
        new CountManualMatchesVisitor());
      maps[i]->visitRo(*manualMatchVisitor);
      numManualMatches += manualMatchVisitor->getStat();
      LOG_VARD(numManualMatches);

      LOG_INFO("Applying pre conflation operations...");
      LOG_VART(ConfigOptions().getConflatePreOps());
      OpExecutor(ConfigOptions().getConflatePreOps()).apply(copy);
      UnifyingConflator(mt).apply(copy);
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
      if (mt)
      {
        cout << "Threshold: " << mt->toString() << endl;
      }
      cout << comparator.toString();
      cout << QString("number of manual matches made: %1\n").arg(numManualMatches) << endl;
    }
    QString line = QString("%1,%2,%3,%4\n").arg(-1)
        .arg(comparator.getPercentCorrect())
        .arg(comparator.getPercentWrong())
        .arg(comparator.getPercentUnnecessaryReview());
    result += line;

    score = -comparator.getPercentWrong() * 5 - comparator.getPercentUnnecessaryReview();
    cout << "Score: " << score << endl;

    return result;
  }

private:

  bool _validateMatches(const OsmMapPtr& map, const QString& map1Path, const QString& map2Path)
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

  void _printIssues(const QMap<ElementId, QString>& issues, const QString& type,
                    const QString& map1Path, const QString& map2Path)
  {
    if (!issues.isEmpty())
    {
      QFileInfo fileInfo1(map1Path);
      QFileInfo fileInfo2(map2Path);
      cout << "There are " << StringUtils::formatLargeNumber(issues.size()) <<
              " manual match " << type << " for inputs " <<
              fileInfo1.completeBaseName().right(30) <<
              " and " << fileInfo2.completeBaseName().right(30) << ":\n\n";
      int issueCount = 0;
      for (QMap<ElementId, QString>::const_iterator itr = issues.begin();
           itr != issues.end(); ++itr)
      {
        cout << itr.key().toString() + ": " + itr.value() + "\n";

        issueCount++;
        if (issueCount >= 10)
        {
          cout << "Printing " << type << " for the first 10 elements only..." << endl;
          break;
        }
      }
    }
  }

  void _optimize(vector<OsmMapPtr> maps, bool showConfusion)
  {
    ScoreFunction* sf = new ScoreFunction();
    sf->_cmd = this;
    sf->_maps = maps;
    sf->_showConfusion = showConfusion;

    NelderMead optimizer(3, sf, 0.0001);

    Vector result;

    result = Vector(.01, .01, 0.01);
    optimizer.step(result, sf->f(result));
    result = Vector(0, 1, 0.01);
    optimizer.step(result, sf->f(result));
    result = Vector(.22, 1, .33);
    optimizer.step(result, sf->f(result));
    result = Vector(0, 0.8, 0.01);
    optimizer.step(result, sf->f(result));

    while (optimizer.done() == false)
    {
      result = optimizer.step(result, sf->f(result));
      result[0] = min(1., max(0., result[0]));
      result[1] = min(1., max(0., result[1]));
      result[2] = min(1., max(0., result[2]));
      LOG_VAR(result.getVector());
    }
  }
};

HOOT_FACTORY_REGISTER(Command, ScoreMatchesCmd)

}

