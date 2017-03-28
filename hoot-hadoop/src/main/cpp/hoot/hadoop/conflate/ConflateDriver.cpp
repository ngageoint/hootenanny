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

#include "ConflateDriver.h"

// Hoot
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/hadoop/stats/MapStats.h>
#include <hoot/hadoop/pbf/PbfInputFormat.h>
#include <hoot/hadoop/pbf/PbfRecordReader.h>
#include <hoot/hadoop/pbf/PbfRecordWriter.h>

// Pretty Pipes
#include <pp/mapreduce/Job.h>
#include <pp/Hdfs.h>

// Qt
#include <QDir>
#include <QFileInfo>

#include "ConflateMapper.h"
#include "ConflateReducer.h"

namespace hoot
{

ConflateDriver::ConflateDriver()
{
}

void ConflateDriver::conflate(QString in, vector<Envelope> envelopes, double buffer,
  QString out)
{
  // create a job
  pp::Job job;

  job.setVerbose(Log::getInstance().getLevel() <= Log::Debug);
  // set the name
  job.setName("ConflateDriver");

  // be nice and don't start the reduce tasks until most of the map tasks are done.
  job.getConfiguration().setDouble("mapred.reduce.slowstart.completed.maps", 0.98);

  // set the input/output
  pp::Hdfs fs;
  job.setInput(fs.getAbsolutePath(in.toStdString()));
  job.setOutput(fs.getAbsolutePath(out.toStdString()));

  job.getConfiguration().set(ConflateMapper::envelopesKey(), _toString(envelopes));
  job.getConfiguration().set(ConflateMapper::replacementsKey(),
    fs.getAbsolutePath(in.toStdString()));
  job.getConfiguration().setDouble(ConflateMapper::maxWaySizeKey(), buffer);
  job.getConfiguration().setDouble(ConflateMapper::bufferKey(), buffer);

  // read the max ids from in and write them to the configuration
  MapStats stats;
  stats.readDir(in);
  stats.write(job.getConfiguration());

  // setup the mapper and reducer classes.
  job.setMapperClass(ConflateMapper::className());
  job.setReducerClass(ConflateReducer::className());
  job.setInputFormatClass(PbfInputFormat::className());
  job.setRecordReaderClass(PbfRecordReader::className());
  job.setRecordWriterClass(PbfRecordWriter::className());

  // Adds all libraries in this directory to the job.
  job.addLibraryDirs(ConfigOptions().getHootHadoopLibpath());

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

  _addDefaultJobSettings(job);

  // conflation runs can go for a _long_ time. Setting timeout to 6 hours.
  job.getConfiguration().setInt("mapred.task.timeout", 6 * 3600 * 1000);

  // run the job.
  job.run();
}

string ConflateDriver::_toString(const vector<Envelope>& e)
{
  QStringList l;
  if (e.size() == 0)
  {
    return "null";
  }
  for (size_t i = 0; i < e.size(); i++)
  {
    l.append(GeometryUtils::toString(e[i]));
  }
  return l.join(";").toStdString();
}

}
