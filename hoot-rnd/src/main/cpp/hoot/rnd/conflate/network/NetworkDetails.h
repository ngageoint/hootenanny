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
#ifndef NETWORKDETAILS_H
#define NETWORKDETAILS_H

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/algorithms/SublineStringMatcher.h>
#include <hoot/core/conflate/highway/HighwayClassifier.h>
#include <hoot/rnd/conflate/network/EdgeString.h>
#include <hoot/rnd/conflate/network/LegacyVertexMatcher.h>
#include <hoot/rnd/conflate/network/OsmNetwork.h>
#include <hoot/rnd/conflate/network/SearchRadiusProvider.h>

namespace hoot
{

/**
 * This abstracts the details of a network (e.g. positions, bounding boxes, etc.).
 *
 * The advantage is that we don't link concepts such as OsmMap and ElementIds directly to the
 * network algorithms.
 */
class NetworkDetails : public SearchRadiusProvider
{
public:
  NetworkDetails(ConstOsmMapPtr map, ConstOsmNetworkPtr n1, ConstOsmNetworkPtr n2);

  Meters calculateLength(ConstNetworkEdgePtr e);

  ConstOsmMapPtr getMap() const { return _map; }

  double getEdgeMatchScore(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2);
  
  double getEdgeStringMatchScore(ConstEdgeStringPtr e1, ConstEdgeStringPtr e2);

  double getPartialEdgeMatchScore(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2);

  /**
   * Returns a score matching v1 to v2. This does not consider any neighboring vertices. 0 means
   * no match and larger scores are better.
   */
  double getVertexMatchScore(ConstNetworkVertexPtr v1, ConstNetworkVertexPtr v2);

  virtual Envelope getEnvelope(ConstNetworkEdgePtr e) const;

  virtual Envelope getEnvelope(ConstNetworkVertexPtr v) const;

  Meters getSearchRadius(ConstNetworkEdgePtr e1) const;

  Meters getSearchRadius(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2) const;

  Meters getSearchRadius(ConstNetworkVertexPtr v1) const;

  Meters getSearchRadius(ConstNetworkVertexPtr v1, ConstNetworkVertexPtr v2) const;

  bool hasConfidentTiePoint(ConstNetworkVertexPtr v);

  bool isReversed(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2);

  bool isCandidateMatch(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2);

  bool isCandidateMatch(ConstNetworkVertexPtr v1, ConstNetworkVertexPtr v2);

  WayStringPtr toWayString(ConstEdgeStringPtr e) const;

private:
  shared_ptr<HighwayClassifier> _classifier;
  ConstOsmMapPtr _map;
  ConstOsmNetworkPtr _n1, _n2;
  shared_ptr<SublineStringMatcher> _sublineMatcher;
  LegacyVertexMatcherPtr _vertexMatcher;

  class SublineCache
  {
  public:
    bool reversed;
    double p;
  };

  QHash< ElementId, QHash<ElementId, SublineCache> > _sublineCache;

  const SublineCache& _getSublineCache(ConstWayPtr w1, ConstWayPtr w2);

  LegacyVertexMatcherPtr _getVertexMatcher();
};

typedef shared_ptr<NetworkDetails> NetworkDetailsPtr;
typedef shared_ptr<const NetworkDetails> ConstNetworkDetailsPtr;

}

#endif // NETWORKDETAILS_H
