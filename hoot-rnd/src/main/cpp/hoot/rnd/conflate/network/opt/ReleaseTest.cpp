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
#include "ReleaseTest.h"

// hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/util/HootException.h>

// Qt
#include <QTextStream>

namespace hoot
{

ReleaseTest::ReleaseTest(QDir d, QStringList confs) :
AbstractRegressionTest(d, confs)
{
}

void ReleaseTest::_parseScore()
{
  QDir scoresDir("scores");
  QStringList nameFilters;
  nameFilters.append("*scores.txt");
  const QStringList scoresDirContents = scoresDir.entryList(nameFilters, QDir::Files);
  if (scoresDirContents.size() != 1)
  {
    const QString msg =
      "Found " + QString::number(scoresDirContents.size()) + " score files and expected to " +
      "find one scores file.";
    LOG_ERROR(msg);
    throw HootException(msg);
  }
  LOG_VARD(scoresDirContents[0]);
  QFile scoresFile("scores/" + scoresDirContents[0]);
  if (!scoresFile.open(QIODevice::ReadOnly))
  {
    const QString msg = "Unable to open scores file: scores/" + scoresDirContents[0];
    LOG_ERROR(msg);
    throw HootException(msg);
  }
  try
  {
    QTextStream inStream(&scoresFile);
    QString line;
    bool foundConflatedScoreLine = false;
    _overallScore = -1;
    do
    {
      line = inStream.readLine();
      LOG_VARD(line);
      if (line.toLower().contains("conflated"))
      {
        foundConflatedScoreLine = true;
      }
      else if (foundConflatedScoreLine && line.toLower().startsWith("overall"))
      {
        LOG_VARD(line.split(" "));
        _overallScore = line.split(" ")[1].trimmed().toDouble();
        LOG_VARD(_overallScore);
      }
    }
    while (!line.isNull() && _overallScore == -1);
  }
  catch (const std::exception& e)
  {
    scoresFile.close();
    throw e;
  }
  scoresFile.close();
  LOG_VARD(_overallScore);

  LOG_ERROR("Test: " << getName() << " passed with overall score: " << _overallScore);
  LOG_VARD(_minPassingScore);
  if (_overallScore > _minPassingScore)
  {
    LOG_ERROR(_overallScore << " is a new high score for: " << getName());
    LOG_ERROR("\n\n***BOOM GOES THE DYNAMITE!***\n");
    _minPassingScore = _overallScore;
  }
  LOG_VARD(_minPassingScore);
}

}
