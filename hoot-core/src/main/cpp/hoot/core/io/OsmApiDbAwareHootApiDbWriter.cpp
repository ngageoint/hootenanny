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
  LOG_VARD(ConfigOptions().getHootapiDbWriterOsmapidbUrl());
  if (!_osmApiDb.isSupported(ConfigOptions().getHootapiDbWriterOsmapidbUrl()))
  {
    throw HootException(
      "The OsmApiDbAwareHootApiDbWriter requires a valid OSM API DB URL for element id sequencing.");
  }

  LOG_VARD(urlStr);
  HootApiDbWriter::open(urlStr);
  _osmApiDb.open(ConfigOptions().getHootapiDbWriterOsmapidbUrl());
}

long OsmApiDbAwareHootApiDbWriter::_getRemappedElementId(const ElementId& eid)
{
  LOG_DEBUG("OsmApiDbAwareHootApiDbWriter::_getRemappedElementId");
  long retVal = -1;

  switch(eid.getType().getEnum())
  {
    case ElementType::Node:

      /*if (_nodeRemap.count(eid.getId()) == 1)
      {
        retVal = _nodeRemap.at(eid.getId());
      }
      else
      {*/
        retVal = _osmApiDb.getNextId(ElementType::Node);
        _nodeRemap[eid.getId()] = retVal;
        if (_outputMappingFile.length() > 0)
        {
          _sourceNodeIds.insert(eid.getId());
        }
      //}

      break;

    case ElementType::Way:

      /*if (_wayRemap.count(eid.getId()) == 1)
      {
        retVal = _wayRemap.at(eid.getId());
      }
      else
      {*/
        retVal = _osmApiDb.getNextId(ElementType::Way);
        _wayRemap[eid.getId()] = retVal;
        if ( _outputMappingFile.length() > 0 )
        {
          _sourceWayIds.insert(eid.getId());
        }
      //}

      break;

    case ElementType::Relation:

      /*if (_relationRemap.count(eid.getId()) == 1)
      {
        retVal = _relationRemap.at(eid.getId());
      }
      else
      {*/
        retVal = _osmApiDb.getNextId(ElementType::Relation);
        _relationRemap[eid.getId()] = retVal;
        if ( _outputMappingFile.length() > 0 )
        {
          _sourceRelationIds.insert(eid.getId());
        }
      //}

      break;

  default:
    LOG_ERROR("Tried to create or remap ID for invalid type");
    throw NotImplementedException();

    break;
  }

  return retVal;
}

//TODO: get rid of this method
vector<long> OsmApiDbAwareHootApiDbWriter::_remapNodes(const vector<long>& nids)
{
  vector<long> result(nids.size()); // Creates the vector and fills nids.size number of zeroes

  for (size_t i = 0; i < nids.size(); i++)
  {
    // This is only called when adding nodes for a way.  If a way has a node we
    //    did not successfully create a mapping for when importing nodes,
    //    we can't continue
    if (_nodeRemap.count(nids[i]) == 1)
    {
      result[i] = _nodeRemap.at(nids[i]);
    }
    else
    {
      if (ConfigOptions().getHootapiDbWriterOsmapidbUrl().isEmpty())
      {
        throw HootException(QString("Requested ID remap for node " +  QString::number(nids[i])
          + QString(" but it did not exist in mapping table")));
      }
      else
      {
        result[i] = nids[i];
      }
    }
  }

  return result;
}

void OsmApiDbAwareHootApiDbWriter::writePartial(const shared_ptr<const Node>& n)
{
  bool countChange = true;

  LOG_VARD(n->getId());

  Tags t = n->getTags();
  _addElementTags(n, t);

  if (n->getId() < 1)
  {
    bool alreadyThere = _nodeRemap.count(n->getId()) != 0;
    LOG_VARD(alreadyThere);
    long nodeId = _getRemappedElementId(n->getElementId());
    LOG_VARD(nodeId);
    if (alreadyThere)
    {
      _hootdb.updateNode(nodeId, n->getY(), n->getX(), n->getVersion() + 1, t);
    }
    else
    {
      _hootdb.insertNode(nodeId, n->getY(), n->getX(), t);
    }
  }
  else
  {
    if (n->getId() < 1)
    {
      throw HootException("Writing non-positive IDs without remap is not supported by "
                          "HootApiDbWriter.");
    }

    //LOG_DEBUG("Inserted node " << QString::number(n->getId()) << ", no remapping" );
    _hootdb.insertNode(n->getId(), n->getY(), n->getX(), t);
  }

  if (countChange)
  {
    _countChange();
    _nodesWritten++;
  }
}

void OsmApiDbAwareHootApiDbWriter::writePartial(const shared_ptr<const Way>& w)
{
  long wayId;

  Tags tags = w->getTags();
  _addElementTags(w, tags);

  if (w->getId() < 1)
  {
    bool alreadyThere = _wayRemap.count(w->getId()) != 0;
    LOG_VARD(alreadyThere);
    wayId = _getRemappedElementId(w->getElementId());
    LOG_VARD(wayId);
    if (alreadyThere)
    {
      _hootdb.updateWay(wayId, w->getVersion() + 1, tags);
    }
    else
    {
      _hootdb.insertWay(wayId, tags);
    }
  }
  else if (w->getId() < 1)
  {
    throw HootException("Non-positive IDs are not supported by HootApiDbWriter.");
  }
  else
  {
    wayId = w->getId();
    _hootdb.insertWay(w->getId(), tags);
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
  long relationId;

  //LOG_DEBUG("Inside writePartial for Relation");

  Tags tags = r->getTags();
  _addElementTags(r, tags);

  if (!r->getType().isEmpty())
  {
    tags["type"] = r->getType();
  }

  if (r->getId() < 1)
  {
    relationId = _getRemappedElementId(r->getElementId());
    LOG_VARD(relationId);

    LOG_DEBUG("Inserting relation with source ID = " <<
              QString::number(r->getId()) << " which maps to DB ID = " <<
              QString::number(relationId) );

    _hootdb.insertRelation(relationId, tags);
  }
  else
  {
    if (r->getId() < 1)
    {
      throw HootException("Non-positive IDs are not supported by HootApiDbWriter without remapping");
    }

    _hootdb.insertRelation(r->getId(), tags);
    relationId = r->getId();
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

  //LOG_DEBUG("All members added to relation " << QString::number(relationId));

  _countChange();

  //LOG_DEBUG("Leaving relation write cleanly");

  _relationsWritten++;
}

}
