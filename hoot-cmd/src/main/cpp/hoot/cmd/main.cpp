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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

// GCC
#include <execinfo.h>

// GEOS
#include <geos/geom/Envelope.h>
using namespace geos::geom;

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/Hoot.h>
#include <hoot/core/HootConfig.h>
#include <hoot/core/cmd/Command.h>
#include <hoot/core/util/Log.h>
#include <hoot/js/v8Engine.h>
using namespace hoot;

// Qt
#include <QCoreApplication>
#include <QString>
#include <QTime>
#include <QString>

// Standard
#include <exception>

using namespace std;

int main(int argc, char *argv[])
{
  Hoot::getInstance().init();

  v8Engine::setPlatformInit();
  v8Engine::getInstance().init();

  QCoreApplication app(argc, argv);

  vector<QString> cmds = Factory::getInstance().getObjectNamesByBase(Command::className());
  std::shared_ptr<Command> c;
  for (size_t i = 0; i < cmds.size(); i++)
  {
    c.reset(Factory::getInstance().constructObject<Command>(cmds[i]));
    QString argName = c->getName();
    if (QString(argv[1]) == argName)
    {
      break;
    }
    else
    {
      c.reset();
    }
  }

  if (c != nullptr)
  {
    try
    {
      return c->run(argv, argc);
    }
    catch (const std::exception& e)
    {
      cerr << "Error running " << c->getName().toStdString() << ":" << endl;
      cerr << e.what() << endl;
      return -1;
    }
  }
  else
  {
    c.reset(Factory::getInstance().constructObject<Command>("hoot::HelpCmd"));
    c->run(argv, argc);
    return -1;
  }
}
