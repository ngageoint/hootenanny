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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef NETWORKMERGER_H
#define NETWORKMERGER_H

// hoot
#include <hoot/core/conflate/merging/MergerBase.h>
#include <hoot/core/conflate/network/EdgeMatch.h>
#include <hoot/core/conflate/network/NetworkDetails.h>

namespace hoot
{

/**
 * Merges whole network pairs (no partials).
 *
 * In the case of network matches we're guaranteed there is no overlap between matches so we can
 * use some of the functions in HighwayMerger, but others are too complex/imprecise.
 */
class NetworkMerger : public MergerBase
{
public:

  static std::string className() { return "hoot::NetworkMerger"; }

  NetworkMerger() = default;
  virtual ~NetworkMerger() = default;
  /**
   * Constructed with a set of element matching pairs. The pairs are generally Unknown1 as first
   * and Unknown2 as second.
   */
  NetworkMerger(const std::set<std::pair<ElementId, ElementId>>& pairs, ConstEdgeMatchPtr edgeMatch,
    ConstNetworkDetailsPtr details);

  virtual void apply(const OsmMapPtr& map,
                     std::vector<std::pair<ElementId, ElementId>>& replaced) override;

  virtual QString toString() const override;

  virtual QString getDescription() const { return "Merges roads matched by the Network Algorithm"; }

  virtual QString getName() const { return QString::fromStdString(className()); }

protected:

  virtual PairsSet& _getPairs() override { return _pairs; }
  virtual const PairsSet& _getPairs() const override { return _pairs; }

private:

  std::set<std::pair<ElementId, ElementId>> _pairs;
  ConstEdgeMatchPtr _edgeMatch;
  ConstNetworkDetailsPtr _details;
};

}

#endif // NETWORKMERGER_H
