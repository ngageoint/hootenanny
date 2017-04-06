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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/perty/PertyTestRunner.h>
#include <hoot/core/perty/PertyTestRunResult.h>

// Qt
#include <QFileInfo>
#include <QDir>

namespace hoot
{

class PertyTestCmd : public BaseCommand
{

  public:

    static string className() { return "hoot::PertyTestCmd"; }

    PertyTestCmd() { }

    virtual QString getName() const { return "perty-test"; }

    virtual int runSimple(QStringList args)
    {
      if (args.size() != 2)
      {
        cout << getHelp() << endl << endl;
        throw HootException(QString("%1 takes two parameters.").arg(getName()));
      }

      QList<boost::shared_ptr<const PertyTestRunResult> > results =
        PertyTestRunner().runTest(args[0], args[1]);

      LOG_INFO("\n\nPERTY Test Results");
      LOG_INFO("\n\nNumber of Test Runs: " << results.size());
      bool anyTestFailed = false;
      bool anyTestRunPassedWithScoreOutsideOfAllowedVarianceAndHigherThanExpected = false;
      for (QList<boost::shared_ptr<const PertyTestRunResult> >::const_iterator it = results.begin();
           it != results.end(); it++)
      {
        boost::shared_ptr<const PertyTestRunResult> result = *it;
        LOG_INFO(result->toString());
        anyTestFailed = !result->testPassed();
        //just checking here for test run scores that were higher than expected but allowed to pass;
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
          "expected score variance and higher than its expected score.")
      }

      if (anyTestFailed)
      {
        LOG_ERROR("At least one PERTY test run failed the score variance check.");
        return -1;
      }

      return 0;
    }
};

HOOT_FACTORY_REGISTER(Command, PertyTestCmd)

}
