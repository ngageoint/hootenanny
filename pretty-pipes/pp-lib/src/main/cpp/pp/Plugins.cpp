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

#include "Plugins.h"

// Qt
#include <QLibrary>

// Standard
#include <iostream>
#include <stdlib.h>

#include "Exception.h"

using namespace std;

namespace pp
{

QStringList Plugins::_loaded;

Plugins::Plugins()
{
}

void Plugins::loadAllLibraries(const string& libs) throw(Exception)
{
  QStringList l = QString::fromStdString(libs).split(",");
  for (int i = 0; i < l.size(); i++)
  {
    loadLibrary(l[i].toStdString());
  }
}

void Plugins::loadLibrary(const string& lib) throw(Exception)
{
  QString str = QString::fromStdString(lib);

  if (_loaded.contains(str) == false)
  {
    QLibrary lib(str);
    if (lib.load() == false)
    {
      QString message = "Error loading library: " + str + "\n";
      message += "LD_LIBRARY_PATH: " + QString(getenv("LD_LIBRARY_PATH")) + "\n";
      message += "JAVA_HOME: " + QString(getenv("JAVA_HOME")) + "\n";
      message += "HADOOP_HOME: " + QString(getenv("HADOOP_HOME")) + "\n";

      throw pp::Exception(message);
    }
    _loaded << str;
  }
  else
  {
    //cout << "Skipping load library: " << lib << endl;
  }
}

}
