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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "GeometryMerger.h"

//  GEOS
#include <geos/util/IllegalArgumentException.h>

//  Hoot
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/geometry/PolygonCompare.h>
#include <hoot/core/util/ConfigOptions.h>

#include <hoot/core/util/StringUtils.h>

//  Qt



//  Standard
#include <thread>

using namespace geos::geom;
using namespace std;

namespace hoot
{

GeometryMerger::GeometryMerger()
  : _maxThreads(ConfigOptions().getCookieCutterAlphaShapeMaxThreads()),
    _threadsWorking(true),
    _finishedThreads(0),
    _geometryCount(0),
    _counter(0),
    _updateInterval(ConfigOptions().getTaskStatusUpdateInterval())
{
}

GeometryPtr GeometryMerger::mergeGeometries(std::vector<GeometryPtr> geometries, const geos::geom::Envelope& envelope)
{
  LOG_DEBUG("Merging " << StringUtils::formatLargeNumber(geometries.size()) << " geometries...");
  //  Get the geometry count
  _geometryCount = geometries.size();
  //  Update the max threads to speed up smaller datasets
  _maxThreads = std::min(_maxThreads, static_cast<int>(geometries.size() / 2));
  //  Create and start the threads
  std::vector<std::thread> threads;
  for (int i = 0; i < _maxThreads; ++i)
    threads.push_back(thread(&GeometryMerger::mergeGeometryThread, this));
  //  Wait for all threads to become active
  while (_finishedThreads < _maxThreads)
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

  PolygonCompare compare(envelope);
  //  Combine the geometries two at a time
  while (geometries.size() > 1)
  {
    // Sort polygons using the Hilbert value. This increases the chances that nearby polygons will
    // be merged early and speed up the union process.
    sort(geometries.begin(), geometries.end(), compare);
    LOG_TRACE("Remaining geometries: " << geometries.size());
    //  Reserve the correct amount of space in the thread return vector
    _geometryReturnMutex.lock();
    _geometryReturn.resize(0);
    _geometryReturn.reserve(geometries.size() / 2 + 1);
    _geometryReturnMutex.unlock();
    //  Create the stack of work
    _geometryStackMutex.lock();
    _geometryStack.empty();
    for (size_t i = 0; i < geometries.size() - 1; i += 2)
      _geometryStack.emplace(geometries[i], geometries[i + 1]);
    _geometryStackMutex.unlock();
    //  Reset finished threads to zero each round of merges
    _finishedThreads = 0;
    //  Tell the threads to start working
    _wait.notify_all();
    //  Grab the odd geometry if there is one to tack on to the end
    GeometryPtr extra;
    if (geometries.size() % 2 == 1)
      extra = geometries[geometries.size() - 1];
    //  Now that we have everything from geometries, clear it before the swap
    geometries.clear();
    //  Don't start the next round of pairings until all threads are done processing pairs
    while (!_geometryStack.empty() || _finishedThreads < _maxThreads)
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    //  Add the extra geometry on to the end
    if (extra)
      _geometryReturn.push_back(extra);
    //  Swap the returned geometries into the empty container
    geometries.swap(_geometryReturn);
  }
  //  Stop the threads
  _threadsWorking = false;
  _wait.notify_all();
  //  Wait on the threads to exit
  for (int i = 0; i < _maxThreads; ++i)
    threads[i].join();
  //  Report the final progress
  PROGRESS_INFO(
    "Merged " << StringUtils::formatLargeNumber(_counter + 1) << " of " <<
    StringUtils::formatLargeNumber(_geometryCount) << " geometries.");
  //  Return the merged geometry
  return geometries[0];
}

void GeometryMerger::mergeGeometryThread()
{
  //  Increment the thread finished count to indicate this thread is active
  _finishedThreads++;
  //  Wait to begin work until the producer thread creates some work
  std::unique_lock<std::mutex> lock(_waitMutex);
  _wait.wait(lock);
  //  Continue running until the producer thread signals the work is complete
  while (_threadsWorking)
  {
    //  Lock the mutex and try to get a geometry pair to union
    GeometryPair pair;
    _geometryStackMutex.lock();
    if (!_geometryStack.empty())
    {
      pair = _geometryStack.top();
      _geometryStack.pop();
    }
    _geometryStackMutex.unlock();
    //  Make sure that there was something taken off of the queue
    if (pair.first && pair.second)
    {
      GeometryPtr g;
      // Sometimes GEOS gives results that are incorrect. In those cases we try cleaning the
      // geometries and attempting it again.
      bool cleanAndRetry = false;
      try
      {
        //  Attempt the first union operation on the two geometries
        double area = pair.first->getArea() + pair.second->getArea();
        g = pair.first->Union(pair.second.get());
        if (g->isEmpty() || fabs(g->getArea() - area) > 0.1)
          cleanAndRetry = true;
      }
      catch (const geos::util::GEOSException& e)
      {
        LOG_TRACE("Topology error. Attempting to fix it: " << e.what());
        cleanAndRetry = true;
      }
      //  Something failed in the union, validate the geometry and retry
      if (cleanAndRetry)
      {
        pair.first.reset(GeometryUtils::validateGeometry(pair.first.get()));
        pair.second.reset(GeometryUtils::validateGeometry(pair.second.get()));
        try
        {
          g = pair.first->Union(pair.second.get());
        }
        // if the cleaning didn't fix the problem
        catch(const geos::util::GEOSException& e)
        {
          // report an error
          QString error = "Error unioning two geometries. " + QString(e.what()) + "\n" +
              "geom1: " + QString::fromStdString(pair.first->toString()) + "\n" +
              "geom2: " + QString::fromStdString(pair.second->toString());
          throw HootException(error);
        }
      }
      //  Push the geometry onto the return vector safely
      _geometryReturnMutex.lock();
      _geometryReturn.push_back(g);
      _geometryReturnMutex.unlock();
      //  Increment the counter
      ++_counter;
      if (_counter % _updateInterval == 0)
      {
        //  Because this is a potentially long operation and it gets more complex
        //  as more and more geometries are joined, reduce the update interval
        //  by a factor of 10 as _counter nears _geometryCount
        if (_geometryCount - _counter < _updateInterval && _updateInterval > 10)
          _updateInterval = _updateInterval / 10;
        PROGRESS_INFO(
          "Merged " << StringUtils::formatLargeNumber(_counter) << " of " <<
          StringUtils::formatLargeNumber(_geometryCount) << " geometries.");
      }
    }
    else
    {
      //  Increment the thread finished count
      _finishedThreads++;
      //  Wait until the next round is ready
      _wait.wait(lock);
    }
  }
}

}
