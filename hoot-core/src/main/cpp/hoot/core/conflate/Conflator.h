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
#ifndef CONFLATOR_H
#define CONFLATOR_H

// Local Includes
#include "WorkingMap.h"
#include <hoot/core/manipulators/Manipulation.h>

// Boost Includes
#include <boost/shared_ptr.hpp>

// GDAL
class OGRSpatialReference;

// Standard Includes
#include <deque>
#include <map>
#include <queue>
#include <set>
#include <vector>

// Qt
#include <QString>

namespace hoot
{
  using namespace boost;
  using namespace std;

  class OsmMap;
  class Manipulator;

/**
 * Conflates a map made up of Unknown1, Unknown2 and Conflated elements into a new map. An attempt
 * is made to conflate Unknown1 and Unknown2 elements into Conflated elements. If there is no
 * valid match or the element isn't recognized then it is simply passed through the system.
 *
 * The conflation process is made up of a set of manipulators that identify specific manipulations
 * that can be applied to a map. Change the set of manipulators can dramatically impact the
 * conflation process. For instance the WayMerger manipulator only operates on highways and the
 * BuildingMergeManipulator only operates on buildings.
 *
 * To conflate the conflator uses the specified manipulators to create all the reasonably possible
 * manipulations and then applies the manipulations in a greedy fashion. Unfortunately this requires
 * that the scores on the manipulations must be in the same scale.
 *
 * NOTE: Some old operations may strip unknown elements out during the conflation process. These
 * operations should be cleaned when they're found to conform to the newer approach of keeping all
 * unrecognized elements.
 */
class Conflator
{
public:

  /**
   * Comparison struct for the priority queue. Puts the highest scoring results on top.
   */
  struct GreaterWorkingMap
    : public std::binary_function<
              boost::shared_ptr<const WorkingMap>,
              boost::shared_ptr<const WorkingMap>, bool>
  {
  public:


    bool operator()(boost::shared_ptr<const WorkingMap>& left,
                    boost::shared_ptr<const WorkingMap>& right) const
    {
      return (left->getScore() < right->getScore());
    }
  };

  /**
   * Comparison struct for the priority queue. Puts the lowest scoring results on top.
   */
  struct LesserWorkingMap
    : public std::binary_function<
              boost::shared_ptr<const WorkingMap>,
              boost::shared_ptr<const WorkingMap>, bool>
  {
  public:


    bool operator()(boost::shared_ptr<const WorkingMap>& left,
                    boost::shared_ptr<const WorkingMap>& right) const
    {
      return (left->getScore() > right->getScore());
    }
  };

  static QString createBogusReviewTagsKey() { return "create.bogus.review.tags"; }
  static QString manipulatorsKey() { return "conflator.manipulators"; }

  Conflator();

  /**
   * Load the source data up into memory for later conflation.
   */
  void loadSource(boost::shared_ptr<OsmMap> map);

  /**
   * Conflate the input values into an output map.
   */
  void conflate();

  boost::shared_ptr<const OsmMap> getMap() const { return _map; }

  boost::shared_ptr<const OsmMap> getBestMap() const { return _bestMap->getMap(); }

  /**
   * Replaces the collection of manipulators that will be used to conflate this map.
   */
  void setManipulators(const deque< boost::shared_ptr<Manipulator> >& m) { _manipulators = m; }

  void setMaxIterations(int i) { _maxIterations = i; }

  void setDebugging(bool debug) { _debug = debug; }

  /**
   * Sets the source without performing any pre-processing.
   */
  void setSource(boost::shared_ptr<OsmMap> map);

protected:

  bool _debug;
  int _maxIterations;
  double _bestScore;
  Meters _vectorError, _maxDividedSeparation;
  priority_queue<
          boost::shared_ptr<const WorkingMap>,
          deque< boost::shared_ptr<const WorkingMap> >,
          GreaterWorkingMap > _searchHeap;

  priority_queue<
        ManipulationHolder,
        deque< ManipulationHolder >,
        LessThanManipulationHolder > _manipulationHeap;

  deque< boost::shared_ptr<Manipulator> > _manipulators;
  vector< boost::shared_ptr<Manipulation> > _manipulations;
  boost::shared_ptr<OsmMap> _map;
  boost::shared_ptr<WorkingMap> _bestMap;
  boost::shared_ptr<OGRSpatialReference> _planarSrs;
  map< ElementId, set< boost::shared_ptr<Manipulation>, LessThanManipulation > > _impacted2Manipulation;

  double _minValidScore;
  bool _createBogusReviewTags;

  void _applyManipulations();

  void _createManipulations();

  /**
   * Create manipulations for the specified ways.
   */
  void _createManipulations(boost::shared_ptr<OsmMap> map, const set<ElementId>& eids);

  void _saveMap(QString path);

  void _updateManipulationEstimates(boost::shared_ptr<const OsmMap> map, const set<ElementId>& eids);
};

}

#endif // CONFLATOR_H
