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
 * @copyright Copyright (C) 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>

#include <hoot/test/cmd/TestCommand.h>
#include <hoot/test/conflate/optimization/ConflateTestSettingsOptimizer.h>
#include <hoot/test/conflate/optimization/CaseTestFitnessFunction.h>
#include <hoot/test/conflate/optimization/ReleaseTestFitnessFunction.h>
#include <hoot/test/conflate/optimization/PertyTestFitnessFunction.h>

namespace hoot
{

class OptimizeConflateTestSettingsCmd : public TestCommand
{
public:

  static QString className() { return "OptimizeConflateTestSettingsCmd"; }

  OptimizeConflateTestSettingsCmd() { }

  QString getName() const override { return "optimize-network-conf"; }
  QString getDescription() const override
  { return "Determines an optimal configuration for the Network conflation algorithm"; }

  virtual int runSimple(QStringList& args)
  {
    if (args.size() < 6 || args.size() > 7)
    {
      const std::string help = getHelp().toStdString();
      std::cout << help << std::endl << std::endl;
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
      fitnessFunction = std::make_shared<CaseTestFitnessFunction>(testsDir, configFilePath);
    }
    else if (testType == "release")
    {
      fitnessFunction = std::make_shared<ReleaseTestFitnessFunction>(testsDir, configFilePath);
    }
    else
    {
      fitnessFunction = std::make_shared<PertyTestFitnessFunction>(testsDir, configFilePath);
    }

    ConflateTestSettingsOptimizer(verbose).runOptimization(
      fitnessFunction, numIterations, testSettingsPath, summaryFilePath);

    return 0;
  }
};

HOOT_FACTORY_REGISTER(TestCommand, OptimizeConflateTestSettingsCmd)

}

