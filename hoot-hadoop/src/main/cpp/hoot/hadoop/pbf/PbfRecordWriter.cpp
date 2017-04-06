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

#include "PbfRecordWriter.h"

// Hadoop Utils
#include <hadoop/SerialUtils.hh>

// Pretty Pipes
#include <pp/Factory.h>
#include <pp/Hdfs.h>
using namespace pp;

// Standard
#include <map>

namespace hoot
{

PP_FACTORY_REGISTER(pp::RecordWriter, PbfRecordWriter)

PbfRecordWriter::PbfRecordWriter()
{
  _OsmPbfWriter = new OsmPbfWriter();
}

PbfRecordWriter::~PbfRecordWriter()
{
  delete _OsmPbfWriter;
}

void PbfRecordWriter::close()
{
  try
  {
    LOG_INFO("Finalizing map.");
    _OsmPbfWriter->finalizePartial();
    _out->flush();
    _out.reset();
  }
  catch (std::exception& e)
  {
    throw HadoopUtils::Error(e.what());
  }
}

void PbfRecordWriter::emit(const string&, const string&)
{
  LOG_WARN("Emit is not supported with this record writer. Use emitRecord.");
}

void PbfRecordWriter::emitRecord(const boost::shared_ptr<const Node>& n)
{
  _OsmPbfWriter->writePartial(n);
}

void PbfRecordWriter::emitRecord(boost::shared_ptr<OsmMap> map)
{
  if (_out.get() == NULL)
  {
    LOG_WARN("Invalid stream.");
  }
  if (map.get() == NULL)
  {
    LOG_WARN("Invalid map.");
  }
  else
  {
    // RHEL complains about this being ambiguous
    const boost::shared_ptr<const OsmMap>& co = map;
    _OsmPbfWriter->writePartial(co);
  }
}

void PbfRecordWriter::setMapContext(int part, string workDir)
{
  QString path = QString("%1/part-%2m.pbf").
      arg(QString::fromStdString(workDir)).
      arg(part, 5, 10, QChar('0'));
  _path = path.toStdString();
  Hdfs fs;
  _out.reset(fs.create(_path));
  _OsmPbfWriter->intializePartial(_out.get());
}

void PbfRecordWriter::setReduceContext(HadoopPipes::ReduceContext& context)
{
  int part = context.getJobConf()->getInt("mapred.task.partition");
  string workDir = context.getJobConf()->get("mapred.work.output.dir");

  setReduceContext(part, workDir);
}

void PbfRecordWriter::setReduceContext(int part, string workDir)
{
  QString path = QString("%1/part-%2r.pbf").
      arg(QString::fromStdString(workDir)).
      arg(part, 5, 10, QChar('0'));
  _path = path.toStdString();
  Hdfs fs;
  _out.reset(fs.create(_path));
  _OsmPbfWriter->intializePartial(_out.get());
}


}
