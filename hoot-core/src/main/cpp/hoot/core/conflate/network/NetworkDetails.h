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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef NETWORKDETAILS_H
#define NETWORKDETAILS_H

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/algorithms/linearreference/WaySublineMatchString.h>
#include <hoot/core/conflate/network/EdgeMatch.h>
#include <hoot/core/conflate/network/EdgeString.h>
#include <hoot/core/conflate/network/EdgeSublineMatch.h>
#include <hoot/core/conflate/network/EidMapper.h>
#include <hoot/core/conflate/network/LegacyVertexMatcher.h>
#include <hoot/core/conflate/network/OsmNetwork.h>
#include <hoot/core/conflate/network/SearchRadiusProvider.h>
#include <hoot/core/util/Configurable.h>


namespace hoot
{

class SublineStringMatcher;
class HighwayClassifier;

/**
 * This abstracts the details of a network (e.g. positions, bounding boxes, etc.).
 *
 * The advantage is that we don't link concepts such as OsmMap and ElementIds directly to the
 * network algorithms.
 */
class NetworkDetails : public SearchRadiusProvider, public Configurable
{
public:

  static QString className() { return "hoot::NetworkDetails"; }

  NetworkDetails(ConstOsmMapPtr map, ConstOsmNetworkPtr n1, ConstOsmNetworkPtr n2);
  virtual ~NetworkDetails() = default;

  Meters calculateDistance(ConstEdgeLocationPtr el) const;
  /**
   * Returns the distance from el to s.
   */
  Meters calculateDistance(ConstEdgeStringPtr s, ConstEdgeLocationPtr el) const;

  Meters calculateLength(ConstNetworkEdgePtr e) const;
  Meters calculateLength(ConstEdgeSublinePtr e) const;
  Meters calculateLength(ConstEdgeStringPtr e) const;

  Radians calculateHeading(ConstEdgeLocationPtr el) const;

  /**
   * Callers to this method are responsible for filtering out invalid edges.
   */
  Radians calculateHeadingAtVertex(ConstNetworkEdgePtr e, ConstNetworkVertexPtr v) const;

  QList<EdgeSublineMatchPtr> calculateMatchingSublines(ConstNetworkEdgePtr e1,
    ConstNetworkEdgePtr e2);

  /**
   * calculate the distance relative to the string start. This may be negative. An attempt will
   * be made to calculate the distance to el even if it isn't part of es, but only if el1 is
   * part of an edge that is contained by es.
   *
   * If a valid distance can't be reasonable found numeric_limits<double>::max() will be returned.
   */
  Meters calculateStringLocation(ConstEdgeStringPtr es, ConstEdgeLocationPtr el1) const;

  /**
   * Run an experiment to see if a valid match is created by adding esm onto em. If a valid match
   * is created it will be returned. Otherwise a null is returned.
   */
  EdgeMatchPtr extendEdgeMatch(ConstEdgeMatchPtr em, ConstNetworkEdgePtr e1,
    ConstNetworkEdgePtr e2) const;

  /**
   * Extend edge string 'es' with 'e'. 'es' will be enlarged as necessary to include e. If e
   * cannot extend 'es' an exception will be thrown.
   */
  void extendEdgeString(EdgeStringPtr es, ConstNetworkEdgePtr e) const;

  QList<ConstNetworkVertexPtr> getCandidateMatches(ConstNetworkVertexPtr v);

  /**
   * Calculate the nearest location on string to subline. If subline isn't on an edge that
   * intersects string then elString and elSubline will be null.
   *
   * If string overlaps subline then an arbitrary point within the overlapping region will be
   * returned. You are guaranteed in this case that elString == elSubline.
   */
  void calculateNearestLocation(ConstEdgeStringPtr string, ConstEdgeSublinePtr subline,
    ConstEdgeLocationPtr &elString, ConstEdgeLocationPtr &elSubline) const;

  double getEdgeMatchScore(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2);

  double getEdgeStringMatchScore(ConstEdgeStringPtr e1, ConstEdgeStringPtr e2);

  virtual geos::geom::Envelope getEnvelope(ConstNetworkEdgePtr e) const;

  virtual geos::geom::Envelope getEnvelope(ConstNetworkVertexPtr v) const;

