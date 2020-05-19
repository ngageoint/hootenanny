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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef MERGEFACTORY_H
#define MERGEFACTORY_H

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/conflate/merging/MergerCreator.h>
#include <hoot/core/conflate/matching/MatchSet.h>

// Qt
#include <QString>

// Standard
#include <set>
#include <vector>

namespace hoot
{

class Match;
class Merger;

/**
 * A factory for creating a merge from a set of matches (Singleton).
 */
class MergerFactory
{
public:

  static std::string className() { return "hoot::ScriptMerger"; }

  static int logWarnCount;

  static QString mergerCreatorsKey() { return "merger.creators"; }

  ~MergerFactory();

  /**
   * Searches through all the available creators in order to create the requested merge. If no
   * appropriate creator is found a NULL will be returned.
   *
   * The caller assumes ownership of the new Mergers in the result.
   */
  void createMergers(const hoot::OsmMapPtr &map, const MatchSet& matches,
    std::vector<MergerPtr>& result) const;

  /**
   * @brief Returns a description of all available match creators.
   */
  std::vector<CreatorDescription> getAllAvailableCreators() const;

  /**
   * A singleton is provided, but not required.
   */
  static MergerFactory& getInstance();

  /**
   * Determines if two matches are conflicting
   *
   * @param map map owning the elements involved in the matches
   * @param m1 the first match
   * @param m2 the second match
   * @param matches an optional set of all matches found during conflation; this allows match
   * caching to be used in situations where duplicated match calculation is prohibitively expensive
   * @return true if the two matches are conflicting; false otherwise
   */
  bool isConflicting(
    const ConstOsmMapPtr& map,  const ConstMatchPtr& m1, const ConstMatchPtr& m2,
    const QHash<QString, ConstMatchPtr>& matches = QHash<QString, ConstMatchPtr>()) const;

  /**
   * Registers the specified creator with the MergeFactory and takes ownership of the creator.
   */
  void registerCreator(MergerCreatorPtr creator) { _creators.push_back(creator); }

  void registerDefaultCreators();

  void reset();

private:

  // Since this is a Singleton, we shouldn't be accessing its constructor, but there are a some
  // spots where we are.  This is here to limit any further constructor access.
  friend class UnifyingConflator;
  friend class MultiaryUtilities;

  MergerFactory();

  static std::shared_ptr<MergerFactory> _theInstance;

  std::vector<MergerCreatorPtr> _creators;
};

}

#endif // MERGEFACTORY_H
