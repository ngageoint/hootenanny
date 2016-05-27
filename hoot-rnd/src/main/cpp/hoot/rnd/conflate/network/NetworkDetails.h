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
#include <hoot/rnd/conflate/network/OsmNetwork.h>

namespace hoot
{

/**
 * This abstracts the details of a network (e.g. positions, bounding boxes, etc.).
 *
 * The advantage is that we don't link concepts such as OsmMap and ElementIds directly to the
 * network algorithms.
 */
class NetworkDetails
{
public:
  NetworkDetails(ConstOsmMapPtr map, ConstOsmNetworkPtr network);

  double getEdgeMatchScore(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2);
  
  double getPartialEdgeMatchScore(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2);

  Envelope getEnvelope(ConstNetworkEdgePtr e);

  Envelope getEnvelope(ConstNetworkVertexPtr v);

  Meters getSearchRadius(ConstNetworkEdgePtr e);

  Meters getSearchRadius(ConstNetworkVertexPtr v);

  bool isReversed(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2);

  bool isCandidateMatch(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2);

  bool isCandidateMatch(ConstNetworkVertexPtr v1, ConstNetworkVertexPtr v2);

private:
  shared_ptr<HighwayClassifier> _classifier;
  ConstOsmMapPtr _map;
  ConstOsmNetworkPtr _network;
  shared_ptr<SublineStringMatcher> _sublineMatcher;
};

typedef shared_ptr<NetworkDetails> NetworkDetailsPtr;
typedef shared_ptr<const NetworkDetails> ConstNetworkDetailsPtr;

}

#endif // NETWORKDETAILS_H
