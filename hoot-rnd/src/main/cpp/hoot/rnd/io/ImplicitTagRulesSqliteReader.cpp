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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "ImplicitTagRulesSqliteReader.h"

// hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QSqlError>
#include <QVariant>
#include <QSet>

namespace hoot
{

ImplicitTagRulesSqliteReader::ImplicitTagRulesSqliteReader(const QString path)
{
  if (!QSqlDatabase::contains(path))
  {
    _db = QSqlDatabase::addDatabase("QSQLITE", path);
    _db.setDatabaseName(path);
    if (!_db.open())
    {
      throw HootException("Error opening DB. " + path);
    }
  }
  else
  {
    _db = QSqlDatabase::database(path);
  }

  if (!_db.isOpen())
  {
    throw HootException("Error DB is not open. " + path);
  }

  _prepareQueries();
}

void ImplicitTagRulesSqliteReader::_prepareQueries()
{
  _selectTagsForWord = QSqlQuery(_db);
  if (!_selectTagsForWord.prepare("SELECT kvp FROM rules WHERE word=:word"))
  {
    throw HootException(
      QString("Error preparing query: %1").arg(_selectTagsForWord.lastError().text()));
  }
}

bool ImplicitTagRulesSqliteReader::wordsInvolveMultipleRules(const QSet<QString>& words)
{
  //can't prepare this one due to variable inputs
  QSqlQuery uniqueRuleCountForWords = QSqlQuery(_db);
  QString queryStr = "SELECT COUNT(*) DISTINCT rule_id FROM rules WHERE word IN (";
  for (QSet<QString>::const_iterator wordItr = words.begin(); wordItr != words.end(); ++wordItr)
  {
    queryStr += *wordItr + ",";
  }
  queryStr.chop(1);
  queryStr += ")";

  if (!uniqueRuleCountForWords.exec(queryStr))
  {
    throw HootException(
      QString("Error executing query: %1").arg(uniqueRuleCountForWords.lastError().text()));
  }
  return uniqueRuleCountForWords.value(0).toInt() > 1;
}

Tags ImplicitTagRulesSqliteReader::getTags(const QString word)
{
  _selectTagsForWord.bindValue(":word", word);
  if (!_selectTagsForWord.exec())
  {
    throw HootException(
      QString("Error executing query: %1").arg(_selectTagsForWord.lastError().text()));
  }

  Tags tags;
  while (_selectTagsForWord.next())
  {
    tags.appendValue(_selectTagsForWord.value(0).toString());
  }
  return tags;
}

}
