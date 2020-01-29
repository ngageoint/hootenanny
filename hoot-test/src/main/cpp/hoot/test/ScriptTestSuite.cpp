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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ScriptTestSuite.h"

// hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/HootConfig.h>

// Qt
#include <QDir>
#include <QFileInfo>

namespace hoot
{

ScriptTestSuite::ScriptTestSuite(QString dir, bool printDiff, double waitTimeSec,
                                 bool hideDisableTests, bool suppressFailureDetail) :
TestSuite(dir.toStdString())
{
  QDir d(dir);
  QStringList files = d.entryList(QDir::Files);
  QStringList ignorePrefix;

  // if the web services are disabled, then ignore all script tests that start with Service
# ifndef HOOT_HAVE_SERVICES
    ignorePrefix << "Service";
    ignorePrefix << "RndService";
# endif
# ifndef HOOT_HAVE_RND
    ignorePrefix << "Rnd";
# endif

  for (int i = 0; i < files.size(); i++)
  {
    QFileInfo fi(d.absoluteFilePath(files[i]));
    if (files[i].endsWith(".sh") && fi.isExecutable() == false )
    {
      LOG_WARN("Found a .sh file that is not executable. Is that what you want?");
      LOG_WARN(".sh file: " << fi.absoluteFilePath());
    }

    if (files[i].endsWith(".off") == false && fi.isExecutable())
    {
      bool ignore = false;
      for (int j = 0; j < ignorePrefix.size(); j++)
      {
        if (fi.baseName().startsWith(ignorePrefix[j]))
        {
          if (!hideDisableTests)
          {
            LOG_WARN("Disabling: " << fi.filePath());
          }
          ignore = true;
        }
      }

      if (!ignore)
      {
        QString path = d.absoluteFilePath(files[i]);
        addTest(new ScriptTest(path, printDiff, suppressFailureDetail, waitTimeSec * 1000));
      }
    }
  }
}

}
