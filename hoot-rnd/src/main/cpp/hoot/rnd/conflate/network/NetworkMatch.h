#ifndef NETWORKMATCH_H
#define NETWORKMATCH_H

// hoot
#include <hoot/core/conflate/Match.h>
#include <hoot/core/conflate/MatchThreshold.h>
#include <hoot/core/conflate/highway/HighwayMatch.h>

#include "EdgeMatch.h"

namespace hoot
{

/**
 * Represents a match between two edges in a network.
 */
class NetworkMatch : public Match
{
public:
  NetworkMatch(const ConstOsmMapPtr& map, ConstEdgeMatchPtr edgeMatch,
    double score, ConstMatchThresholdPtr mt);

  /**
   * Classifies the match and returns a classification object.
   */
  virtual const MatchClassification& getClassification() const { return _classification; }

  /**
   * This may require modification if the network matcher ever matches things besides lines. E.g.
   * river polygons.
   */
  virtual MatchMembers getMatchMembers() const { return MatchMembers::Polyline; }

  /**
   * As new network matching routines are introduced this will need to be modified. E.g. Railway
   */
  virtual QString getMatchName() const { return HighwayMatch::getHighwayMatchName(); }

  virtual double getScore() const { return getProbability(); }

  virtual double getProbability() const { return _classification.getMatchP(); }

  /**
   * Returns true if any of the elements in this are also in other's match pairs.
   */
  virtual bool isConflicting(const Match& other, const ConstOsmMapPtr& map) const;

  virtual bool isWholeGroup() const { return true; }

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
  virtual set< pair<ElementId, ElementId> > getMatchPairs() const { return _pairs; }

  virtual QString toString() const;

  virtual MatchType getType() const { return _threshold->getType(getClassification()); }

protected:
  void _discoverWayPairs(ConstOsmMapPtr map, ConstEdgeMatchPtr edgeMatch);

  ConstWayPtr _toWay(ConstNetworkEdgePtr edge) const;

private:
  MatchClassification _classification;
  ConstEdgeMatchPtr _edgeMatch;
  set< pair<ElementId, ElementId> > _pairs;
};

}

#endif // NETWORKMATCH_H
