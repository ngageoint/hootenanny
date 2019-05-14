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

#include "LineRecordWriter.h"

// Pretty Pipes
#include <pp/Factory.h>
#include <pp/Exception.h>
#include <pp/Hdfs.h>

// Standard
#include <map>

using namespace std;

namespace pp
{

PP_FACTORY_REGISTER(pp::RecordWriter, LineRecordWriter)

LineRecordWriter::LineRecordWriter()
{
}

LineRecordWriter::~LineRecordWriter()
{
}

void LineRecordWriter::close()
{
  _out->flush();
  _out.reset();
}

void LineRecordWriter::emitRecord(const char* keyData, size_t keySize, const char* valueData,
  size_t valueSize)
{
  ostream& os = *_out;
  if (os.good() == false)
  {
    throw Exception("output stream is not good.");
  }
  os.write(keyData, keySize);
  os.write("\t", 1);
  os.write(valueData, valueSize);
  os.write("\n", 1);
}

void LineRecordWriter::init(const string& workDir, int part)
{
  QString path = QString("%1/part-%2.txt").
      arg(QString::fromStdString(workDir)).
      arg(part, 5, 10, QChar('0'));
  _path = path.toStdString();
  Hdfs fs;
  _out.reset(fs.create(_path));
}

void LineRecordWriter::setReduceContext(HadoopPipes::ReduceContext& context)
{
  int part = context.getJobConf()->getInt("mapred.task.partition");
  const string& workDir = context.getJobConf()->get("mapred.work.output.dir");

  init(workDir, part);
}

}
