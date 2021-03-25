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
 * @copyright Copyright (C) 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

#include "ConfigOptionsDisplayer.h"

// Hoot
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/HootException.h>

// Qt
#include <QFile>
#include <QProcess>
#include <QTextCodec>
#include <QTextStream>

namespace hoot
{

QString runProcess(const QString& cmd)
{
  QProcess proc;
  proc.start("bash", QStringList() << "-c" << cmd);
  if (!proc.waitForStarted())
    throw HootException("Unable to list configuration options.");
  if (!proc.waitForFinished())
    throw HootException("Unable to list configuration options.");
  return QString(proc.readAll());
}

QString ConfigOptionsDisplayer::getAllOptionNames(const bool withDetails)
{
  const QString configOptionsFile = ConfigOptions().getConfigOptionsFile();
  QString cmd;
  LOG_VARD(withDetails);
  if (!withDetails)
  {
    cmd = "cat " + configOptionsFile + " | grep '^===' | sed 's/=== //g'";
  }
  else
  {
    cmd = "cat " + configOptionsFile + " | grep -v '//'";
  }

  return runProcess(cmd);
}

QString ConfigOptionsDisplayer::getOptionName(const QString& optionName, const bool withDetails)
{
  const QString configOptionsFile = ConfigOptions().getConfigOptionsFile();
  QString cmd;
  LOG_VARD(optionName);
  LOG_VARD(withDetails);
  if (!withDetails)
  {
    cmd =
      "cat " + configOptionsFile + " | grep '^=== " + optionName.toLower().trimmed() +
      "' | sed 's/=== //g'";
    return runProcess(cmd);
  }
  else
  {
    return _getAllConfigOptionsDetails(optionName, configOptionsFile);
  }
}

QString ConfigOptionsDisplayer::_getAllConfigOptionsDetails(const QString& optionName,
                                                            const QString& configOptionsFile)
{
  QFile file(configOptionsFile);
  if (!file.open(QFile::ReadOnly))
  {
    throw HootException("Error opening file for reading: " + configOptionsFile);
  }

  // We could eventually replace this w/ some fancy lookahead regex called by grep...doing it by
  // brute force for now.
  QString buffer;
  QTextStream ts(&buffer);
  try
  {
    bool foundOption = false;
    while (!file.atEnd())
    {
      const QString line = QString::fromUtf8(file.readLine().constData());

      if (line.startsWith("=== " + optionName))
      {
        foundOption = true;
      }
      else if (line.startsWith("=== "))
      {
        foundOption = false;
      }

      if (foundOption)
      {
        ts << line;
      }
    }
  }
  catch (...)
  {
    file.close();
  }
  file.close();

  return ts.readAll();
}

}
