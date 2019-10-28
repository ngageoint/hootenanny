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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/rnd/conflate/matching/ScoreMatchesDiff.h>

namespace hoot
{

class ScoreMatchesDiffCmd : public BaseCommand
{

public:

  static std::string className() { return "hoot::ScoreMatchesDiffCmd"; }

  ScoreMatchesDiffCmd()
  {
  }

  virtual QString getName() const override { return "score-matches-diff"; }

  virtual QString getType() const override { return "rnd"; }

  virtual QString getDescription() const override
  { return "Compares conflation performance between files output by score-matches (experimental)"; }

  virtual int runSimple(QStringList& args) override
  {
    if (args.size() != 3)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw HootException(QString("%1 takes three parameters.").arg(getName()));
    }

    try
    {
      ScoreMatchesDiff diffGen;
      diffGen.calculateDiff(args[0].trimmed(), args[1].trimmed());
      diffGen.printDiff(args[2].trimmed());
    }
    catch (const HootException& e)
    {
      LOG_ERROR(e.what());
      return 1;
    }

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, ScoreMatchesDiffCmd)

}

