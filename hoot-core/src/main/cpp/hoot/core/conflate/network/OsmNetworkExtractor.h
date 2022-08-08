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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef OSMNETWORKEXTRACTOR_H
#define OSMNETWORKEXTRACTOR_H

// hoot
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/conflate/network/OsmNetwork.h>
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

class OsmNetworkExtractorVisitor;

/**
 * Traverse the OSM Map and extract a graph that represents some network. E.g. a road network, a
 * hydrography network, etc.
 */
class OsmNetworkExtractor
{
public:

  static QString className() { return "OsmNetworkExtractor"; }

  OsmNetworkExtractor() = default;
  ~OsmNetworkExtractor() = default;

  OsmNetworkPtr extractNetwork(ConstOsmMapPtr map);

  /**
   * What criterion must be satisified to include an element in this graph? For instance, status
   * is unknown1 and HighwayCriterion is satisfied.
   *
   * This criterion should only let linear types through. No arbitrary collections or nodes.
   */
  void setCriterion(ElementCriterionPtr criterion) { _criterion = criterion; }

private:

  static int logWarnCount;

  friend class OsmNetworkExtractorVisitor;

  ElementCriterionPtr _criterion;
  ConstOsmMapPtr _map;
  OsmNetworkPtr _network;

  void _addEdge(ConstElementPtr from, ConstElementPtr to, QList<ConstElementPtr> members,
    bool directed) const;

  /**
   * This is a very strict definition of contiguous. We're looking to make sure that the first
   * member connects to the second, connects to the third, etc. We could be more liberal, but we
   * can let that be use case driven.
   */
  bool _isContiguous(const ConstRelationPtr& r);
  bool _isContiguous(const ConstRelationPtr& r, long& lastNode);

  void _getFirstLastNodes(const ConstRelationPtr& r, ElementId& first, ElementId& last);

  bool _isValidElement(const ConstElementPtr& e) const;

  void _visit(const ConstElementPtr& e);
};

}

#endif // OSMNETWORKEXTRACTOR_H
