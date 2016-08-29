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
#ifndef NETWORKMERGER_H
#define NETWORKMERGER_H

// hoot
#include <hoot/core/algorithms/linearreference/WayMatchStringMapping.h>
#include <hoot/core/conflate/MergerBase.h>

#include "EdgeMatch.h"
#include "NetworkDetails.h"

namespace hoot
{

/**
 * Merges whole network pairs (no partials).
 *
 * In the case of network matches we're guaranteed there is no overlap between matches so we can
 * use some of the functions in HighwaySnapMerger, but others are too complex/imprecise.
 */
class NetworkMerger : public MergerBase
{
public:
  /**
   * Constructed with a set of element matching pairs. The pairs are generally Unknown1 as first
   * and Unknown2 as second.
   */
  NetworkMerger(const set< pair<ElementId, ElementId> >& pairs, ConstEdgeMatchPtr edgeMatch,
    ConstNetworkDetailsPtr details);

  virtual void apply(const OsmMapPtr& map, vector< pair<ElementId, ElementId> >& replaced) const;

  virtual QString toString() const;

protected:
  virtual PairsSet& getPairs() { return _pairs; }
  virtual const PairsSet& getPairs() const { return _pairs; }

private:
  set< pair<ElementId, ElementId> > _pairs;
  ConstEdgeMatchPtr _edgeMatch;
  ConstNetworkDetailsPtr _details;
};

}

#endif // NETWORKMERGER_H
