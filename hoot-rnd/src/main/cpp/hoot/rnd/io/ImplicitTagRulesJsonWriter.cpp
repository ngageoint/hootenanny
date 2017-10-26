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
#include "ImplicitTagRulesJsonWriter.h"

// hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Factory.h>

// Qt
#include <QStringBuilder>

namespace hoot
{

HOOT_FACTORY_REGISTER(ImplicitTagRuleWordPartWriter, ImplicitTagRulesJsonWriter)

ImplicitTagRulesJsonWriter::ImplicitTagRulesJsonWriter() :
_ruleWordPartCtr(0)
{
}

ImplicitTagRulesJsonWriter::~ImplicitTagRulesJsonWriter()
{
  close();
}

bool ImplicitTagRulesJsonWriter::isSupported(const QString outputUrl)
{
  return outputUrl.endsWith(".json", Qt::CaseInsensitive);
}

void ImplicitTagRulesJsonWriter::open(const QString outputUrl)
{
  _ruleWordPartCtr = 0;

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

  _outputFile->write(QString("[\n").toUtf8());
}

void ImplicitTagRulesJsonWriter::write(const QString inputUrl, const long totalParts)
{
  QFile inputFile(inputUrl);
  if (!inputFile.open(QIODevice::ReadOnly))
  {
    throw HootException(QObject::tr("Error opening %1 for reading.").arg(inputUrl));
  }

  //QString::number(count) % "\t" % word % "\t" % kvp;
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
      QString closingRuleLine = "  }";
      if (_ruleWordPartCtr < (totalParts - 1))
      {
        closingRuleLine += ",";
      }
      closingRuleLine += "\n";
      _outputFile->write(closingRuleLine.toUtf8());
      _outputFile->write(QString("  {\n").toUtf8());
      _currentWord = word;
    }

    const QString wordLine = "    \"word\": \"" % word % "\",\n";
    _outputFile->write(wordLine.toUtf8());

    const QString kvpLine = "    \"" + kvp + "\": " % count % ",\n";
    _outputFile->write(kvpLine.toUtf8());
  }
  inputFile.close();

  _ruleWordPartCtr++;
}

void ImplicitTagRulesJsonWriter::close()
{
  if (_outputFile.get())
  {
    _outputFile->write(QString("]").toUtf8());
    _outputFile->close();
    _outputFile.reset();
    _ruleWordPartCtr = 0;
  }
}

}
