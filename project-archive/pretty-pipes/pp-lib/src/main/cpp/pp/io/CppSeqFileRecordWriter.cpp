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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015 Maxar (http://www.maxar.com/)
 */

#include "CppSeqFileRecordWriter.h"

// Pretty Pipes
#include <pp/Factory.h>
#include <pp/Hdfs.h>

// Standard
#include <map>

namespace pp
{

PP_FACTORY_REGISTER(pp::RecordWriter, CppSeqFileRecordWriter)

CppSeqFileRecordWriter::CppSeqFileRecordWriter()
{
  _writer = 0;
}

CppSeqFileRecordWriter::~CppSeqFileRecordWriter()
{
  delete _writer;
}

void CppSeqFileRecordWriter::emitRecord(const char* keyData, size_t keySize, const char* valueData,
  size_t valueSize)
{
  _writer->appendRaw(keyData, keySize, valueData, valueSize);
}

void CppSeqFileRecordWriter::setReduceContext(HadoopPipes::ReduceContext& context)
{
  int part = context.getJobConf()->getInt("mapred.task.partition");

  QString path = QString("%1/part-%2.csq").
      arg(QString::fromStdString(context.getJobConf()->get("mapred.work.output.dir"))).
      arg(part, 5, 10, QChar('0'));
  _path = path.toStdString();
  Hdfs fs;
  _out.reset(fs.create(_path));
  _writer = new CppSeqFile::Writer(*_out);
}

}
