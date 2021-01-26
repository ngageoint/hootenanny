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
 * @copyright Copyright (C) 2017, 2018, 2019, 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/conflate/optimization/ConflateTestSettingsOptimizer.h>
#include <hoot/core/conflate/optimization/CaseTestFitnessFunction.h>
#include <hoot/core/conflate/optimization/ReleaseTestFitnessFunction.h>
#include <hoot/core/conflate/optimization/PertyTestFitnessFunction.h>

using namespace std;

namespace hoot
{

class OptimizeConflateTestSettingsCmd : public BaseCommand
{
public:

  static QString className() { return "hoot::OptimizeConflateTestSettingsCmd"; }

  OptimizeConflateTestSettingsCmd() { }

  virtual QString getName() const { return "optimize-network-conf"; }

  virtual QString getDescription() const
  { return "Determines an optimal road conflation configuration for the Network Algorithm"; }

  virtual QString getType() const { return "rnd"; }

  virtual int runSimple(QStringList& args)
  {
    if (args.size() < 6 || args.size() > 7)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes six or seven parameters.").arg(getName()));
    }

    const QString testsDir = args[0];
    const QString testType = args[1].toLower();
    if (testType != "case" && testType != "release" && testType != "perty")
    {
      throw HootException("Invalid test type specified: " + testType);
    }
    const QString testSettingsPath = args[2];
    bool parsedIterations = false;
    const int numIterations = args[3].toInt(&parsedIterations);
    if (!parsedIterations)
    {
      throw HootException("Invalid number of test iterations: " + args[3]);
    }
    const QString summaryFilePath = args[4];
    bool verbose = false;
    if (args[5].toLower() == "true")
    {
      verbose = true;
    }
    QString configFilePath = "";
    if (args.size() == 7)
    {
      configFilePath = args[6];
    }

    std::shared_ptr<AbstractTestFitnessFunction> fitnessFunction;
    if (testType == "case")
    {
      fitnessFunction.reset(new CaseTestFitnessFunction(testsDir, configFilePath));
    }
    else if (testType == "release")
    {
      fitnessFunction.reset(new ReleaseTestFitnessFunction(testsDir, configFilePath));
    }
    else
    {
      fitnessFunction.reset(new PertyTestFitnessFunction(testsDir, configFilePath));
    }

    ConflateTestSettingsOptimizer(verbose).runOptimization(
      fitnessFunction, numIterations, testSettingsPath, summaryFilePath);

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, OptimizeConflateTestSettingsCmd)

}

