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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef DIRECTEDGRAPH_H
#define DIRECTEDGRAPH_H

// Boost
#include <boost/shared_ptr.hpp>

// Qt
#include <QMultiHash>

namespace hoot
{

  class Way;
  class OsmMap;

  using namespace boost;

class DirectedGraph
{
public:

  class Edge
  {
  public:
    Edge(long from, long to, double weight)
    {
      this->from = from;
      this->to = to;
      this->weight = weight;
    }

    long from;
    long to;
    double weight;
  };

  DirectedGraph();

  virtual ~DirectedGraph() {}

  void addEdge(long from, long to, double weight);

  /**
   * The input map must be in a planar projection with a unit of meters. It is also assumed that
   * IntersectionSplitter has been run on the map.
   */
  void deriveEdges(boost::shared_ptr<const OsmMap> map);

  const QMultiHash<long, Edge>& getEdges() const { return _edges; }

  /**
   * Determine the cost of a way in cost per meter.
   */
  virtual double determineCost(boost::shared_ptr<Way> way);

  virtual bool isOneWay(boost::shared_ptr<Way> way);

protected:

  QMultiHash<long, Edge> _edges;

  double _mphToSecondsPerMeter(double mph) { return 2.23694 / mph; }
};

}

#endif // DIRECTEDGRAPH_H
