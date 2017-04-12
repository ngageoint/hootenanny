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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "FourPassManager.h"

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/OsmMapListener.h>
#include <hoot/core/conflate/NodeReplacements.h>
#include <hoot/core/conflate/OutsideBoundsRemover.h>
#include <hoot/core/ops/SuperfluousNodeRemover.h>
#include <hoot/core/conflate/TileBoundsCalculator.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/NotImplementedException.h>
#include <hoot/core/util/Log.h>

// Standard
#include <set>

// TGS
#include <tgs/System/Time.h>

namespace hoot
{
using namespace geos::geom;

FourPassManager::FourPassManager(boost::shared_ptr<TileWorker2> worker)
{
  _worker = worker;
  // set the buffer to ~5km.
  _buffer = 0.05;
  _maxNodesPerBox = 5000000;
}

FourPassManager::~FourPassManager()
{
  // clean up all temporary files that may still be around.
  if (_worker->exists(_worker->getWorkDir()))
  {
    _worker->rmdir(_worker->getWorkDir());
  }
}

void FourPassManager::apply(QString outputPath)
{
  if (_worker->exists(outputPath))
  {
    _worker->rmdir(outputPath);
  }
  QString tmpStr = outputPath;
  tmpStr.replace(QRegExp("/+$"), "");
  _wayCache = tmpStr + "-cache/ways";
  LOG_INFO("Way cache is: " << _wayCache);

  // make the max way size a little smaller than our buffer.
  _worker->init(_input1, _input2, _buffer);
  _envelope = _worker->calculateEnvelope();
  _iteration = -1;

  // determine the best split points for the tile bounds.
  {
    // r1 & r2 can be quite large. (~2.5GB each w/ 1km pixel size over the globe)
    cv::Mat r1, r2;
    _worker->calculateNodeDensity(r1, r2);
    _determineTileBounds(r1, r2);
  }

  // break any way that is larger than buffer into buffer size pieces and merge the files together.
  if (_worker->exists(_wayCache))
  {
    LOG_INFO("Using ways cached in: " << _wayCache);
    LOG_INFO("If you want to recalculate the cache then remove it before the conflation run.");
    _createNextScratch();
  }
  else
  {
    _createNextScratch();
    _worker->breakWays(_scratchOut);
  }

  for (size_t i = 0; i < 4; i++)
  {
    _iteration = i;
    _createNextScratch();

    vector<Envelope> appliedBits;

    // Cleanup and conflate every fourth tile together with overlap. No two tiles will overlap
    // during one pass of the data.
    for (size_t x = i % 2; x < _tiles.size(); x += 2)
    {
      for (size_t y = i / 2; y < _tiles[0].size(); y += 2)
      {
        Envelope e = _tiles[x][y];
        for (size_t j = 0; j < appliedBits.size(); j++)
        {
          if (e.intersects(appliedBits[j]))
          {
            LOG_TRACE("Overlapping env 1: " << e.toString());
            LOG_TRACE("Overlapping env 2: " << appliedBits[j].toString());
            throw HootException("Two tiles overlap.");
          }
        }
        appliedBits.push_back(e);
      }
    }

    _worker->applyOp(_op, appliedBits, _scratchIn, _scratchOut);
  }

  _worker->cleanup(_scratchOut, outputPath);
  _worker->rmdir(_scratchIn);
  _worker->rmdir(_scratchOut);
}

void FourPassManager::_createNextScratch()
{
  // if there is something old to delete and we're not using the way cache
  if (_scratchIn.isEmpty() == false && _scratchIn != _wayCache)
  {
    //#   warning Not removing scratch
    _worker->rmdir(_scratchIn);
  }

  _mapPart = 0;
  _nodeReplacementPart = 0;
  _scratchIn = _scratchOut;
  QString name;
  if (_iteration == -1)
  {
    name = _wayCache;
  }
  else
  {
    name = _worker->getWorkDir() + "/" + QString("TileConflateScratch-%1").arg(_iteration);
    if (_worker->exists(name))
    {
      _worker->rmdir(name);
    }
  }

  _scratchOut = name;
  _worker->mkdir(_scratchOut);
}

void FourPassManager::_determineTileBounds(const cv::Mat& r1, const cv::Mat& r2)
{
  double start = Tgs::Time::getTime();
  TileBoundsCalculator tbc(_buffer);

  tbc.setMaxNodesPerBox(_maxNodesPerBox);
  tbc.setSlop(0.10);
  tbc.setEnvelope(_envelope);
  if (_input2.isEmpty())
  {
    cv::Mat zeros = cv::Mat::zeros(r1.size(), r1.type());
    tbc.setImages(r1, zeros);
  }
  else
  {
    tbc.setImages(r1, r2);
  }

  _tiles = tbc.calculateTiles();
  double end = Tgs::Time::getTime();
  LOG_INFO("Determine tile bounds took " << end - start << " seconds.");
}

void FourPassManager::setSource(QString input1)
{
  _input1 = input1;
  _input2.clear();
}

void FourPassManager::setSources(QString input1, QString input2)
{
  _input1 = input1;
  _input2 = input2;
}

}
