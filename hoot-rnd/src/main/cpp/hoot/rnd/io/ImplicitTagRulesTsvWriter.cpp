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

// Qt
#include <QStringBuilder>

namespace hoot
{

HOOT_FACTORY_REGISTER(ImplicitTagRuleWordPartWriter, ImplicitTagRulesTsvWriter)

ImplicitTagRulesTsvWriter::ImplicitTagRulesTsvWriter()
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

void ImplicitTagRulesTsvWriter::write(const QString inputUrl, const long /*totalParts*/)
{
  //QString::number(count) % "\t" % word % "\t" % kvp;

  //each word takes up two rows; first col in first row contains words; remaining cols in first row
  //contain kvps; in second row, each kvp has the count directly below it

  QFile inputFile(inputUrl);
  if (!inputFile.open(QIODevice::ReadOnly))
  {
    throw HootException(QObject::tr("Error opening %1 for reading.").arg(inputUrl));
  }

  QString row1;
  QString row2;
  while (!inputFile.atEnd())
  {
    const QString line = QString::fromUtf8(inputFile.readLine().constData());
    const QStringList lineParts = line.split("\t");
    const QString count = lineParts[0]; //skipping numeric error checking here
    const QString word = lineParts[1];
    const QString kvp = lineParts[2];

    if (_currentWord.isEmpty())
    {
      _currentWord = word;
    }
    else if (word != _currentWord)
    {
      row1 += "\n";
      _outputFile->write(row1.toUtf8());
      row1 = "";
      row2 += "\n";
      _outputFile->write(row2.toUtf8());
      row2 = "";
    }

    if (row1.isEmpty())
    {
      row1 = word % "\t";
    }
    else
    {
      row1 += "\t" % kvp;
      row2 += "\t" % count;
    }
  }
  inputFile.close();
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
