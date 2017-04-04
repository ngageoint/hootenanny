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
#include <pp/io/RecordWriter.h>
#include <pp/mapreduce/Reducer.h>

// std
#include <string>

// Qt
#include <QString>

namespace hoot
{

using namespace std;

class WriteOsmSqlStatementsReducer : public pp::Reducer
{

public:

  static string className() { return "hoot::WriteOsmSqlStatementsReducer"; }

  WriteOsmSqlStatementsReducer();

  virtual void reduce(HadoopPipes::ReduceContext& context);

private:

  pp::RecordWriter* _writer;
  HadoopPipes::ReduceContext* _context;
  QString _tableHeader;
  QString _sqlStatements;
  long _sqlStatementBufferSize;

  void _flush();

};

}

#endif // WRITEOSMSQLSTATEMENTSREDUCER_H
