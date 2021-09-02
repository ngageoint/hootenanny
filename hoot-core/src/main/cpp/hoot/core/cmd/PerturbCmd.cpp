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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/algorithms/perty/PertyOp.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/algorithms/perty/PertyMatchScorer.h>
#include <hoot/core/scoring/MapMatchScoringUtils.h>
#include <hoot/core/algorithms/perty/PertyTestRunner.h>
#include <hoot/core/algorithms/perty/PertyTestRunResult.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QElapsedTimer>

using namespace std;

namespace hoot
{

/**
 * @see PertyOp
 */
class PerturbCmd : public BaseCommand
{
public:

  static QString className() { return "PerturbCmd"; }

  PerturbCmd() = default;

  QString getName() const override { return "perturb"; }
  QString getDescription() const override { return "Perturbs features in a map"; }

  int runSimple(QStringList& args) override
  {
    QElapsedTimer timer;
    timer.start();

    LOG_VARD(args.size());
    if (args.size() < 2 || args.size() > 3)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw IllegalArgumentException(
        QString("%1 takes at two to three parameters. You provided %2: %3")
          .arg(getName())
          .arg(args.size())
          .arg(args.join(",")));
    }

    bool scoreOptionSpecified = false;
    if (args.contains("--score"))
    {
      args.removeAt(args.indexOf("--score"));
      scoreOptionSpecified = true;
    }

    bool testOptionSpecified = false;
    if (args.contains("--test"))
    {
      args.removeAt(args.indexOf("--test"));
      testOptionSpecified = true;
    }

    assert(args.size() == 2);
    if (scoreOptionSpecified && testOptionSpecified)
    {
      throw HootException("Cannot specify both the --score and --test options.");
    }

    const QString input = args[0];
    const QString output = args[1];

    LOG_STATUS(
      "Perturbing map ..." << FileUtils::toLogFormat(input, 25) << " and writing output to ..." <<
      FileUtils::toLogFormat(output, 25) << "...");

    const QString msg = "Perturbation operation ran in %1 total.";
    if (!scoreOptionSpecified && !testOptionSpecified)
    {
      OsmMapPtr map = std::make_shared<OsmMap>();
      IoUtils::loadMap(map, input, true, Status::Unknown1);

      PertyOp perty;
      perty.apply(map);

      MapProjector::projectToWgs84(map);
      IoUtils::saveMap(map, output);

      LOG_STATUS(msg.arg(StringUtils::millisecondsToDhms(timer.elapsed())));
    }
    else if (scoreOptionSpecified)
    {
      std::shared_ptr<const MatchComparator> matchComparator =
        PertyMatchScorer().scoreMatches(input, output);
      LOG_STATUS(msg.arg(StringUtils::millisecondsToDhms(timer.elapsed())));
      cout << MapMatchScoringUtils::getMatchScoringString(matchComparator);
    }
    else if (testOptionSpecified)
    {
      QList<std::shared_ptr<const PertyTestRunResult>> results =
      PertyTestRunner().runTest(input, output);

      LOG_STATUS("\n\nPERTY Test Results");
      LOG_STATUS("\n\nNumber of Test Runs: " << results.size());
      bool anyTestFailed = false;
      bool anyTestRunPassedWithScoreOutsideOfAllowedVarianceAndHigherThanExpected = false;
      for (QList<std::shared_ptr<const PertyTestRunResult>>::const_iterator it = results.begin();
           it != results.end(); ++it)
      {
        std::shared_ptr<const PertyTestRunResult> result = *it;
        LOG_STATUS(result->toString());
        anyTestFailed = !result->testPassed();
        // Just checking here for test run scores that were higher than expected but allowed to
        // pass. still want to log a warning about it so that the expected scores can eventually be
        // updated in regressions tests
        if (!result->getFailOnBetterScore() && result->testPassed() &&
            (result->getScoreVariance() > result->getAllowedScoreVariance()))
        {
          assert(result->getScore() > result->getExpectedScore());
          anyTestRunPassedWithScoreOutsideOfAllowedVarianceAndHigherThanExpected = true;
        }
      }

      if (anyTestRunPassedWithScoreOutsideOfAllowedVarianceAndHigherThanExpected)
      {
        LOG_WARN(
          "At least one PERTY test run had a allowable passing score that was greater than the " <<
          "expected score variance and higher than its expected score.");
      }

      if (anyTestFailed)
      {
        LOG_ERROR("At least one PERTY test run failed the score variance check.");
        return -1;
      }

       LOG_STATUS(msg.arg(StringUtils::millisecondsToDhms(timer.elapsed())));
    }

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, PerturbCmd)

}

