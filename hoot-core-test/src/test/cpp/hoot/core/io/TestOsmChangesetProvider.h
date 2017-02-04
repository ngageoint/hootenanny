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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef OSMCHANGESETTESTPROVIDER_H
#define OSMCHANGESETTESTPROVIDER_H

// Hoot
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/io/ChangesetProvider.h>
#include <hoot/core/io/ApiDb.h>

// TGS
#include <tgs/Statistics/Random.h>
using namespace Tgs;

namespace hoot
{

/**
 * @brief The SqlTestChangesetProvider class
 * Implementation of ChangeSetProvider that psuedo-randomly creates, modifies, and deletes
 * nodes, ways, and relations for use with both the SQL and OSM changeset writers.
 */
class TestOsmChangesetProvider : public ChangeSetProvider
{
public:
  /**
   * Constructor
   *
   * @param useCoordScale SQL changeset provider uses the ApiDb::COORDINATE_SCALE
   *  while the OSM changeset does not
   */
  TestOsmChangesetProvider(bool useCoordScale)
    : _ctr(0), _max(10), _node(0), _way(0), _rel(0), _coordinateScale(1.0)
  {
    Random::instance()->seed(0);
    if (useCoordScale)
      _coordinateScale = ApiDb::COORDINATE_SCALE;
  }

  ~TestOsmChangesetProvider() { }

  boost::shared_ptr<OGRSpatialReference> getProjection() const
  {
    return boost::shared_ptr<OGRSpatialReference>();
  }

  void close() { }

  bool hasMoreChanges() { return _ctr < _max; }

  Change readNextChange()
  {
    Change change;
    change.type = (Change::ChangeType)(Random::instance()->generateInt() % 3);

    switch ((ElementType::Type)(Random::instance()->generateInt() % 3))
    {
    default:
    case ElementType::Node:
      {
        NodePtr node(new Node(Status::Unknown1, ++_node, getLon(), getLat(), 15.0));
        change.e = node;
      }
      break;
    case ElementType::Way:
      {
        NodePtr node1(new Node(Status::Unknown1, ++_node, getLon(), getLat(), 15.0));
        NodePtr node2(new Node(Status::Unknown1, ++_node, getLon(), getLat(), 15.0));
        WayPtr way(new Way(Status::Unknown1, ++_way, 15.0));
        way->addNode(node1->getId());
        way->addNode(node2->getId());
        way->setTag("key1", "value1");
        change.e = way;
      }
      break;
    case ElementType::Relation:
      {
        NodePtr node1(new Node(Status::Unknown1, ++_node, getLon(), getLat(), 15.0));
        NodePtr node2(new Node(Status::Unknown1, ++_node, getLon(), getLat(), 15.0));
        WayPtr way(new Way(Status::Unknown1, ++_way, 15.0));
        way->addNode(node1->getId());
        way->addNode(node2->getId());
        way->setTag("key1", "value1");
        RelationPtr relation(new Relation(Status::Unknown1, ++_rel, 15.0));
        relation->addElement("role1", node1->getElementId());
        relation->addElement("role2", way->getElementId());
        relation->setTag("key2", "value2");
        change.e = relation;
      }
      break;
    }
    _ctr++;
    return change;
  }

private:
  double getLat() { return (Random::instance()->generateInt() % 180 -  90.0) / _coordinateScale; }
  double getLon() { return (Random::instance()->generateInt() % 360 - 180.0) / _coordinateScale; }

  int _ctr;
  int _max;
  int _node;
  int _way;
  int _rel;
  double _coordinateScale;

};

}

#endif // OSMCHANGESETTESTPROVIDER_H
