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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef MATCH_H
#define MATCH_H

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/conflate/matching/MatchMembers.h>
#include <hoot/core/info/ApiEntityInfo.h>

namespace hoot
{

class MatchThreshold;
class ElementId;
class MatchType;
class MatchClassification;
class Match;

using MatchPtr = std::shared_ptr<Match>;
using ConstMatchPtr = std::shared_ptr<const Match>;

/**
 * Describes a specific match between two sets of elements. e.g. the match between two
 * buildings or the match between an intersection and a round-a-bout.
 *
 * This class is not re-entrant or thread safe.
 */
class Match : public ApiEntityInfo
{
public:

  static QString className() { return "Match"; }

  Match() = default;
  virtual ~Match() = default;

  virtual QString explain() const;
  virtual void setExplain(const QString& /*explainText*/) {}

  /**
   * Classifies the match and returns a classification object.
   */
  virtual const MatchClassification& getClassification() const = 0;

  /**
   * Optionally return the types of members stored in this match. This can be multiple member types
   * OR'd together.
   */
  virtual MatchMembers getMatchMembers() const { return MatchMembers::None; }

  /**
   * TODO: This already exists in ApiEntityInfo
   */
  virtual QString getName() const = 0;

  /**
   * Returns the score associated with this match. Score is a bit abstract at this point and may
   * end up being feature specific. In the case of roads it is the probability * length.
   */
  virtual double getScore() const { return getProbability(); }

  /**
   * Should return a proper probability of this being a match given only the two elements as input.
   * E.g. ignores other matches that may manipulate this probability.
   */
  virtual double getProbability() const = 0;

  /**
   * Returns true if this match conflicts with "other". In other words, if both of these matches
   * cannot be applied to a single OsmMap then true is returned.
   *
   * Two matches can only be conflicting if they contain the same ElementIds in getMatchPairs().
   *
   * @param other match to check for conflict with
   * @param map map owning the elements involved in the matches
   * @param matches an optional set of all matches found during conflation; this allows match
   * caching to be used in situations where duplicated match calculation is prohibitively expensive
   * @return true if the two matches are conflicting; false otherwise
   */
  virtual bool isConflicting(
    const std::shared_ptr<const Match>& other, const ConstOsmMapPtr& map,
    const QHash<QString, ConstMatchPtr>& matches = QHash<QString, ConstMatchPtr>()) const = 0;

  /**
   * If the match should _not_ be optimized into a group of non-conflicting matches, then this
   * returns true. If one member of a sub-graph of matches specifies whole group, then every match
   * in the whole group will be taken together and not optimized.
   *
   * This is most handy when defining features that should be reviewed if a conflict exists. This
   * is not a good idea with highly connected features such as roads. Experimentation with
   * this has occurred with POIs and buildings (redmine #3003).
   */
  virtual bool isWholeGroup() const { return false; }

  /**
   * Returns a set of pairs that this match represents. For instance, if this match represents
   * a merge lane on a highway then there are three ways involved in each input data set. The
   * lower part of the highway, the upper part of the highway and the merge lane. Each of these
   * ways will be matched to an equivalent way in the other data set so this will return a set
   * of three pairs.
   *
   * In some cases, such as buildings, a single element may be matched multiple times and show up
   * in multiple pairs.
   *
   * In general Unknown1 should be the status of the first element and Unknown2 the status of the
   * second element.
   */
  virtual std::set<std::pair<ElementId, ElementId>> getMatchPairs() const = 0;

  /**
   * Returns this match's type.
   *
   * @return a match type
   */
  virtual MatchType getType() const;

  /**
   * Returns a collection of matches indexed by the IDs of the elements involved
   *
   * @param matches the matches to index
   * @return an indexed collection of matches
   */
  static QHash<QString, ConstMatchPtr> getIdIndexedMatches(
    const std::vector<ConstMatchPtr>& matches);

  /**
   * String representation for match pairs
   *
   * @param matchPairs the match pairs to represent as a string
   * @return a string
   */
  static QString matchPairsToString(
    const std::set<std::pair<ElementId, ElementId>>& matchPairs);

  /**
   * @todo This already exists in ApiEntityInfo
   */
  virtual QString toString() const = 0;

  std::shared_ptr<const MatchThreshold> getThreshold() const { return _threshold; }

protected:

  friend class MatchPtrComparator;

  static long _orderCount;
  long _order;

  const std::shared_ptr<const MatchThreshold> _threshold;

  ElementId _eid1, _eid2;

  Match(const std::shared_ptr<const MatchThreshold>& threshold);
  Match(
    const std::shared_ptr<const MatchThreshold>& threshold, const ElementId& eid1,
    const ElementId& eid2);
};

inline std::ostream& operator<<(std::ostream & o, ConstMatchPtr m)
{
  o << m->toString().toStdString();
  return o;
}

/**
 * Use this comparator whenever a match pointer is placed into an ordered container.
 */
struct MatchPtrComparator
{
  bool operator() (ConstMatchPtr m1, ConstMatchPtr m2) const
  {
    return m1->_order < m2->_order;
  }
};

}

#endif // MATCH_H
