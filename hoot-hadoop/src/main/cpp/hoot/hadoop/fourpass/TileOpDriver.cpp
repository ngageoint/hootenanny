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

#include "TileOpDriver.h"

// Hoot
#include <hoot/core/io/ObjectOutputStream.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/hadoop/stats/MapStats.h>
#include <hoot/hadoop/pbf/PbfInputFormat.h>
#include <hoot/hadoop/pbf/PbfRecordReader.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/hadoop/pbf/PbfRecordWriter.h>

// Pretty Pipes
#include <pp/mapreduce/Job.h>
#include <pp/Hdfs.h>

// Qt
#include <QDir>
#include <QFileInfo>

#include "TileOpMapper.h"
#include "TileOpReducer.h"

namespace hoot
{

TileOpDriver::TileOpDriver()
{
}

void TileOpDriver::apply(QString in, vector<Envelope> envelopes, double buffer,
  QString out)
{
  // create a job
  pp::Job job;

  job.setVerbose(Log::getInstance().getLevel() <= Log::Debug);
  // set the name
  job.setName("TileOpDriver");

  // be nice and don't start the reduce tasks until most of the map tasks are done.
  job.getConfiguration().setDouble("mapred.reduce.slowstart.completed.maps", 0.98);

  // set the input/output
  pp::Hdfs fs;
  job.setInput(fs.getAbsolutePath(in.toStdString()));
  job.setOutput(fs.getAbsolutePath(out.toStdString()));

  if (_op == 0)
  {
    throw HootException("You must specify an operation.");
  }

  stringstream ss;
  ObjectOutputStream oos(ss);
  oos.writeObject(*_op);
  oos.flush();
  LOG_INFO("oos size: " << ss.str().size());
  job.getConfiguration().setBytes(TileOpReducer::opKey(), ss.str());

  job.getConfiguration().set(TileOpMapper::envelopesKey(), _toString(envelopes));
  job.getConfiguration().set(TileOpMapper::replacementsKey(),
    fs.getAbsolutePath(in.toStdString()));
  job.getConfiguration().setDouble(TileOpMapper::maxWaySizeKey(), buffer);
  job.getConfiguration().setDouble(TileOpMapper::bufferKey(), buffer);

  // read the max ids from in and write them to the configuration
  MapStats stats;
  stats.readDir(in);
  stats.write(job.getConfiguration());

  // setup the mapper and reducer classes.
  job.setMapperClass(TileOpMapper::className());
  job.setReducerClass(TileOpReducer::className());
  job.setInputFormatClass(PbfInputFormat::className());
  job.setRecordReaderClass(PbfRecordReader::className());
  job.setRecordWriterClass(PbfRecordWriter::className());

  // Adds all libraries in this directory to the job.
  job.addLibraryDirs(ConfigOptions().getHootHadoopLibpath());

  LOG_INFO("Hoot home: " << getenv("HOOT_HOME"));

  const std::vector<std::string>& dirs = job.getLibraryDirs();
  for (size_t i = 0; i < dirs.size(); i++)
  {
    LOG_INFO("lib dir: " << dirs[i]);
  }

  job.addFile(ConfPath::search("hoot.json").toStdString());

  // if GDAL isn't installed on all nodes, then we'll need to copy over the projection info.
  QString gdalData = QString(getenv("GDAL_DATA"));
  if (gdalData != "")
  {
    QDir gdalDir(gdalData);
    if (gdalDir.exists() == false)
    {
      LOG_WARN("Could not find GDAL_DIR: " << gdalData);
    }
    else
    {
      QStringList filters;
      filters << "*.csv";
      QFileInfoList fil = gdalDir.entryInfoList(filters, QDir::Files, QDir::Name);
      for (int i = 0; i < fil.size(); i++)
      {
        LOG_INFO("Adding GDAL_DATA file: " << fil[i].absoluteFilePath());
        job.addFile(fil[i].absoluteFilePath().toStdString());
      }
    }
  }


  // This library will be used to provide mapper/reducer classes and anything else referenced
  // by the factory.
  job.addPlugin(getenv("HOOT_HOME") + string("/lib/libHootHadoop.so.1"));

  // serialize all the configuration settings.
  job.getConfiguration().set(settingsConfKey().toStdString(),
                             conf().toString().toUtf8().constData());

  _addDefaultJobSettings(job);

  QStringList fileDeps = ConfigOptions().getHootHadoopFileDeps();
  for (int i = 0; i < fileDeps.size(); i++)
  {
    job.addFile(fileDeps[i].toStdString());
  }

  // conflation runs can go for a _long_ time. Setting timeout to 6 hours.
  job.getConfiguration().setInt("mapred.task.timeout", 6 * 3600 * 1000);

  // run the job.
  job.run();
}

void TileOpDriver::setOperation(boost::shared_ptr<OsmMapOperation> op)
{
  _op = op;
}

string TileOpDriver::_toString(const vector<Envelope>& e)
{
  QStringList l;
  if (e.size() == 0)
  {
    return "null";
  }
  for (size_t i = 0; i < e.size(); i++)
  {
    l.append(GeometryUtils::toHexString(e[i]));
  }
  return l.join(";").toStdString();
}

}
