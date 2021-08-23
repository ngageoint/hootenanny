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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "ConflateCaseTestSuite.h"

// hoot
#include <hoot/core/HootConfig.h>
#include <hoot/core/cmd/ConflateCmd.h>
#include <hoot/core/test/ConflateCaseTest.h>


#include <hoot/core/util/ConfPath.h>

// Qt



namespace hoot
{

ConflateCaseTestSuite::ConflateCaseTestSuite(const QString& dir, bool hideDisableTests)
  : AbstractTestSuite(dir),
    _hideDisableTests(hideDisableTests),
    _numTests(0)
{
  QStringList confs;
  loadDir(dir, confs);
  LOG_VART(_numTests);
}

void ConflateCaseTestSuite::loadDir(const QString& dir, QStringList confs)
{
  if (dir.endsWith(".off"))
  {
    return;
  }
  QDir d(dir);

  QFileInfo fi(d.absoluteFilePath("Config.conf"));

  if (fi.exists())
  {
    const QString testConfFile = fi.absoluteFilePath();

    // load the test's config file
    confs.append(testConfFile);

    LOG_VART(confs);
  }

  // a list of strings paths to ignore if this string is found in the path.
  QStringList ignoreList;

# ifndef HOOT_HAVE_RND
  ignoreList << "hoot-rnd";
# endif
# ifndef HOOT_HAVE_SERVICES
  ignoreList << "hoot-services";
# endif
# ifndef HOOT_HAVE_JOSM
  ignoreList << "hoot-josm";
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
      if (!_hideDisableTests)
        LOG_INFO("Disabling: " + path);
    }
    else
    {
      loadDir(path, confs);
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
    // We require that all tests use Testing.conf. We want to load it first in order to give each
    // test a chance to override it when necessary.
    confs.prepend(ConfPath::search("Testing.conf"));

    addTest(new ConflateCaseTest(d, confs));
    _numTests++;
  }
}

}
