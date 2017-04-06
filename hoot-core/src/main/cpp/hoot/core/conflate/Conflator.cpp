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
#include "Conflator.h"

// Boost Includes
using namespace boost;

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/conflate/MapCleaner.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/core/ops/SuperfluousNodeRemover.h>
#include <hoot/core/conflate/SuperfluousWayRemover.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/manipulators/DanglerRemover.h>
#include <hoot/core/manipulators/DividedHighwayMerger.h>
#include <hoot/core/manipulators/WayMerger.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/manipulators/Manipulator.h>
using namespace hoot;


// Qt
#include <qdebug.h>
#include <QDir>
#include <QTime>

// Standard Includes
#include <limits>
#include <iostream>
using namespace std;

// TGS
#include <tgs/StreamUtils.h>
#include <tgs/System/Time.h>
using namespace Tgs;

namespace hoot
{

Conflator::Conflator()
{
  _debug = false;
  _vectorError = 15.0;
  _maxDividedSeparation = 25.0;
  _createBogusReviewTags = ConfigOptions().getCreateBogusReviewTags();

  QStringList manipulatorNames = ConfigOptions().getConflatorManipulators();

  for (int i = 0; i < manipulatorNames.size(); i++)
  {
    Manipulator* m = Factory::getInstance().constructObject<Manipulator>(manipulatorNames[i]);
    _manipulators.push_back(boost::shared_ptr<Manipulator>(m));
  }

  _maxIterations = -1;
  _minValidScore = ConfigOptions().getConflatorMinValidScore();
}

void Conflator::_applyManipulations()
{
  // create the starting working map
  boost::shared_ptr<WorkingMap> start(new WorkingMap(_map));

  if (_debug)
  {
    FileUtils::removeDir("/tmp/conflated");
    QDir().mkpath("/tmp/conflated");
    _saveMap(QString("/tmp/conflated/start.osm"));
  }


  _bestScore = start->getScore();
  _bestMap = start;

  _searchHeap.push(start);

  int iteration = 0;
  int noImprovement = 0;

  double lastPrint = Time::getTime();
  LOG_INFO("Conflating map. Node count: " << _bestMap->getMap()->getNodes().size() <<
           " way count: " <<  _bestMap->getMap()->getWays().size());

  boost::shared_ptr<const WorkingMap> map = start;

  QTime t;
  t.start();

  LOG_DEBUG("Manipulation heap " << _manipulationHeap.size());

  // go through all the manipulators
  while (_manipulationHeap.size() > 0)
  {
    if (Log::getInstance().isInfoEnabled() && Time::getTime() - lastPrint >= 1.0)
    {
      lastPrint = Time::getTime();
      cout << "Current Score: " << map->getScore() << " it: " << iteration << " remaining: " <<
              _manipulationHeap.size() << " elapsed: " << t.elapsed() << "ms             \r";
      cout.flush();
    }

    noImprovement++;

    ManipulationHolder mh = _manipulationHeap.top();
    boost::shared_ptr<const Manipulation> m = mh.m;
    _manipulationHeap.pop();
    // apply the manipulator to our candidate
    if (m->getProbabilityEstimate() > _minValidScore && m->getProbabilityEstimate() == mh.score &&
        m->isValid(map->getMap()))
    {
      set<ElementId> impactedElements;
      set<ElementId> newElements;

      boost::shared_ptr<OsmMap> newMap = map->takeMap();

      m->applyManipulation(newMap, impactedElements, newElements);
      boost::shared_ptr<WorkingMap> newWm(new WorkingMap(newMap));
      newWm->setScore(map->getScore() + m->getScoreEstimate());

      _updateManipulationEstimates(newMap, impactedElements);
      _createManipulations(newMap, newElements);

      if (newWm->getScore() > _bestScore)
      {
        _bestScore = newWm->getScore();
        _bestMap = newWm;
        noImprovement = 0;
      }

      map = newWm;
    }
    else if (_createBogusReviewTags && m->getProbabilityEstimate() == mh.score &&
             m->isValid(map->getMap()))
    {
      boost::shared_ptr<OsmMap> newMap = map->takeMap();
      boost::shared_ptr<WorkingMap> newWm(new WorkingMap(newMap));
      m->addBogusReviewTags(newMap);
      map = newWm;
      _bestMap = newWm;
    }

    if (_debug)
    {
      _saveMap(QString("/tmp/conflated/conf%1.osm").arg(iteration, 3, 10, QChar('0')));
    }

    iteration++;
  }

  if (Log::getInstance().isInfoEnabled())
  {
    cout << endl;
  }

  LOG_INFO("Conflating map. Node count: " << _bestMap->getMap()->getNodes().size() <<
           " way count: " <<  _bestMap->getMap()->getWays().size());

  // remove unnecessary fluff before exporting.
  boost::shared_ptr<OsmMap> tmp = _bestMap->takeMap();
  SuperfluousWayRemover::removeWays(tmp);

  LOG_INFO("Conflating map. Node count: " << tmp->getNodes().size() <<
           " way count: " <<  tmp->getWays().size());

  _bestMap.reset(new WorkingMap(tmp));
}

/**
 * Load the source data up into memory for later conflation.
 */
void Conflator::loadSource(boost::shared_ptr<OsmMap> map)
{
  LOG_DEBUG("Preprocessing inputs...");

  _map = map;

  if (_debug)
  {
    _saveMap(QString("/tmp/conflated/pre.osm"));
  }

  MapCleaner().apply(_map);

  if (_debug)
  {
    _saveMap(QString("/tmp/conflated/post.osm"));
  }

  setSource(_map);
}

/**
 * Conflate the input values into an output map.
 */
void Conflator::conflate()
{
  LOG_DEBUG("Looking for manipulations...");
  _createManipulations();
  LOG_DEBUG("Initial manipulation count: " << _manipulations.size());

  _applyManipulations();
}

void Conflator::_createManipulations(boost::shared_ptr<OsmMap> map, const set<ElementId>& eids)
{
  vector<ElementId> vwids(eids.size());
  for (set<ElementId>::const_iterator it = eids.begin(); it != eids.end(); ++it)
  {
    vwids.push_back(*it);
  }

  vector< boost::shared_ptr<Manipulation> > newM;

  // go through all the manipulators
  for (size_t i = 0; i < _manipulators.size(); i++)
  {
    // initialize the manipulator
    const vector< boost::shared_ptr<Manipulation> >& m =
        _manipulators[i]->findManipulations(map, vwids);

    if (_createBogusReviewTags)
    {
      for (size_t i = 0; i < m.size(); i++)
      {
        m[i]->setAddBogusReviewTags(true);
      }
    }

    newM.insert(newM.begin(), m.begin(), m.end());
  }

  for (size_t i = 0; i < newM.size(); i++)
  {
    if (newM[i]->isValid(map))
    {
      const set<ElementId> ids = newM[i]->getImpactedElementIds(map);
      for (set<ElementId>::iterator it = ids.begin(); it != ids.end(); it++)
      {
        ElementId eid = *it;
        _impacted2Manipulation[eid].insert(newM[i]);
      }
      _manipulationHeap.push(ManipulationHolder(newM[i], newM[i]->getProbabilityEstimate()));
    }
  }

  _manipulations.insert(_manipulations.end(), newM.begin(), newM.end());
  //sort(_manipulations.begin(), _manipulations.end(), GreaterThanManipulation());

}

void Conflator::_createManipulations()
{
  // handy for debugging and screen captures.
  if (_maxIterations != 0)
  {
    // go through all the manipulators
    for (size_t i = 0; i < _manipulators.size(); i++)
    {
      // initialize the manipulator
      const vector< boost::shared_ptr<Manipulation> >& m =
          _manipulators[i]->findAllManipulations(_map);
      _manipulations.insert(_manipulations.begin(), m.begin(), m.end());
    }
  }

  for (size_t j = 0; j < _manipulations.size(); j++)
  {
    if (_createBogusReviewTags)
    {
      _manipulations[j]->setAddBogusReviewTags(true);
    }
    _manipulationHeap.push(ManipulationHolder(_manipulations[j],
                                              _manipulations[j]->getProbabilityEstimate()));
  }

  //sort(_manipulations.begin(), _manipulations.end(), GreaterThanManipulation());

  for (size_t i = 0; i < _manipulations.size(); i++)
  {
    const set<ElementId> ids = _manipulations[i]->getImpactedElementIds(_map);
    for (set<ElementId>::iterator it = ids.begin(); it != ids.end(); it++)
    {
      ElementId eid = *it;
      _impacted2Manipulation[eid].insert(_manipulations[i]);
    }
  }
}

void Conflator::_saveMap(QString path)
{
  LOG_DEBUG("Writing debug .osm file..." << path.toStdString());

  boost::shared_ptr<OsmMap> wgs84(new OsmMap(_map));
  MapProjector::projectToWgs84(wgs84);
  OsmXmlWriter writer;
  writer.setIncludeHootInfo(true);
  writer.setIncludeIds(true);
  writer.write(wgs84, path);
}

void Conflator::setSource(boost::shared_ptr<OsmMap> map)
{
  _bestScore = std::numeric_limits<double>::min();

  _map = map;
}

void Conflator::_updateManipulationEstimates(boost::shared_ptr<const OsmMap> map,
  const set<ElementId>& eids)
{
  set< boost::shared_ptr<Manipulation>, LessThanManipulation > found;
  for (set<ElementId>::const_iterator it = eids.begin(); it != eids.end(); it++)
  {
    const set< boost::shared_ptr<Manipulation>, LessThanManipulation >& s = _impacted2Manipulation[*it];
    found.insert(s.begin(), s.end());
  }

  for (set< boost::shared_ptr<Manipulation> >::iterator it = found.begin(); it != found.end(); it++)
  {
    boost::shared_ptr<Manipulation> m = *it;
    if (m->isValid(map))
    {
      double oldValue = m->getProbabilityEstimate();
      m->updateEstimate(map);
      if (oldValue != m->getProbabilityEstimate())
      {
        _manipulationHeap.push(ManipulationHolder(m, m->getProbabilityEstimate()));
      }
    }
  }
}


} // hoot
