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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef MATCHFACTORY_H
#define MATCHFACTORY_H

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/conflate/matching/Match.h>
#include <hoot/core/conflate/matching/MatchCreator.h>
#include <hoot/core/util/Configurable.h>

// GEOS
#include <geos/geom/Geometry.h>

namespace hoot
{

class MatchThreshold;

/**
 * (Singleton)
 */
class MatchFactory : public Configurable
{
public:

  /**
   * Returns the default MatchFactory with the default creators registered.
   */
  static MatchFactory& getInstance();

  void setConfiguration(const Settings& s) override;

  /**
   * Create a match object for the specified element IDs. If the element IDs aren't recognized as
   * a match by any MatchCreator a null is returned. Not all MatchCreators are guaranteed to support
   * this mechanism (e.g. a road that matches two one way streets).
   */
  MatchPtr createMatch(const ConstOsmMapPtr& map, ElementId eid1, ElementId eid2) const;

  /**
   * Goes through all registered MatchCreators and calls createMatches in the order the creators
   * were registered.
   */
  void createMatches(const ConstOsmMapPtr& map, std::vector<ConstMatchPtr> &matches,
    const std::shared_ptr<geos::geom::Geometry>& bounds,
    std::shared_ptr<const MatchThreshold> threshold = std::shared_ptr<MatchThreshold>()) const;

  /**
   * @brief Returns a description of all available match creators.
   */
  std::vector<CreatorDescription> getAllAvailableCreators() const;

  /**
   * @return Return all of the registered match creators. This is the list of creators that will
   * be used if createMatches is called.
   */
  std::vector<std::shared_ptr<MatchCreator>> getCreators() const { return _creators; }

  /**
   * Registers the specified creator with the MergeFactory and takes ownership of the creator.
   */
  void registerCreator(const std::shared_ptr<MatchCreator>& creator)
  { _creators.push_back(creator); }

  /**
   * @brief registerCreator Register the specified creator by string (constructs the creator)
   * @param c @sa ConfigOptions::getMatchCreators()
   */
  void registerCreator(const QString& c);

  /**
   * Removes all the creators from the factory
   */
  void reset();

  /**
   * Return the list of creators as a string
   */
  QString toString() const;

private:

  // These allow for matching a subset of the input data.
  QString _tagFilterJson;
  QString _critFilterClassName;
  bool _negateCritFilter;

  std::vector<std::shared_ptr<MatchCreator>> _creators;

  MatchFactory();
  ~MatchFactory() = default;

  void _checkMatchCreatorBoundable(const std::shared_ptr<MatchCreator>& matchCreator,
                                   const std::shared_ptr<geos::geom::Geometry>& bounds) const;
  void _setMatchCreators(const QStringList& matchCreatorsList);
  ElementCriterionPtr _createFilter() const;
  void _setTagFilterJson(QString json) { _tagFilterJson = json; }

  friend class MatchCandidateCountVisitorTest;
  friend class TestUtils;
};

}

#endif // MATCHFACTORY_H
