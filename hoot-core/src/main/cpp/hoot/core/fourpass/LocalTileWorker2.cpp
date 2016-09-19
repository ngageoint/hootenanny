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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "LocalTileWorker2.h"

// hoot
#include <hoot/core/Conflator.h>
#include <hoot/core/MapProjector.h>
#include <hoot/core/OsmMapListener.h>
#include <hoot/core/conflate/LargeWaySplitter.h>
#include <hoot/core/conflate/OutsideBoundsRemover.h>
#include <hoot/core/algorithms/WaySplitter.h>
#include <hoot/core/ops/SuperfluousNodeRemover.h>
#include <hoot/core/conflate/TileBoundsCalculator.h>
#include <hoot/core/io/OsmReader.h>
#include <hoot/core/io/OsmWriter.h>
#include <hoot/core/ops/MergeNearbyNodes.h>
#include <hoot/core/ops/RemoveNodeOp.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/UuidHelper.h>
#include <hoot/core/visitors/CalculateBoundsVisitor.h>

namespace hoot
{

class ReplacedNodeListener : public OsmMapListener
{
public:
  ReplacedNodeListener(HashMap<long, long>& m) : _map(m) {}

  virtual shared_ptr<OsmMapListener> clone() const
  {
    return shared_ptr<OsmMapListener>(new ReplacedNodeListener(_map));
  }

