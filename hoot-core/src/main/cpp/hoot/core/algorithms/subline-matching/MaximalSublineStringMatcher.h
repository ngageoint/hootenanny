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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef MAXIMALSUBLINESTRINGMATCHER_H
#define MAXIMALSUBLINESTRINGMATCHER_H

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/algorithms/subline-matching/SublineStringMatcher.h>

namespace hoot
{
class MaximalSublineStringMatcherTest;
class WaySublineMatchString;
class SublineMatcher;

/**
 * This class wraps the MaximalSublineMatcher class to create a matcher that can work on
 * multilinestrings.
 */
class MaximalSublineStringMatcher : public SublineStringMatcher
{
public:

  static std::string className() { return "hoot::MaximalSublineStringMatcher"; }

  static int logWarnCount;

  MaximalSublineStringMatcher();

  /**
   * @throws NeedsReviewException If the multilinestring situation is too complex to handle with
   *  a reasonable set of rules.
   */
  virtual WaySublineMatchString findMatch(const ConstOsmMapPtr& map, const ConstElementPtr& e1,
    const ConstElementPtr& e2, Meters maxRelevantDistance = -1) const;

  virtual void setConfiguration(const Settings& s);

  virtual void setMaxRelevantAngle(Radians r);
  /**
   * minSplitSize is not supported at this time.
   */
  virtual void setMinSplitSize(Meters minSplitSize);
  virtual void setHeadingDelta(Meters headingDelta);
  virtual void setSublineMatcher(const std::shared_ptr<SublineMatcher>& sm);

  virtual QString getDescription() const
  { return "Matches lines based on the maximal subline string found"; }

private:

  // for white box testing.
  friend class MaximalSublineStringMatcherTest;

  Radians _maxAngle;
  Meters _minSplitsize;
  Meters _headingDelta;
  std::shared_ptr<SublineMatcher> _sublineMatcher;

  class ScoredMatch
  {
  public:

    ScoredMatch() : score(0.0) {}
    ScoredMatch(double s, const std::vector<WaySublineMatch>& m) : score(s), matches(m) {}

    double score;
    std::vector<WaySublineMatch> matches;

    QString toString() const
    {
      QString result;
      result += QString("score: %1\n").arg(score);
      result += QString("matches:\n");
      QStringList l;
      for (size_t i = 0; i < matches.size(); i++)
      {
        l.append(matches[i].toString());
      }
      result += l.join("\n");
      return result;
    }
  };

  std::vector<WayPtr> _changeMap(const std::vector<ConstWayPtr>& ways, OsmMapPtr map) const;

  int _countReverses(const std::vector<bool>& r) const;

  ScoredMatch _evaluateMatch(const ConstOsmMapPtr &map, Meters maxDistance,
    const std::vector<ConstWayPtr>& ways1, const std::vector<ConstWayPtr>& ways2,
    const std::vector<bool>& reversed1, const std::vector<bool>& reversed2) const;

  std::vector<ConstWayPtr> _extractWays(const ConstOsmMapPtr& map, const ConstElementPtr& e) const;

  ScoredMatch _findBestMatch(const ConstOsmMapPtr& map, Meters maxDistance,
    std::vector<ConstWayPtr>& ways1, std::vector<ConstWayPtr>& ways2, std::vector<bool>& reversed1,
    std::vector<bool>& reversed2, size_t i = 0, size_t j = 0) const;

  void _insertElementIds(const std::vector<ConstWayPtr>& ways, std::set<ElementId>& elements) const;

  /*
   * ways.size() == reversed.size()
   * If reversed is true then the nodes in the corresponding way are reversed.
   */
  void _reverseWays(const std::vector<WayPtr>& ways, const std::vector<bool>& reversed) const;

  void _configureSublineMatcher();

  void _validateElement(const ConstOsmMapPtr& map, ElementId eid) const;
};

}

#endif // MAXIMALSUBLINESTRINGMATCHER_H
