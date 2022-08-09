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
#ifndef HIGHWAYMATCHCREATOR_H
#define HIGHWAYMATCHCREATOR_H

// hoot
#include <hoot/core/conflate/matching/MatchCreator.h>

namespace hoot
{

class HighwayClassifier;
class SublineStringMatcher;
class TagAncestorDifferencer;

class HighwayMatchCreator : public MatchCreator
{

public:

  static QString className() { return "HighwayMatchCreator"; }

  HighwayMatchCreator();
  ~HighwayMatchCreator() = default;

  /**
   * Create the match
   */
  MatchPtr createMatch(const ConstOsmMapPtr&, ElementId eid1, ElementId eid2) override;

  /**
   * Search the provided map for highway matches and add the matches to the matches vector.
   */
  void createMatches(const ConstOsmMapPtr& map, std::vector<ConstMatchPtr>& matches,
                     ConstMatchThresholdPtr threshold) override;

  std::vector<CreatorDescription> getAllCreators() const override;

  /**
   * Determines whether an element is a candidate for matching for this match creator
   *
   * @param element element to determine the match candidate status of
   * @param map the map the element whose candidacy is being determined belongs to
   * @return true if the element is a match candidate; false otherwise
   */
  bool isMatchCandidate(ConstElementPtr element, const ConstOsmMapPtr& map) override;

  std::shared_ptr<MatchThreshold> getMatchThreshold() override;

  QString getName() const override { return className(); }

  /**
   * @see FilteredByGeometryTypeCriteria
   */
  QStringList getCriteria() const override;

  /**
   * @brief setRunMedianMatching toggles activation of the custom road median matching routine.
   * @param runMatching turns median matching on/off
   * @param identifyingTags tags used to identify secondary road median features (only one must be
   * satisfied)
   * @param transferKeys tag keys transferred from matching secondary road median features to
   * reference road median features
   */
  void setRunMedianMatching(const bool runMatching, const QStringList& identifyingTags, const QStringList& transferKeys);

private:

  // standard classifier used for road matching
  std::shared_ptr<HighwayClassifier> _classifier;
  // classifier used for the road median to divided road tag transfer workflow
  std::shared_ptr<HighwayClassifier> _medianClassifier;
  // geometry matcher
  std::shared_ptr<SublineStringMatcher> _sublineMatcher;
  // threshold controlling the determined match classification
  std::shared_ptr<MatchThreshold> _matchThreshold;
  // used to prevent roads of widely different types from matching
  std::shared_ptr<TagAncestorDifferencer> _tagAncestorDiff;
};

}

#endif // HIGHWAYMATCHCREATOR_H
