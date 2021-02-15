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
#ifndef NETWORKMATCH_H
#define NETWORKMATCH_H

// hoot
#include <hoot/core/conflate/highway/HighwayMatch.h>
#include <hoot/core/conflate/matching/Match.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/conflate/network/EdgeMatch.h>
#include <hoot/core/conflate/network/NetworkDetails.h>

namespace hoot
{

/**
 * Represents a match between two edges in a network.
 */
class NetworkMatch : public Match
{
public:

  static QString className() { return "hoot::NetworkMatch"; }

  NetworkMatch() = default;
  NetworkMatch(const ConstNetworkDetailsPtr& details, ConstEdgeMatchPtr edgeMatch,
    double score, ConstMatchThresholdPtr mt, double scoringFunctionMax,
    double scoringFunctionCurveMidpointX, double scoringFunctionCurveSteepness);
  virtual ~NetworkMatch() = default;

  /**
   * Classifies the match and returns a classification object.
   */
  virtual const MatchClassification& getClassification() const override { return _classification; }

  ConstNetworkDetailsPtr getNetworkDetails() const { return _details; }

  ConstEdgeMatchPtr getEdgeMatch() const { return _edgeMatch; }

  /**
   * This may require modification if the network matcher ever matches things besides lines. E.g.
   * river polygons.
   */
  virtual MatchMembers getMatchMembers() const override { return MatchMembers::Polyline; }

  /**
   * As new network matching routines are introduced this will need to be modified. E.g. Railway
   */
  virtual QString getName() const override { return HighwayMatch::MATCH_NAME; }

  virtual QString getClassName() const override { return className(); }

  virtual double getScore() const override { return getProbability(); }

  virtual double getProbability() const override { return _classification.getMatchP(); }

  /**
   * Returns true if any of the elements in this are also in other's match pairs.
   */
  virtual bool isConflicting(
    const ConstMatchPtr& other, const ConstOsmMapPtr& map,
    const QHash<QString, ConstMatchPtr>& matches = QHash<QString, ConstMatchPtr>()) const override;

  /**
   * Returns true if either of the matched strings contains a non-whole edge.
   */
  bool isPartialMatch() const { return _edgeMatch->containsPartial(); }

  virtual bool isWholeGroup() const override { return true; }

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
  virtual std::set<std::pair<ElementId, ElementId>> getMatchPairs() const override { return _pairs; }

  virtual QString toString() const override;

  virtual MatchType getType() const override { return _threshold->getType(getClassification()); }

  bool isVerySimilarTo(const NetworkMatch* other) const;

  bool contains(const NetworkMatch* other) const;

  virtual QString getDescription() const override { return "Matches roads with the Network Algorithm"; }

protected:

  void _discoverWayPairs(ConstOsmMapPtr map, ConstEdgeMatchPtr edgeMatch);

  ConstElementPtr _toElement(ConstNetworkEdgePtr edge) const;

private:

  MatchClassification _classification;
  ConstNetworkDetailsPtr _details;
  ConstEdgeMatchPtr _edgeMatch;
  std::set<std::pair<ElementId, ElementId>> _pairs;
};

}

#endif // NETWORKMATCH_H
