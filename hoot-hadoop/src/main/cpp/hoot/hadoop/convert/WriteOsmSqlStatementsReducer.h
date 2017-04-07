/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef WRITEOSMSQLSTATEMENTSREDUCER_H
#define WRITEOSMSQLSTATEMENTSREDUCER_H

// Pretty Pipes
#include <pp/mapreduce/Reducer.h>

// std
#include <string>

// Qt
#include <QString>
#include <QMap>

// libpq
#include <postgresql/libpq-fe.h>

namespace hoot
{

using namespace std;

class WriteOsmSqlStatementsReducer : public pp::Reducer
{

public:

  static string className() { return "hoot::WriteOsmSqlStatementsReducer"; }

  WriteOsmSqlStatementsReducer();
  virtual ~WriteOsmSqlStatementsReducer();

  virtual void reduce(HadoopPipes::ReduceContext& context);
  virtual void close();

private:

  HadoopPipes::ReduceContext* _context;
  QMap<QString, QString> _sqlStatementBuffer;
  long _sqlStatementBufferSize;
  bool _retainSqlFile;
  QString _dbConnStr;
  PGconn* _pqConn;
  QMap<QString, long> _elementCounts;
  bool _localJobTracker;

  void _flush();
  void _flushToDb(const QString tableHeader, const QString tableData);
  void _updateElementCounts(const QString tableHeader);
  void _writeElementCounts();
  bool _bufferFull() const;
  QString _tableHeaderToTableName(const QString tableHeader) const;
};

}

#endif // WRITEOSMSQLSTATEMENTSREDUCER_H
