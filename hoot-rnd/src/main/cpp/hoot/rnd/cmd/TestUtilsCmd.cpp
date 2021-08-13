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

// Hoot
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/FileUtils.h>

// Qt
#include <QSet>

namespace hoot
{

/**
 * @brief The TestUtilsCmd class contains utilities for debugging Hootenanny test behavior.
 *
 * See command documentation for details.
 */
class TestUtilsCmd : public BaseCommand
{
public:

  static QString className() { return "hoot::TestUtilsCmd"; }

  TestUtilsCmd() = default;

  QString getName() const override { return "test-utils"; }
  QString getDescription() const override { return "Various utilities for debugging tests."; }
  QString getType() const override { return "rnd"; }

  int runSimple(QStringList& args) override
  {  
    if (!args.contains("--log"))
    {
      throw IllegalArgumentException(
        "The only available command option is --log, which must be specified.");
    }

    const int testLogArgIndex = args.indexOf("--log");
    const QString testLog = args[testLogArgIndex + 1];
    args.removeAt(testLogArgIndex + 1);
    args.removeAll("--log");
    if (testLog.isEmpty())
    {
      throw IllegalArgumentException("No test log specified.");
    }

    const QStringList testLines = FileUtils::readFileToList(testLog, true);
    LOG_VARD(testLines);
    QSet<QString> launchedJobs;
    QSet<QString> completedJobs;
    const QString launchedJobText = "launched test";
    const QString completedJobText = "test completed";
    for (int i = 0; i < testLines.length(); i++)
    {
      const QString line = testLines.at(i);
      QStringList tokens;
      if (line.contains(launchedJobText))
      {
        tokens = line.split(" ");
        launchedJobs.insert(tokens[tokens.size() - 1]);
      }
      else if (line.contains(completedJobText))
      {
        tokens = line.split(" ");
        completedJobs.insert(tokens[tokens.size() - 1]);
      }
    }
    LOG_VARD(launchedJobs.size());
    LOG_VARD(completedJobs.size());

    QSet<QString> unfinishedTests = launchedJobs.subtract(completedJobs);
    if (!unfinishedTests.empty())
    {
      LOG_STATUS("Unfinished tests in ..." << testLog.right(25) << ":\n" << unfinishedTests);
    }
    else
    {
      LOG_STATUS("There are no unfinished tests in ..." << testLog.right(25) << ".");
    }

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, TestUtilsCmd)

}
