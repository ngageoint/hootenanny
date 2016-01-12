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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/Factory.h>
#include <hoot/core/conflate/TileConflator.h>
#include <hoot/core/conflate/LocalTileWorker.h>
#include <hoot/core/cmd/BaseCommand.h>

// Qt
#include <QTime>

namespace hoot
{

class TileConflateCmd : public BaseCommand
{
public:

  static string className() { return "hoot::TileConflateCmd"; }

  TileConflateCmd() { }

  virtual QString getName() const { return "tile-conflate"; }

  virtual int runSimple(QStringList args)
  {
    if (args.size() != 3)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes three parameters.").arg(getName()));
    }

    QTime time;
    time.start();

    shared_ptr<TileWorker> worker(new LocalTileWorker());
    TileConflator tc(worker);
    tc.setSources(args[0], args[1]);
    tc.conflate(args[2]);

    LOG_INFO("Elapsed: " << (double)time.elapsed() / 1000.0 << "sec");
    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, TileConflateCmd)

}