  void replaceNodePre(long oldId, long newId) { _map[oldId] = newId; }

protected:
  HashMap<long, long>& _map;
};

LocalTileWorker2::LocalTileWorker2()
{
  _mapPart = 0;
  _workDir = QDir::tempPath() + "/" +
      UuidHelper::createUuid().toString().replace("{", "").replace("}", "") +
      "-LocalTileWorker2/";
  QDir().mkdir(_workDir);
}

void LocalTileWorker2::applyOp(shared_ptr<OsmMapOperation> op, const vector<Envelope>& tiles,
  QString mapIn, QString mapOut)
{
  _mapPart = 0;
  for (size_t i = 0; i < tiles.size(); i++)
  {
    NodeReplacements replacements;
    replacements.readDir(mapIn);
    shared_ptr<OsmMap> map = _readTile(mapIn, tiles[i], _buffer);

    if (map->getNodeMap().size() > 0)
    {
      // keep track of any replaced nodes.
      shared_ptr<ReplacedNodeListener> rnl(
        new ReplacedNodeListener(replacements.getReplacements()));
      map->registerListener(rnl);

      Boundable* b = dynamic_cast<Boundable*>(op.get());
      if (b != NULL)
      {
        b->setBounds(tiles[i]);
      }
      op->apply(map);

      // it only actually does a reprojection if necessary.
      MapProjector::projectToWgs84(map);

      // Using a copy of the map so we can split ways as needed. Make sure they're the right size.
      const WayMap wm = map->getWays();
      for (WayMap::const_iterator it = wm.begin(); it != wm.end(); ++it)
      {
        const shared_ptr<Way>& w = map->getWay(it->first);
        WaySplitter::split(map, w, _maxWaySize);
      }

      _storeMapPart(map, mapOut);
      _writeNodeReplacements(mapOut, i, replacements);
    }
  }

  _writeTheRest(mapIn, mapOut, tiles, _buffer);
}

void LocalTileWorker2::breakWays(QString out)
{
  shared_ptr<OsmMap> map(new OsmMap());

  OsmReader reader;
  reader.setDefaultStatus(Status::Unknown1);
  reader.read(_in1, map);
  if (_in2.isEmpty() == false)
  {
    reader.setDefaultStatus(Status::Unknown2);
    reader.read(_in2, map);
  }

  // split any ways into smaller bits if the ways bounds are greater than _buffer. In this case
  // all units are in decimal degrees, not meters.
  LargeWaySplitter::splitWays(map, _maxWaySize);

  _storeMapPart(map, out);
}

OGREnvelope LocalTileWorker2::calculateEnvelope()
{
  shared_ptr<OsmMap> map(new OsmMap());

  OsmReader reader;
  reader.setDefaultStatus(Status::Unknown1);
  reader.read(_in1, map);
  if (_in2.isEmpty() == false)
  {
    reader.setDefaultStatus(Status::Unknown2);
    reader.read(_in2, map);
  }

  return CalculateBoundsVisitor::getBounds(map);
}

void LocalTileWorker2::calculateNodeDensity(cv::Mat& r1, cv::Mat& r2)
{
  shared_ptr<OsmMap> map(new OsmMap());

  OsmReader reader;
  reader.setDefaultStatus(Status::Unknown1);
  reader.read(_in1, map);
  if (_in2.isEmpty() == false)
  {
    reader.setDefaultStatus(Status::Unknown2);
    reader.read(_in2, map);
  }

  TileBoundsCalculator tbc(_pixelSize);
  tbc.renderImage(map, r1, r2);
}

void LocalTileWorker2::cleanup(QString mapIn, QString mapOut)
{
  shared_ptr<OsmMap> map = _readAllParts(mapIn);

  OsmWriter writer;
  writer.setIncludePointsInWays(true);
  writer.setIncludeHootInfo(true);
  writer.write(map, mapOut);
}

bool LocalTileWorker2::exists(QString dir)
{
  return QDir().exists(dir);
}

void LocalTileWorker2::init(QString in1, QString in2, double buffer)
{
  _in1 = in1;
  _in2 = in2;
  _maxWaySize = buffer;
  _pixelSize = buffer;
  _buffer = buffer;
}

void LocalTileWorker2::mkdir(QString dir)
{
  if (QDir().mkpath(dir) == false)
  {
    throw HootException("Error making dir: " + dir);
  }
}

shared_ptr<OsmMap> LocalTileWorker2::_readAllParts(QString dir)
{
  shared_ptr<OsmMap> map(new OsmMap());

  OsmReader reader;
  reader.setUseDataSourceIds(true);
  reader.setUseStatusFromFile(true);

  QStringList filters;
  filters << "Part*.osm";
  // load the dregs first so the "bad" nodes get overwritten.
  QString dregs = dir + "/Dregs.osm";
  if (QFileInfo(dregs).exists())
  {
    reader.read(dregs, map);
  }
  QDir d(dir);
  Q_FOREACH(QFileInfo info, d.entryList(filters, QDir::Files, QDir::Name))
  {
    LOG_INFO(info.filePath() << " " << d.absoluteFilePath(info.filePath()) << " " << d.absolutePath() <<
             " " << info.absoluteFilePath());
    reader.read(d.absoluteFilePath(info.filePath()), map);
  }

  return map;
}

shared_ptr<OsmMap> LocalTileWorker2::_readTile(QString input, const Envelope& e, double buffer)
{
  shared_ptr<OsmMap> map = _readAllParts(input);

  Envelope buffered = e;
  buffered.expandBy(buffer);
  OutsideBoundsRemover::removeWays(map, buffered);
  SuperfluousNodeRemover::removeNodes(map, e);

  return map;
}

void LocalTileWorker2::_replaceNodes(shared_ptr<OsmMap> map, const HashMap<long, long>& replacements)
{
  for (HashMap<long, long>::const_iterator it = replacements.begin(); it != replacements.end();
       it++)
  {
    long from = it->first;
    long to = it->second;

    if (map->containsNode(from))
    {
      map->replaceNode(from, to);
    }
  }
}

void LocalTileWorker2::rmdir(QString dir)
{
  FileUtils::removeDir(dir);
}

void LocalTileWorker2::_storeMapPart(shared_ptr<OsmMap> map, QString dir)
{
  QString fn = dir + QString("/Part%1.osm").arg(_mapPart++, 4, 10, QChar('0'));
  OsmWriter writer;
  writer.setIncludePointsInWays(true);
  writer.setIncludeHootInfo(true);
  writer.write(map, fn);
}

void LocalTileWorker2::_writeNodeReplacements(QString dir, size_t i,
  NodeReplacements& replacements)
{
  QString fn = QString("%1/Tile%2.replacement").arg(dir).arg(i);
  replacements.write(fn);
}

void LocalTileWorker2::_writeTheRest(QString dirIn, QString dirOut,
  const vector<Envelope>& conflatedBits, double buffer)
{
  shared_ptr<OsmMap> map = _readAllParts(dirIn);

  for (size_t i = 0; i < conflatedBits.size(); i++)
  {
    Envelope buffered = conflatedBits[i];
    buffered.expandBy(buffer);
    OutsideBoundsRemover::removeWays(map, buffered, true);
  }

  NodeMap nodes = map->getNodeMap();
  // @optimize could use the index for this, but not necessary in debug mode.
  for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
  {
    for (size_t i = 0; i < conflatedBits.size(); i++)
    {
      const Envelope& tile = conflatedBits[i];
      if (tile.contains((it->second)->getX(), (it->second)->getY()))
      {
        RemoveNodeOp::removeNodeNoCheck(map, (it->second)->getId());
      }
    }
  }

  QString fn = dirOut + QString("/Dregs.osm");
  OsmWriter writer;
  writer.setIncludePointsInWays(true);
  writer.setIncludeHootInfo(true);
  writer.write(map, fn);
}

}