  ConstOsmMapPtr getMap() const { return _map; }

  ConstOsmNetworkPtr getNetwork1() const { return _n1; }

  double getPartialEdgeMatchScore(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2);

  Meters getSearchRadius(ConstNetworkEdgePtr e1) const;

  Meters getSearchRadius(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2) const;

  Meters getSearchRadius(ConstNetworkVertexPtr v1) const;

  Meters getSearchRadius(ConstNetworkVertexPtr v1, ConstNetworkVertexPtr v2) const;

  Meters getSearchRadius(ConstWayStringPtr ws1, ConstWayStringPtr ws2) const;

  Meters getSearchRadius(ConstWayPtr w1, ConstWayPtr w2) const;

  /**
   * Returns a score matching v1 to v2. This does not consider any neighboring vertices. 0 means
   * no match and larger scores are better.
   */
  double getVertexMatchScore(ConstNetworkVertexPtr v1, ConstNetworkVertexPtr v2);

  bool hasConfidentTiePoint(ConstNetworkVertexPtr v);

  bool isCandidateMatch(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2);

  bool isCandidateMatch(ConstNetworkVertexPtr v1, ConstNetworkVertexPtr v2);

  /**
   * Starting at v1, v2, are e1 and e2 partial edge match candidates?
   *
   * E.g.
   *
   *     a----1---b
   *       c----2---d
   *
   * isPartialCandidateMatch(a, c, 1, 2) == true
   * isPartialCandidateMatch(a, d, 1, 2) == false
   */
  bool isPartialCandidateMatch(ConstNetworkVertexPtr v1, ConstNetworkVertexPtr v2,
    ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2);

  bool isReversed(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2);

  /**
   * Returns true if e1 and e2 could be connected as a string.
   *
   * Returns true if the angle is reasonable between the two edges for a continous edge. Hard angles
   * generally add too much noise.
   */
  bool isStringCandidate(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2) const;

  bool isStringCandidate(ConstEdgeStringPtr es, ConstEdgeSublinePtr esl) const;

  ConstWayPtr toWay(ConstNetworkEdgePtr e) const;

  WayStringPtr toWayString(ConstEdgeStringPtr e, const EidMapper& mapper = EidMapper()) const;

  virtual void setConfiguration(const Settings& conf);

private:

  static int logWarnCount;

  std::shared_ptr<HighwayClassifier> _classifier;
  ConstOsmMapPtr _map;
  ConstOsmNetworkPtr _n1, _n2;
  std::shared_ptr<SublineStringMatcher> _sublineMatcher;
  LegacyVertexMatcherPtr _vertexMatcher;
  double _maxStubLength;

  class SublineCache
  {
    public:

      QString toString() const
      {
        return  "p: " + QString::number(p) + " " + matches->toString();
      }

      double p;
      WaySublineMatchStringPtr matches;
  };

  QHash<ElementId, QHash<ElementId, SublineCache>> _sublineCache;

  SublineCache _calculateSublineScore(ConstOsmMapPtr map, ConstWayPtr w1, ConstWayPtr w2) const;

  /**
   * Assuming e1 & e2 match at v1 and v2 respectively, score the edges for match based on their
   * relative angles.
   * @param v1 - vertex in e1
   * @param v2 - vertex in e2
   * @return
   */
  double _getEdgeAngleScore(ConstNetworkVertexPtr v1, ConstNetworkVertexPtr v2,
    ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2);

  const SublineCache& _getSublineCache(ConstWayPtr w1, ConstWayPtr w2);

  LegacyVertexMatcherPtr _getVertexMatcher();

  EdgeSublinePtr _toEdgeSubline(const WaySubline& ws, ConstNetworkEdgePtr);

  void _trimEdgeString(ConstElementProviderPtr provider, EdgeStringPtr es, WayPtr w,
    const WaySublineCollection& ws) const;
};

typedef std::shared_ptr<NetworkDetails> NetworkDetailsPtr;
typedef std::shared_ptr<const NetworkDetails> ConstNetworkDetailsPtr;

// not implemented
bool operator<(ConstNetworkDetailsPtr, ConstNetworkDetailsPtr);

}

#endif // NETWORKDETAILS_H
