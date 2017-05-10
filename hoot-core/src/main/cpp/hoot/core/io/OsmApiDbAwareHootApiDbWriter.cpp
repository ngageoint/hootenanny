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
 * @copyright Copyright (C) 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
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

void OsmApiDbAwareHootApiDbWriter::writePartial(const ConstNodePtr& n)
{
  Tags t = n->getTags();
  _addElementTags(n, t);

  long nodeId;
  LOG_VART(n->getId());
  LOG_VART(n->getStatus());
  if (/*(n->getStatus() == Status::Unknown1 || n->getStatus() == Status::Conflated) &&*/ n->getId() > 0)
  {
    nodeId = n->getId();
  }
  else
  {
    nodeId =  _getRemappedElementId(n->getElementId());
  }
  LOG_VART(nodeId);
  const bool alreadyThere = _nodeRemap.count(nodeId) != 0;
  if (alreadyThere)
  {
    _hootdb.updateNode(nodeId, n->getY(), n->getX(), n->getVersion() + 1, t);

    LOG_VART(n->getVersion() + 1);
  }
  else
  {
    _hootdb.insertNode(nodeId, n->getY(), n->getX(), t);
    _nodeRemap[nodeId] = nodeId;

    LOG_TRACE("version=1");
  }

  _countChange();
  _nodesWritten++;
}

void OsmApiDbAwareHootApiDbWriter::writePartial(const ConstWayPtr& w)
{
  Tags tags = w->getTags();
  _addElementTags(w, tags);

  long wayId;
  LOG_VART(w->getElementId());
  LOG_VART(w->getStatus());
  if (/*(w->getStatus() == Status::Unknown1 || w->getStatus() == Status::Conflated) &&*/ w->getId() > 0)
  {
    wayId = w->getId();
  }
  else
  {
    wayId =  _getRemappedElementId(w->getElementId());
  }
  const bool alreadyThere = _wayRemap.count(wayId) != 0;
  if (alreadyThere)
  {
    _hootdb.updateWay(wayId, w->getVersion() + 1, tags);
  }
  else
  {
    _hootdb.insertWay(wayId, tags);
    _wayRemap[wayId] = wayId;
  }

  if (_remapIds)
  {
    _hootdb.insertWayNodes(wayId, _remapNodes(w->getNodeIds()));
  }
  else
  {
    _hootdb.insertWayNodes(wayId, w->getNodeIds());
  }

  _countChange();
  _waysWritten++;
}

void OsmApiDbAwareHootApiDbWriter::writePartial(const ConstRelationPtr& r)
{
  Tags tags = r->getTags();
  _addElementTags(r, tags);
  if (!r->getType().isEmpty())
  {
    tags["type"] = r->getType();
  }

  long relationId;
  LOG_VART(r->getId());
  LOG_VART(r->getStatus());
  if (/*(r->getStatus() == Status::Unknown1 || r->getStatus() == Status::Conflated) &&*/ r->getId() > 0)
  {
    relationId = r->getId();
  }
  else
  {
    relationId =  _getRemappedElementId(r->getElementId());
  }
  const bool alreadyThere = _relationRemap.count(relationId) != 0;
  if (alreadyThere)
  {
    _hootdb.updateRelation(relationId, r->getVersion() + 1, tags);
  }
  else
  {
    _hootdb.insertRelation(relationId, tags);
    _relationRemap[relationId] = relationId;
  }

  for (size_t i = 0; i < r->getMembers().size(); ++i)
  {
    RelationData::Entry e = r->getMembers()[i];

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
