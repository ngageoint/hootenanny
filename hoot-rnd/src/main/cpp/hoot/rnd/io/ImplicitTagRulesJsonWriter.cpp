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

// Qt
#include <QStringBuilder>

namespace hoot
{

ImplicitTagRulesJsonWriter::ImplicitTagRulesJsonWriter()
{
}

ImplicitTagRulesJsonWriter::~ImplicitTagRulesJsonWriter()
{
  close();
}

void ImplicitTagRulesJsonWriter::open(const QString output)
{
  _file.reset(new QFile());
  _file->setFileName(output);
  if (_file->exists() && !_file->remove())
  {
    throw HootException(QObject::tr("Error removing existing %1 for writing.").arg(output));
  }
  if (!_file->open(QIODevice::WriteOnly | QIODevice::Text))
  {
    throw HootException(QObject::tr("Error opening %1 for writing.").arg(output));
  }
  LOG_DEBUG("Opened: " << output << ".");
}

bool caseInsensitiveLessThan(const QString s1, const QString s2)
{
  return s1.toLower() < s2.toLower();
}

void ImplicitTagRulesJsonWriter::write(const ImplicitTagRulesByWord& rules)
{
  _file->write(QString("[\n").toUtf8());

  long ruleCtr = 0;
  //sort rules alphabetically by word case insensitively (QMap sorts them case sensitively by
  //default)
  QStringList words = rules.keys();
  qSort(words.begin(), words.end(), caseInsensitiveLessThan);
  for (int i = 0; i < rules.size(); i++)
  {
    _file->write(QString("  {\n").toUtf8());

    const QString word = words.at(i);
    if (word.contains("="))
    {
      LOG_VARE(word);
    }
    const QString wordLine = "    \"word\": \"" % word % "\",\n";
    _file->write(wordLine.toUtf8());

    long kvpCtr = 0;
    const QMap<QString, long> kvpsWithCount = rules[word];
    for (QMap<QString, long>::const_iterator kvpItr = kvpsWithCount.begin();
       kvpItr != kvpsWithCount.end(); ++kvpItr)
    {
      QString kvpLine = "    \"" % kvpItr.key() % "\": " % QString::number(kvpItr.value());
      if (kvpCtr < (kvpsWithCount.size() - 1))
      {
        kvpLine += ",";
      }
      kvpLine += "\n";
      _file->write(kvpLine.toUtf8());
      kvpCtr++;
    }

    QString closingRuleLine = "  }";
    if (ruleCtr < (rules.size() - 1))
    {
      closingRuleLine += ",";
    }
    closingRuleLine += "\n";
    _file->write(closingRuleLine.toUtf8());
    ruleCtr++;
  }

  _file->write(QString("]").toUtf8());
}

void ImplicitTagRulesJsonWriter::close()
{
  if (_file.get())
  {
    _file->close();
    _file.reset();
  }
}

}
