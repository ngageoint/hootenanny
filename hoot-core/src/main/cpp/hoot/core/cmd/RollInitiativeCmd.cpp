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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/Factory.h>
#include <hoot/core/MapProjector.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/perty/PertyOp.h>
#include <hoot/core/util/Settings.h>

// Tgs
#include <tgs/Statistics/Random.h>

// Standard
#include <time.h>

namespace hoot
{

/**
 * A fun little easter egg.
 */
class RollInitiativeCmd : public BaseCommand
{
public:

  static string className() { return "hoot::RollInitiativeCmd"; }

  RollInitiativeCmd() { }

  virtual bool displayInHelp() const { return false; }

  virtual QString getHelp() const
  {
    return getName() +
        "  Rolls initiative for the scrum.";
  }

  virtual QString getName() const { return "roll-initiative"; }

  virtual int runSimple(QStringList args)
  {
    if (args.size() != 0)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes zero parameters.").arg(getName()));
    }

    QStringList pigs;
    pigs << "Jason Surratt"
         << "Matt Jackson"
         << "Jong Choi"
         << "Brian Hatchl"
         << "Brandon Witham"
         << "Mike Jeffe"
         << "Christy Berman"
         << "Drew Bower"
         << "Greg Schmidt"
         << "Mike Benedict";

    Tgs::Random::instance()->seed(time(0));
    for (int i = 0; i < pigs.size(); i++)
    {
      pigs[i] = QString("%1 %2").arg(Tgs::Random::instance()->generateInt(20) + 1, 2).arg(pigs[i]);
    }

    pigs.sort();

    for (int i = pigs.size() - 1; i >= 0; i--)
    {
      cout << pigs[i] << endl;
    }

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, RollInitiativeCmd)

}

