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

#include "PaintNodesDriver.h"

// hoot
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/UuidHelper.h>
#include <hoot/hadoop/stats/MapStats.h>

// Pretty Pipes
#include <pp/Hdfs.h>
#include <pp/mapreduce/Job.h>
#include <pp/io/CppSeqFileRecordWriter.h>
#include <pp/io/CppSeqFileRecordReader.h>
using namespace pp;

// Standard
#include <vector>
using namespace std;

#include "PaintNodesMapper.h"
#include "PaintNodesReducer.h"
#include "../pbf/PbfInputFormat.h"
#include "../pbf/PbfRecordReader.h"

namespace hoot
{

PaintNodesDriver::PaintNodesDriver()
{
}

const cv::Mat& PaintNodesDriver::calculateDensity(const Envelope& e, double pixelSize,
  QString input)
{
  Hdfs fs;
  if (fs.exists(input.toStdString()) == false)
  {
    throw HootException("Input file does not exist.");
  }

  _width = ceil(e.getWidth() / pixelSize) + 1;
  _height = ceil(e.getHeight() / pixelSize) + 1;
  QString cacheName = QString("%1/density-%2x%3").arg(input).arg(_width).arg(_height);

  // if we've already calculated and cached the values.
  if (fs.exists(cacheName.toStdString()))
  {
    LOG_INFO("Loading cache file " << cacheName);
    LOG_INFO("If you want to recalculate the cache then run this first: ");
    LOG_INFO("  hadoop fs -rmr " << cacheName);
    _loadOutputDir(cacheName);
  }
  else
  {
    // create a job
    Job job;

    // set the name
    job.setName("Paint Nodes " + input.toStdString());

    fs.mkdirs("tmp");
    QString output = "tmp/" + UuidHelper::createUuid().toString().replace("{", "").replace("}", "") +
        "-PaintNodes";

    // set the input/output
    job.setInput(input.toStdString());
    job.setOutput(output.toStdString());

    // be nice and don't start the reduce tasks until most of the map tasks are done.
    job.getConfiguration().setDouble("mapred.reduce.slowstart.completed.maps", 0.98);

    job.getConfiguration().set("hoot.envelope", MapStats::toString(e));
    job.getConfiguration().setDouble("hoot.pixel.size", pixelSize);

    // setup the mapper and reducer classes.
    job.setMapperClass(PaintNodesMapper::className());
    job.setReducerClass(PaintNodesReducer::className());
    job.setInputFormatClass(PbfInputFormat::className());
    job.setRecordReaderClass(PbfRecordReader::className());
    job.setRecordWriterClass(CppSeqFileRecordWriter::className());

    // Adds all libraries in this directory to the job.
    job.addLibraryDirs(ConfigOptions().getHootHadoopLibpath());

    job.addFile(ConfPath::search("hoot.json").toStdString());

    // This library will be used to provide mapper/reducer classes and anything else referenced
    // by the factory.
    job.addPlugin(getenv("HOOT_HOME") + string("/lib/libHootHadoop.so.1"));

    _addDefaultJobSettings(job);

    // run the job.
    job.run();

    _loadOutputDir(output);

    // if the input is a directory
    if (fs.getFileStatus(input.toStdString()).isDir())
    {
      // store the density info for later use.
      fs.rename(output.toStdString(), cacheName.toStdString());
    }
    else
    {
      fs.deletePath(output.toStdString(), true);
    }
  }

  return _c;
}

void PaintNodesDriver::_loadOutputDir(const QString& output)
{
  int h = _height;
  int w = _width;
  LOG_INFO("w: " << _width << " h: " << _height);
  _c = cv::Mat(cvSize(_width, _height), CV_32SC1);

  for (int py = 0; py < h; py++)
  {
    int32_t* row1 = _c.ptr<int32_t>(py);

    for (int px = 0; px < w; px++)
    {
      row1[px] = 0;
    }
  }

  Hdfs fs;

  vector<FileStatus> status = fs.listStatus(output.toStdString(), true);
  for (size_t i = 0; i < status.size(); i++)
  {
    QString fn = QString::fromStdString(status[i].getPath());
    if (fn.endsWith(".csq"))
    {
      _loadPartFile(fn);
    }
  }
}

void PaintNodesDriver::_loadPartFile(const QString& partPath)
{
  Hdfs fs;
  auto_ptr<istream> is(fs.open(partPath.toStdString()));
  CppSeqFile::Reader reader(*is);

  string s;
  s.resize(1024);
  PaintNodesReducer::Pixel pixel;
  int32_t sum;
  while (reader.nextFixed<PaintNodesReducer::Pixel, int>(pixel, sum))
  {
    assert(pixel.x < _width);
    assert(pixel.y < _height);
    _c.ptr<int32_t>(pixel.y)[pixel.x] = sum;
  }
}

}
