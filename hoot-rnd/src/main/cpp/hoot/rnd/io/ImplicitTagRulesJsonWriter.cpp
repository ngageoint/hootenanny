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
_ruleCtr(0)
{
}

ImplicitTagRulesJsonWriter::~ImplicitTagRulesJsonWriter()
{
  close();
}

bool ImplicitTagRulesJsonWriter::isSupported(const QString url)
{
  return url.endsWith(".json", Qt::CaseInsensitive);
}

void ImplicitTagRulesJsonWriter::open(const QString url)
{
  _ruleCtr = 0;

  _file.reset(new QFile());
  _file->setFileName(url);
  if (_file->exists() && !_file->remove())
  {
    throw HootException(QObject::tr("Error removing existing %1 for writing.").arg(url));
  }
  if (!_file->open(QIODevice::WriteOnly | QIODevice::Text))
  {
    throw HootException(QObject::tr("Error opening %1 for writing.").arg(url));
  }
  LOG_DEBUG("Opened: " << url << ".");

  _file->write(QString("[\n").toUtf8());
}

//TODO: fix
void ImplicitTagRulesJsonWriter::write(const long /*totalParts*/)
{
//  _file->write(QString("  {\n").toUtf8());

//  const QString word = ruleWordPart.getWord();
//  const QString wordLine = "    \"word\": \"" % word % "\",\n";
//  _file->write(wordLine.toUtf8());

//  long kvpCtr = 0;
//  const QMap<QString, long> kvpsWithCount = ruleWordPart.getTagsToCounts();
//  for (QMap<QString, long>::const_iterator kvpItr = kvpsWithCount.begin();
//       kvpItr != kvpsWithCount.end(); ++kvpItr)
//  {
//    QString kvpLine = "    \"" % kvpItr.key() % "\": " % QString::number(kvpItr.value());
//    if (kvpCtr < (kvpsWithCount.size() - 1))
//    {
//       kvpLine += ",";
//    }
//    kvpLine += "\n";
//    _file->write(kvpLine.toUtf8());
//    kvpCtr++;
//  }

//  QString closingRuleLine = "  }";
//  if (_ruleCtr < (totalParts - 1))
//  {
//    closingRuleLine += ",";
//  }
//  closingRuleLine += "\n";
//  _file->write(closingRuleLine.toUtf8());
//  _ruleCtr++;
}

void ImplicitTagRulesJsonWriter::close()
{
  if (_file.get())
  {
    _file->write(QString("]").toUtf8());
    _file->close();
    _file.reset();
    _ruleCtr = 0;
  }
}

}
