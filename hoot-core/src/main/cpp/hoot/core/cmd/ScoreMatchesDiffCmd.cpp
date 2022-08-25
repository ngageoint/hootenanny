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
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/conflate/matching/ScoreMatchesDiff.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QElapsedTimer>

namespace hoot
{

class ScoreMatchesDiffCmd : public BaseCommand
{

public:

  static QString className() { return "ScoreMatchesDiffCmd"; }

  ScoreMatchesDiffCmd() = default;

  QString getName() const override { return "score-matches-diff"; }
  QString getDescription() const override
  { return "Compares conflate performance between score-matches outputs (experimental)"; }
  QString getType() const override { return "advanced"; }

  int runSimple(QStringList& args) override
  {
    if (args.size() != 3)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw IllegalArgumentException(
        QString("%1 takes three parameters. You provided %2: %3")
          .arg(getName())
          .arg(args.size())
          .arg(args.join(",")));
    }

    const QString input1 = args[0].trimmed();
    const QString input2 = args[1].trimmed();
    const QString output = args[2].trimmed();

    QElapsedTimer timer;
    timer.start();

    LOG_STATUS(
      "Calculating conflate match difference between ..." << FileUtils::toLogFormat(input1, 25) <<
      " and ..." << FileUtils::toLogFormat(input2, 25) << "; writing output to ..." <<
      FileUtils::toLogFormat(output, 25) << "...");

    ScoreMatchesDiff diffGen;
    diffGen.calculateDiff(input1, input2);
    diffGen.printDiff(output);

    LOG_STATUS("Matches difference calculated in " << StringUtils::millisecondsToDhms(timer.elapsed()) << " total.");

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, ScoreMatchesDiffCmd)

}

