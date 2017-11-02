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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ImplicitTagRulesTsvWriter.h"

// hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/util/ConfigOptions.h>

// Qt
#include <QStringBuilder>

namespace hoot
{

HOOT_FACTORY_REGISTER(ImplicitTagRuleWordPartWriter, ImplicitTagRulesTsvWriter)

ImplicitTagRulesTsvWriter::ImplicitTagRulesTsvWriter() :
_statusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval())
{
}

ImplicitTagRulesTsvWriter::~ImplicitTagRulesTsvWriter()
{
  close();
}

bool ImplicitTagRulesTsvWriter::isSupported(const QString outputUrl)
{
  return outputUrl.endsWith(".tsv", Qt::CaseInsensitive);
}

void ImplicitTagRulesTsvWriter::open(const QString outputUrl)
{
  _outputFile.reset(new QFile());
  _outputFile->setFileName(outputUrl);
  if (_outputFile->exists() && !_outputFile->remove())
  {
    throw HootException(QObject::tr("Error removing existing %1 for writing.").arg(outputUrl));
  }
  if (!_outputFile->open(QIODevice::WriteOnly | QIODevice::Text))
  {
    throw HootException(QObject::tr("Error opening %1 for writing.").arg(outputUrl));
  }
  LOG_DEBUG("Opened: " << outputUrl << ".");
}

void ImplicitTagRulesTsvWriter::write(const QString inputUrl)
{
  LOG_INFO("Writing implicit tag rules to: " << _outputFile->fileName() << "...");

  //each word takes up two rows; first col in first row contains words; remaining cols in first row
  //contain kvps; in second row, each kvp has the count directly below it

  //The input is assumed sorted by word, then by kvp.
  QFile inputFile(inputUrl);
  if (!inputFile.open(QIODevice::ReadOnly))
  {
    throw HootException(QObject::tr("Error opening %1 for reading.").arg(inputUrl));
  }

  long lineCtr = 0;
  while (!inputFile.atEnd())
  {
    const QString line = QString::fromUtf8(inputFile.readLine().constData());
    LOG_VART(line);
    const QString word = line.split("\t")[1].trimmed();
    LOG_VART(word);

    LOG_VART(_currentWord);
    if (!_currentWord.isEmpty() && word != _currentWord)
    {
      assert(_wordPartsBuffer.size() > 0);
      LOG_TRACE(
        "New word encountered while previous word's data still held.  Flushing previous word " <<
        "buffer for " << _currentWord << "...");
      _flushWordPartsBuffer();
    }
    _currentWord = word;
    _wordPartsBuffer.append(line);
    lineCtr++;

    if (lineCtr % _statusUpdateInterval == 0)
    {
      PROGRESS_INFO(
        "TSV implicit tag rules writer has parsed " << StringUtils::formatLargeNumber(lineCtr) <<
        " input file lines.");
    }
  }
  _flushWordPartsBuffer(true);
  inputFile.close();
}

void ImplicitTagRulesTsvWriter::_flushWordPartsBuffer(const bool lastRule)
{
  QString row1 = _currentWord % "\t";
  QString row2 = "\t";
  for (int i = 0; i < _wordPartsBuffer.size(); i++)
  {
    const QStringList lineParts = _wordPartsBuffer.at(i).split("\t");
    const QString kvp = lineParts[2].trimmed();
    const QString count = lineParts[0].trimmed(); //skipping numeric error checking here

    row1 += kvp;
    row2 += count;
    if (i < _wordPartsBuffer.size() - 1)
    {
      row1 += "\t";
      row2 += "\t";
    }
  }
  row1 += "\n";
  if (!lastRule)
  {
    row2 += "\n";
  }
  _outputFile->write(row1.toUtf8());
  _outputFile->write(row2.toUtf8());
  _wordPartsBuffer.clear();
}

void ImplicitTagRulesTsvWriter::close()
{
  if (_outputFile.get())
  {
    _outputFile->close();
    _outputFile.reset();
  }
}

}
