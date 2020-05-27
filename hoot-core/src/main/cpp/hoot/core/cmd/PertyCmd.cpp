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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/algorithms/perty/PertyOp.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/algorithms/perty/PertyMatchScorer.h>
#include <hoot/core/scoring/MapMatchScoringUtils.h>
#include <hoot/core/algorithms/perty/PertyTestRunner.h>
#include <hoot/core/algorithms/perty/PertyTestRunResult.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QElapsedTimer>

using namespace std;

namespace hoot
{

class PertyCmd : public BaseCommand
{
public:

  static string className() { return "hoot::PertyCmd"; }

  PertyCmd() { }

  virtual QString getName() const override { return "perty"; }

  virtual QString getDescription() const override { return "Perturbs a map using PERTY"; }

  virtual int runSimple(QStringList& args) override
  {
    QElapsedTimer timer;
    timer.start();

    LOG_VARD(args.size());
    if (args.size() < 2 || args.size() > 3)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes two or three parameters.").arg(getName()));
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

    const QString msg = "PERTY operation ran in %1 total.";
    if (!scoreOptionSpecified && !testOptionSpecified)
    {
      OsmMapPtr map(new OsmMap());
      IoUtils::loadMap(map, args[0], true, Status::Unknown1);

      PertyOp perty;
      perty.apply(map);

      MapProjector::projectToWgs84(map);
      IoUtils::saveMap(map, args[1]);

      LOG_STATUS(msg.arg(StringUtils::millisecondsToDhms(timer.elapsed())));
    }
    else if (scoreOptionSpecified)
    {
      std::shared_ptr<const MatchComparator> matchComparator =
        PertyMatchScorer().scoreMatches(args[0], args[1]);
      LOG_STATUS(msg.arg(StringUtils::millisecondsToDhms(timer.elapsed())));
      cout << MapMatchScoringUtils::getMatchScoringString(matchComparator);
    }
    else if (testOptionSpecified)
    {
      QList<std::shared_ptr<const PertyTestRunResult>> results =
      PertyTestRunner().runTest(args[0], args[1]);

      LOG_INFO("\n\nPERTY Test Results");
      LOG_INFO("\n\nNumber of Test Runs: " << results.size());
      bool anyTestFailed = false;
      bool anyTestRunPassedWithScoreOutsideOfAllowedVarianceAndHigherThanExpected = false;
      for (QList<std::shared_ptr<const PertyTestRunResult>>::const_iterator it = results.begin();
           it != results.end(); ++it)
      {
        std::shared_ptr<const PertyTestRunResult> result = *it;
        LOG_INFO(result->toString());
        anyTestFailed = !result->testPassed();
        //Just checking here for test run scores that were higher than expected but allowed to pass.
        //still want to log a warning about it so that the expected scores can eventually be updated
        //in regressions tests
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

HOOT_FACTORY_REGISTER(Command, PertyCmd)

}

