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
#ifndef MERGEFACTORY_H
#define MERGEFACTORY_H

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/conflate/MergerCreator.h>
#include <hoot/core/conflate/match-graph/MatchGraph.h>

// Qt
#include <QString>

// Standard
#include <set>
#include <vector>

namespace hoot
{
using namespace std;

class Match;
class Merger;

/**
 * A factory for creating a merge from a set of matches.
 */
class MergerFactory
{
public:

  static QString mergerCreatorsKey() { return "merger.creators"; }

  /**
   * A custom merge factory can be created or the global singleton can be used.
   */
  MergerFactory();

  ~MergerFactory();

  /**
   * Should be avoided on the global instance. Mostly useful for unit testing.
   */
  void clear();

  /**
   * Searches through all the available creators in order to create the requested merge. If no
   * appropriate creator is found a NULL will be returned.
   *
   * The caller assumes ownership of the new Mergers in the result.
   */
  void createMergers(const hoot::OsmMapPtr &map, const MatchSet& matches,
    vector<Merger *> &result) const;

  /**
   * @brief Returns a description of all available match creators.
   */
  vector<MergerCreator::Description> getAllAvailableCreators() const;

  /**
   * A singleton is provided, but not required.
   */
  static MergerFactory& getInstance();

  bool isConflicting(const ConstOsmMapPtr& map, const Match* m1, const Match* m2) const;

  /**
   * Registers the specified creator with the MergeFactory and takes ownership of the creator.
   */
  void registerCreator(MergerCreator* creator) { _creators.push_back(creator); }

  void registerDefaultCreators();

  void reset() { _creators.clear(); }

private:
  static shared_ptr<MergerFactory> _theInstance;

  vector<MergerCreator*> _creators;
};

}

#endif // MERGEFACTORY_H
