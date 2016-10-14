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
#include "OsmNetworkExtractor.h"

#include <hoot/core/elements/Element.h>
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/schema/OsmSchema.h>

namespace hoot
{

class OsmNetworkExtractorVisitor : public ElementVisitor
{
public:

  OsmNetworkExtractorVisitor(OsmNetworkExtractor& parent) : _parent(parent) {}

  virtual void visit(const ConstElementPtr& e)
  {
    _parent._visit(e);
  }

private:
  OsmNetworkExtractor& _parent;
};

OsmNetworkExtractor::OsmNetworkExtractor()
{
}

void OsmNetworkExtractor::_addEdge(ConstElementPtr from, ConstElementPtr to,
  QList<ConstElementPtr> members, bool directed)
{
  ConstNetworkVertexPtr v1 = _network->getSingleVertex(from->getElementId());
  if (!v1.get())
  {
    v1.reset(new NetworkVertex(from));
    _network->addVertex(v1);
  }
  ConstNetworkVertexPtr v2 = _network->getSingleVertex(to->getElementId());
  if (!v2.get())
  {
    v2.reset(new NetworkVertex(to));
    _network->addVertex(v2);
  }

  NetworkEdgePtr edge(new NetworkEdge(v1, v2, directed));
  edge->setMembers(members);

  _network->addEdge(edge);
}

OsmNetworkPtr OsmNetworkExtractor::extractNetwork(ConstOsmMapPtr map)
{
  _network.reset(new OsmNetwork());

  _map = map;
  // go through all the elements.
  OsmNetworkExtractorVisitor v(*this);
  map->visitRo(v);

  return _network;
}

bool OsmNetworkExtractor::_isContiguous(const ConstRelationPtr& r)
{
  assert(OsmSchema::getInstance().isLinear(*r));

  const vector<RelationData::Entry>& members = r->getMembers();
  bool result = members.size() > 0;
  long lastNode;
  for (size_t i = 0; i < members.size(); ++i)
  {
    ElementId eid = members[i].getElementId();
    assert(eid.getType() == ElementType::Way);
    ConstWayPtr w = _map->getWay(eid);
    if (i > 0)
    {
      if (w->getFirstNodeId() != lastNode)
      {
        result = false;
      }
    }
    lastNode = w->getLastNodeId();
  }

  return result;
}

bool OsmNetworkExtractor::_isValidElement(const ConstElementPtr& e)
{
  bool result = true;
  if (e->getElementType() == ElementType::Node)
  {
    result = false;
  }
  else if (e->getElementType() == ElementType::Relation)
  {
    ConstRelationPtr r = dynamic_pointer_cast<const Relation>(e);
    if (OsmSchema::getInstance().isLinear(*e) == false)
    {
      LOG_WARN("Received a non-linear relation as a valid network element. Ignoring relation. " <<
        e);
      result = false;
    }
    else
    {
      const vector<RelationData::Entry>& members = r->getMembers();
      for (size_t i = 0; i < members.size(); ++i)
      {
        if (members[i].getElementId().getType() != ElementType::Way)
        {
          LOG_WARN("Received a linear relation that contains a non-linear element: " << e);
        }
      }
    }
  }

  return result;
}

void OsmNetworkExtractor::_visit(const ConstElementPtr& e)
{
  if (_criterion->isSatisfied(e) && _isValidElement(e))
  {
    QList<ConstElementPtr> members;
    ElementId from, to;

    if (e->getElementType() == ElementType::Way)
    {
      members.append(e);
      ConstWayPtr w = dynamic_pointer_cast<const Way>(e);
      from = ElementId::node(w->getFirstNodeId());
      to = ElementId::node(w->getLastNodeId());
    }
    else if (e->getElementType() == ElementType::Relation)
    {
      members.append(e);
      ConstRelationPtr r = dynamic_pointer_cast<const Relation>(e);

      if (_isContiguous(r))
      {
        const vector<RelationData::Entry>& members = r->getMembers();
        from = ElementId::node(_map->getWay(members[0].getElementId())->getFirstNodeId());
        to = ElementId::node(_map->getWay(members[members.size() - 1].getElementId())->
          getLastNodeId());
      }
      // if this is a bad multi-linestring then don't include it in the network.
      else
      {
        LOG_WARN("Found a non-contiguous relation when extracting a network. Ignoring: " << e);
        return;
      }
    }

    bool directed = false;
    if (OsmSchema::getInstance().isOneWay(*e))
    {
      directed = true;
      if (OsmSchema::getInstance().isReversed(*e))
      {
        swap(from, to);
      }
    }

    _addEdge(_map->getNode(from), _map->getNode(to), members, directed);
  }
}

}
