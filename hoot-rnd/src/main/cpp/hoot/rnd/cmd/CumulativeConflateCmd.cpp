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
 * @copyright Copyright (C) 2015, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/rnd/conflate/CumulativeConflator.h>

using namespace std;

namespace hoot
{

class ConflateCumulativeCmd : public BaseCommand
{
public:

  static string className() { return "hoot::ConflateCumulativeCmd"; }

  ConflateCumulativeCmd() { }

  virtual QString getName() const { return "conflate-cumulative"; }

  virtual QString getDescription() const
  { return "Conflates three or more maps into a single map, retaining feature provenance (experimental)"; }

  virtual QString getType() const { return "rnd"; }

  virtual int runSimple(QStringList args)
  {
    //TODO: make this work with stats
    if (args.contains("--stats"))
    {
      throw HootException("Multi-conflation does not work with the --stats option.");
    }

    if (args.size() < 4)
    {
      cout << getHelp() << endl << endl;
      throw HootException(
        QString("%1 takes four or more parameters with at least three input paths and exactly") +
        QString("one output path. ").arg(getName()));
    }

    QStringList inputsTemp = args;
    inputsTemp.removeLast();
    const QStringList inputs = inputsTemp;
    const QString output = args.last();

    CumulativeConflator::conflate(inputs, output);

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, ConflateCumulativeCmd)

}

