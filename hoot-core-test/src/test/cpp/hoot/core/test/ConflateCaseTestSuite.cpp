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
#include "ConflateCaseTestSuite.h"

// hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/HootConfig.h>

// Qt
#include <QDir>
#include <QFileInfo>

namespace hoot
{

ConflateCaseTestSuite::ConflateCaseTestSuite(QString dir) : TestSuite(dir.toStdString())
{
  QStringList confs;
  _loadDir(dir, confs);
}

void ConflateCaseTestSuite::_loadDir(QString dir, QStringList confs)
{
  if (dir.endsWith(".off"))
  {
    return;
  }
  QDir d(dir);

  QFileInfo fi(d.absoluteFilePath("Config.conf"));

  if (fi.exists())
  {
    confs.append(fi.absoluteFilePath());
  }

  // a list of strings paths to ignore if this string is found in the path.
  QStringList ignoreList;

# ifndef HOOT_HAVE_RND
    ignoreList << "hoot-rnd";
# endif
# ifndef HOOT_HAVE_SERVICES
    ignoreList << "hoot-services";
# endif
# ifndef HOOT_HAVE_NODEJS
    ignoreList << "hoot-js";
# endif

  QStringList dirs = d.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
  for (int i = 0; i < dirs.size(); i++)
  {
    QString path = d.absoluteFilePath(dirs[i]);

    bool ignore = false;

    for (int i = 0; i < ignoreList.size(); i++)
    {
      if (path.contains(ignoreList[i]))
      {
        ignore = true;
      }
    }

    if (ignore)
    {
      LOG_WARN("Disabling: " + path);
    }
    else
    {
      _loadDir(path, confs);
    }
  }

  if (dirs.size() > 0)
  {
    // this is entirely a preference thing. I want people to keep the tests clean and uncluttered.
    if (QFileInfo(d, "Input1.osm").exists() ||
        QFileInfo(d, "Input2.osm").exists() ||
        QFileInfo(d, "Output.osm").exists())
    {
      throw HootException("Please put conflate test cases in a directory w/o sub directories.");
    }
  }
  else
  {
    addTest(new ConflateCaseTest(d, confs));
  }
}

}
