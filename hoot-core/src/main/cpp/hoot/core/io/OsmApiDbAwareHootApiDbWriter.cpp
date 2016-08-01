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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "OsmApiDbAwareHootApiDbWriter.h"

// hoot
#include <hoot/core/Factory.h>

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

  //LOG_DEBUG("Remapped ID for element type " << eid.getType().toString() << " from " <<
            //eid.getId() << " to " << retVal);

  return retVal;
}

void OsmApiDbAwareHootApiDbWriter::writePartial(const shared_ptr<const Node>& n)
{
  Tags t = n->getTags();
  _addElementTags(n, t);

  const long nodeId = _getRemappedElementId(n->getElementId());
  const bool alreadyThere = _nodeRemap.count(nodeId) != 0;
  if (alreadyThere)
  {
    _hootdb.updateNode(nodeId, n->getY(), n->getX(), n->getVersion() + 1, t);
  }
  else
  {
    _hootdb.insertNode(nodeId, n->getY(), n->getX(), t);
  }

  _countChange();
  _nodesWritten++;
}

void OsmApiDbAwareHootApiDbWriter::writePartial(const shared_ptr<const Way>& w)
{
  Tags tags = w->getTags();
  _addElementTags(w, tags);

  const long wayId = _getRemappedElementId(w->getElementId());
  const bool alreadyThere = _wayRemap.count(wayId) != 0;
  if (alreadyThere)
  {
    _hootdb.updateWay(wayId, w->getVersion() + 1, tags);
  }
  else
  {
    _hootdb.insertWay(wayId, tags);
  }

  if (_remapIds == true)
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

void OsmApiDbAwareHootApiDbWriter::writePartial(const shared_ptr<const Relation>& r)
{
  Tags tags = r->getTags();
  _addElementTags(r, tags);
  if (!r->getType().isEmpty())
  {
    tags["type"] = r->getType();
  }

  const long relationId = _getRemappedElementId(r->getElementId());
  const bool alreadyThere = _relationRemap.count(relationId) != 0;
  if (alreadyThere)
  {
    _hootdb.updateRelation(relationId, r->getVersion() + 1, tags);
  }
  else
  {
    _hootdb.insertRelation(relationId, tags);
  }

  for (size_t i = 0; i < r->getMembers().size(); ++i)
  {
    RelationData::Entry e = r->getMembers()[i];

    // May need to create new ID mappings for items we've not yet seen
    ElementId relationMemberElementId = e.getElementId();

    if (_remapIds == true)
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
