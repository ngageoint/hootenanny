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

#include "WordCountWriter.h"

// hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/DbUtils.h>
#include "WordCount.h"

// Qt
#include <QtAlgorithms>
#include <QFile>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

namespace hoot
{

bool ascendingWords(const WordCount& w1, const WordCount& w2)
{
  return w1.word < w2.word;
}

bool descendingCount(const WordCount& w1, const WordCount& w2)
{
  return w1.count > w2.count;
}

void WordCountWriter::write(QString basePath, QVector<WordCount> words)
{
  LOG_INFO("Start writing");
  QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName(basePath);
  if (!db.open())
  {
    throw HootException("Error opening database: " + basePath);
  }

  DbUtils::execNoPrepare(db, "CREATE TABLE words (word TEXT PRIMARY KEY, count INT)");
  DbUtils::execNoPrepare(db, "BEGIN");

  QSqlQuery q(db);
  if (q.prepare("INSERT INTO words (word, count) VALUES(:word, :count)") == false)
  {
    throw HootException(QString("Error preparing query: %1").arg(q.lastError().text()));
  }
  for (int i = 0; i < words.size(); i++)
  {
    q.bindValue(":word", words[i].word);
    q.bindValue(":count", words[i].count);

    if (q.exec() == false)
    {
      QString err = QString("Error executing query: %1 (%2)").arg(q.executedQuery()).
          arg(q.lastError().text());
      throw HootException(err);
    }
  }

  DbUtils::execNoPrepare(db, "COMMIT");
  LOG_INFO("Committed");
}

void WordCountWriter::_writeFile(QString path, long totalCount, QVector<WordCount> words,
  long maxSize)
{
  QFile fp(path);

  if (fp.open(QIODevice::WriteOnly) == false)
  {
    throw HootException("Error opening file: " + path);
  }

  fp.write(QString("%1\t%2\n").arg(totalWordCount()).arg(totalCount).toUtf8());

  for (int i = 0; i < words.size(); i++)
  {
    QByteArray line = QString("%1\t%2\n").arg(words[i].word).arg(words[i].count).toUtf8();
    if (maxSize >= 0 && fp.pos() + line.size() > maxSize)
    {
      return;
    }
    fp.write(line);
  }
}

}
