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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "OsmApiDbAwareHootApiDbWriter.h"

// hoot
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapWriter, OsmApiDbAwareHootApiDbWriter)

OsmApiDbAwareHootApiDbWriter::OsmApiDbAwareHootApiDbWriter() :
HootApiDbWriter()
{
}

OsmApiDbAwareHootApiDbWriter::~OsmApiDbAwareHootApiDbWriter()
{
  _osmApiDb.close();
}

void OsmApiDbAwareHootApiDbWriter::open(QString urlStr)
{
  if (!_osmApiDb.isSupported(ConfigOptions().getOsmapidbIdAwareUrl()))
  {
    throw HootException(
      "The OsmApiDbAwareHootApiDbWriter requires a valid OSM API DB URL for element id sequencing.");
  }

  HootApiDbWriter::open(urlStr);
  _osmApiDb.open(ConfigOptions().getOsmapidbIdAwareUrl());
}

long OsmApiDbAwareHootApiDbWriter::_getRemappedElementId(const ElementId& eid)
{
  long retVal = -1;

  switch (eid.getType().getEnum())
  {
    case ElementType::Node:

      if (_nodeRemap.count(eid.getId()) == 1)
      {
        retVal = _nodeRemap.at(eid.getId());
      }
      else
      {
        retVal = _osmApiDb.getNextId(ElementType::Node);
        _nodeRemap[eid.getId()] = retVal;
        if (_outputMappingFile.length() > 0)
        {
          _sourceNodeIds.insert(eid.getId());
        }
      }

      break;

    case ElementType::Way:

      if (_wayRemap.count(eid.getId()) == 1)
      {
        retVal = _wayRemap.at(eid.getId());
      }
      else
      {
        retVal = _osmApiDb.getNextId(ElementType::Way);

        _wayRemap[eid.getId()] = retVal;
        if ( _outputMappingFile.length() > 0 )
        {
          _sourceWayIds.insert(eid.getId());
        }
      }

      break;

    case ElementType::Relation:

      if (_relationRemap.count(eid.getId()) == 1)
      {
        retVal = _relationRemap.at(eid.getId());
      }
      else
      {
        retVal = _osmApiDb.getNextId(ElementType::Relation);
        _relationRemap[eid.getId()] = retVal;
        if ( _outputMappingFile.length() > 0 )
        {
          _sourceRelationIds.insert(eid.getId());
        }
      }

      break;

  default:

    LOG_ERROR("Tried to create or remap ID for invalid type");
    throw NotImplementedException();

    break;
  }

  LOG_TRACE("Remapped ID from " << eid << " to " << ElementId(eid.getType(), retVal));

  return retVal;
}

// a lot of similar code in these write methods that could be consolidated into something
// generic

void OsmApiDbAwareHootApiDbWriter::writePartial(const ConstNodePtr& node)
{
  ElementPtr pe( node->clone() );
  NodePtr nodeClone = boost::dynamic_pointer_cast<Node>(pe);

  long nodeId;
  LOG_VART(nodeClone->getId());
  LOG_VART(nodeClone->getStatus());
  if (nodeClone->getId() > 0)
  {
    nodeId = nodeClone->getId();
  }
  else
  {
    nodeId = _getRemappedElementId(nodeClone->getElementId());
  }

  // since it's now a copy for writing, just assign the new id
  nodeClone->setId(nodeId);

  LOG_VART(nodeId);

  _addExportTagsVisitor.visit(nodeClone);

  Tags tags = nodeClone->getTags();

  const bool alreadyThere = _nodeRemap.count(nodeId) != 0;
  if (alreadyThere)
  {
    _hootdb.updateNode(nodeId, nodeClone->getY(), nodeClone->getX(), nodeClone->getVersion() + 1, tags);

    LOG_VART(nodeClone->getVersion() + 1);
  }
  else
  {
    _hootdb.insertNode(nodeId, nodeClone->getY(), nodeClone->getX(), tags);
    _nodeRemap[nodeId] = nodeId;

    LOG_TRACE("version=1");
  }

  _countChange();
  _nodesWritten++;
}

void OsmApiDbAwareHootApiDbWriter::writePartial(const ConstWayPtr& way)
{
  ElementPtr pe( way->clone() );
  WayPtr wayClone = boost::dynamic_pointer_cast<Way>(pe);

  long wayId;
  LOG_VART(wayClone->getElementId());
  LOG_VART(wayClone->getStatus());
  if (wayClone->getId() > 0)
  {
    wayId = wayClone->getId();
  }
  else
  {
    wayId = _getRemappedElementId(wayClone->getElementId());
  }

  // since it's now a copy for writing, just assign the new id
  wayClone->setId(wayId);

  _addExportTagsVisitor.visit(wayClone);

  Tags tags = wayClone->getTags();

  const bool alreadyThere = _wayRemap.count(wayId) != 0;
  if (alreadyThere)
  {
    _hootdb.updateWay(wayId, wayClone->getVersion() + 1, tags);
  }
  else
  {
    _hootdb.insertWay(wayId, tags);
    _wayRemap[wayId] = wayId;
  }

  if (_remapIds)
  {
    _hootdb.insertWayNodes(wayId, _remapNodes(wayClone->getNodeIds()));
  }
  else
  {
    _hootdb.insertWayNodes(wayId, wayClone->getNodeIds());
  }

  _countChange();
  _waysWritten++;
}

void OsmApiDbAwareHootApiDbWriter::writePartial(const ConstRelationPtr& relation)
{
  ElementPtr pe( relation->clone() );
  RelationPtr relationClone = boost::dynamic_pointer_cast<Relation>(pe);

  long relationId;
  LOG_VART(relationClone->getId());
  LOG_VART(relationClone->getStatus());
  if (relationClone->getId() > 0)
  {
    relationId = relationClone->getId();
  }
  else
  {
    relationId = _getRemappedElementId(relationClone->getElementId());
  }

  // since it's now a copy for writing, just assign the new id
  relationClone->setId(relationId);

  _addExportTagsVisitor.visit(relationClone);

  Tags tags = relationClone->getTags();

  if (!relationClone->getType().isEmpty())
  {
    tags["type"] = relationClone->getType();
  }

  const bool alreadyThere = _relationRemap.count(relationId) != 0;
  if (alreadyThere)
  {
    _hootdb.updateRelation(relationId, relationClone->getVersion() + 1, tags);
  }
  else
  {
    _hootdb.insertRelation(relationId, tags);
    _relationRemap[relationId] = relationId;
  }

  for (size_t i = 0; i < relationClone->getMembers().size(); ++i)
  {
    RelationData::Entry e = relationClone->getMembers()[i];

    // May need to create new ID mappings for items we've not yet seen
    ElementId relationMemberElementId = e.getElementId();

    if (_remapIds)
    {
      relationMemberElementId =
        ElementId(relationMemberElementId.getType(), _getRemappedElementId(relationMemberElementId));
    }

    _hootdb.insertRelationMember(relationId, relationMemberElementId.getType(),
                                 relationMemberElementId.getId(), e.role, i);
  }

  _countChange();
  _relationsWritten++;
}

}
