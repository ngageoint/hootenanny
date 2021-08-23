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
 * @copyright Copyright (C) 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#include "PertyTest.h"

// hoot



// Qt
#include <QTextStream>

namespace hoot
{

PertyTest::PertyTest(QDir d, QStringList confs) :
AbstractRegressionTest(d, confs)
{
}

void PertyTest::_parseScore()
{
  QDir outputDir("output");
  QStringList nameFilters;
  nameFilters.append("results");
  const QStringList outputDirContents = outputDir.entryList(nameFilters, QDir::Files);
  if (outputDirContents.size() != 1)
  {
    const QString msg =
      "Found " + QString::number(outputDirContents.size()) + " results files and expected to " +
      "find one results file.";
    LOG_ERROR(msg);
    throw HootException(msg);
  }
  LOG_VARD(outputDirContents[0]);
  QFile resultsFile("output/" + outputDirContents[0]);
  if (!resultsFile.open(QIODevice::ReadOnly))
  {
    const QString msg = "Unable to open results file: output/" + outputDirContents[0];
    LOG_ERROR(msg);
    throw HootException(msg);
  }
  try
  {
    QTextStream inStream(&resultsFile);
    QString line;
    _score = -1;
    do
    {
      line = inStream.readLine();
      LOG_VART(line);
      if (line.toLower().startsWith("test run score (averaged)"))
      {
        _score = line.split(":")[1].trimmed().toDouble();
        LOG_VARD(_score);
      }
    }
    while (!line.isNull() && _score == -1);
  }
  catch (const std::exception& e)
  {
    resultsFile.close();
    throw;
  }
  resultsFile.close();
}

}
