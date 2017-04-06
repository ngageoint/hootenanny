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

#include "HadoopTileWorker.h"

// Hoot
#include <hoot/core/util/Log.h>
#include <hoot/hadoop/conflate/ConflateDriver.h>
#include <hoot/hadoop/stats/MapStatsDriver.h>
#include <hoot/hadoop/paint-nodes/PaintNodesDriver.h>
#include <hoot/hadoop/way-join/WayJoinDriver.h>
#include <hoot/core/util/UuidHelper.h>

// Pretty Pipes
#include <pp/Hdfs.h>
#include <pp/mapreduce/Job.h>

namespace hoot
{

HadoopTileWorker::HadoopTileWorker()
{
  _workDir = "tmp/" +
      UuidHelper::createUuid().toString().replace("{", "").replace("}", "") +
      "-HadoopTileWorker/";
  pp::Hdfs fs;
  _workDir = QString::fromStdString(fs.getAbsolutePath(_workDir.toStdString()));

  _initialized = false;
//#warning to speed debugging
//  pp::Job::setDefaultJobTracker("local");
}

void HadoopTileWorker::breakWays(QString out)
{
  WayJoinDriver wjd1(_maxWaySize);
  wjd1.setRenumbering(0, 0, 0, Status::Unknown1);
  wjd1.calculateWayBounds(_in1, out);
  _stats1.readDir(out);

  pp::Hdfs fs;
  QString tmp = "tmp/" + UuidHelper::createUuid().toString().replace("{", "").replace("}", "") +
      "-WayJoinDriver2.pbf";
  tmp = QString::fromStdString(fs.getAbsolutePath(tmp.toStdString()));

  LOG_INFO("stats1: " << _stats1.toString().toStdString());
  LOG_INFO("stats2: " << _stats2.toString().toStdString());
  WayJoinDriver wjd2(_maxWaySize);
  long nodeDelta = std::max(1l, _stats1.getMaxNodeId() - _stats2.getMinNodeId() + 1);
  long wayDelta = std::max(1l, _stats1.getMaxWayId() - _stats2.getMinWayId() + 1);
  long relationDelta = std::max(1l, _stats1.getMaxRelationId() - _stats2.getMinRelationId() + 1);
  LOG_INFO("Node delta: " << nodeDelta << " way delta: " << wayDelta << " relation delta: " <<
           relationDelta);
  wjd2.setRenumbering(nodeDelta, wayDelta, relationDelta, Status::Unknown2);
  wjd2.calculateWayBounds(_in2, tmp);

  QString moveTo = out + "/in2-part";
  vector<pp::FileStatus> s = fs.listStatus(tmp.toStdString(), true);
  for (size_t i = 0; i < s.size(); i++)
  {
    QString fn = QString::fromStdString(s[i].getPath());
    if (fn.endsWith(".pbf") || fn.endsWith(".stats"))
    {
      QString base = fn;
      base.remove(QRegExp(".*/"));
      QString to = moveTo + base;
      LOG_INFO("Moving from " << fn.toStdString() << " to " << to.toStdString());
      fs.rename(fn.toStdString(), to.toStdString());
    }
  }

  fs.deletePath(tmp.toStdString(), true);
//#warning for debugging
//  pp::Job::setDefaultJobTracker("");
}

OGREnvelope HadoopTileWorker::calculateEnvelope()
{
  return _envelope;
}

void HadoopTileWorker::calculateNodeDensity(cv::Mat& r1, cv::Mat& r2)
{
  r1 = _r1;
  r2 = _r2;
}

MapStats HadoopTileWorker::_calculateStats(QString in)
{
  MapStats result;
  pp::Hdfs fs;
  // if the stats have already been calculated
  vector<pp::FileStatus> files = fs.listStatus(in.toStdString(), true);
  for (size_t i = 0; i < files.size(); i++)
  {
    if (QString::fromStdString(files[i].getPath()).endsWith(".stats"))
    {
      // read them in and return it.
      result.readDir(in);
      return result;
    }
  }

  result = MapStatsDriver().calculateStats(in);
  // if the input is a directory, then we can store the stats in it.
  if (fs.getFileStatus(in.toStdString()).isDir())
  {
    // write the newly calculated stats out to the input directory.
   boost::shared_ptr<ostream> out(fs.create((in + "/all.stats").toStdString()));
    result.write(*out);
  }
  return result;
}

void HadoopTileWorker::cleanup(QString mapIn, QString mapOut)
{
  /// @todo This is very inefficient. Replace w/ a custom mapper that simply replaces nodes
  vector<Envelope> tiles;
  ConflateDriver cd;
  cd.conflate(mapIn, tiles, _buffer, mapOut);
}

void HadoopTileWorker::conflate(const vector<Envelope>& tiles, QString mapIn, QString mapOut)
{
  ConflateDriver cd;
  cd.conflate(mapIn, tiles, _buffer, mapOut);
}

bool HadoopTileWorker::exists(QString dir)
{
  pp::Hdfs fs;
  return fs.exists(dir.toStdString());
}

void HadoopTileWorker::init(QString in1, QString in2, double buffer)
{
  _in1 = in1;
  _in2 = in2;
  _maxWaySize = buffer;
  _pixelSize = buffer;
  _buffer = buffer;

  _stats1 = _calculateStats(in1);
  _stats2 = _calculateStats(in2);

  // calculate the envelope of both maps.
  Envelope e = _stats1.getEnvelope();
  e.expandToInclude(&_stats2.getEnvelope());
  LOG_INFO("Env: " << e.toString());

  _envelope.MinX = e.getMinX();
  _envelope.MaxX = e.getMaxX();
  _envelope.MinY = e.getMinY();
  _envelope.MaxY = e.getMaxY();

  PaintNodesDriver pnd1;
  _r1 = pnd1.calculateDensity(e, _pixelSize, in1);

  PaintNodesDriver pnd2;
  _r2 = pnd2.calculateDensity(e, _pixelSize, in2);
}

void HadoopTileWorker::mkdir(QString dir)
{
  pp::Hdfs fs;
  fs.mkdirs((dir).toStdString());
}

void HadoopTileWorker::rmdir(QString dir)
{
  pp::Hdfs fs;
  fs.deletePath((dir).toStdString(), true);
}

}
