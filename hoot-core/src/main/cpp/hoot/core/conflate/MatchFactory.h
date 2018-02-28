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
#ifndef MATCHFACTORY_H
#define MATCHFACTORY_H

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/conflate/MatchCreator.h>

// Standard
#include <vector>

namespace hoot
{

class Match;
class MergeCreator;
class MatchThreshold;

class MatchFactory
{
public:

  MatchFactory();

  ~MatchFactory();

  /**
   * Returns the default MatchFactory with the default creators registered.
   */
  static MatchFactory& getInstance();

  /**
   * Create a match object for the specified element IDs. If the element IDs aren't recognized as
   * a match by any MatchCreator a null is returned. Not all MatchCreators are guaranteed to support
   * this mechanism (e.g. a road that matches two one way streets).
   */
  Match* createMatch(const ConstOsmMapPtr& map, ElementId eid1, ElementId eid2) const;

  /**
   * Goes through all registered MatchCreators and calls createMatches in the order the creators
   * were registered.
   */
  void createMatches(const ConstOsmMapPtr& map, std::vector<const Match *> &matches,
    const geos::geom::Envelope &bounds,
    boost::shared_ptr<const MatchThreshold> threshold = boost::shared_ptr<MatchThreshold>()) const;

  /**
   * @brief Returns a description of all available match creators.
   */
  std::vector<MatchCreator::Description> getAllAvailableCreators() const;

  /**
   * @return Return all of the registered match creators. This is the list of creators that will
   * be used if createMatches is called.
   */
  std::vector< boost::shared_ptr<MatchCreator> > getCreators() const { return _creators; }

  /**
   * Registers the specified creator with the MergeFactory and takes ownership of the creator.
   */
  void registerCreator(boost::shared_ptr<MatchCreator> creator) { _creators.push_back(creator); }

  /**
   * @brief registerCreator Register the specified creator by string (constructs the creator)
   * @param c @sa ConfigOptions::getMatchCreators()
   */
  void registerCreator(QString c);

  void reset() { _creators.clear(); }

  /**
   * Returns the number of candidate matches for all registered match creators.
   *
   * @return the match candidate count
   */
  long getMatchCandidateCount(const ConstOsmMapPtr& map, const geos::geom::Envelope& bounds);

private:

  void _checkMatchCreatorBoundable(boost::shared_ptr<MatchCreator> matchCreator,
                                   const geos::geom::Envelope& bounds) const;
  static void _setMatchCreators(QStringList matchCreatorsList);

  static boost::shared_ptr<MatchFactory> _theInstance;

  std::vector< boost::shared_ptr<MatchCreator> > _creators;

  friend class MatchCandidateCountVisitorTest;
  friend class MatchCandidateCountVisitorRndTest;
};

}

#endif // MATCHFACTORY_H
