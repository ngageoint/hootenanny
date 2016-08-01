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
#include "HootApiDbWriter.h"

#include <iostream>
#include <fstream>

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/util/NotImplementedException.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/io/ElementInputStream.h>

// Qt
#include <QtSql/QSqlDatabase>
#include <QUrl>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapWriter, HootApiDbWriter)

HootApiDbWriter::HootApiDbWriter()
{
  _open = false;
  _remapIds = true;
  _nodesWritten = 0;
  _waysWritten = 0;
  _relationsWritten = 0;

  setConfiguration(conf());
}

HootApiDbWriter::~HootApiDbWriter()
{
  close();
}

void HootApiDbWriter::_addElementTags(const shared_ptr<const Element> &e, Tags& t)
{
  if (e->getCircularError() >= 0.0)
  {
    t["error:circular"] = QString::number(e->getCircularError());
  }
  t["hoot:status"] = QString::number(e->getStatus().getEnum());
}

void HootApiDbWriter::close()
{
  finalizePartial();
  if ( (_nodesWritten > 0) || (_waysWritten > 0) || (_relationsWritten > 0) )
  {
    LOG_DEBUG("Write stats:");
    LOG_DEBUG("\t    Nodes: " << QString::number(_nodesWritten));
    LOG_DEBUG("\t     Ways: " << QString::number(_waysWritten));
    LOG_DEBUG("\tRelations: " << QString::number(_relationsWritten));
  }
}

void HootApiDbWriter::_countChange()
{
  _hootdb.incrementChangesetChangeCount();
}

void HootApiDbWriter::finalizePartial()
{
  //LOG_DEBUG("Inside finalize partial");
  if (_open)
  {
    //LOG_DEBUG("Ending changeset");
    _hootdb.endChangeset();
    //LOG_DEBUG("Calling commit");
    _hootdb.commit();
    //LOG_DEBUG("Calling close");
    _hootdb.close();
    _open = false;
  }

  //LOG_DEBUG("Leaving finalize partial");
}

bool HootApiDbWriter::isSupported(QString urlStr)
{
  QUrl url(urlStr);

  return _hootdb.isSupported(url);
}

void HootApiDbWriter::open(QString urlStr)
{
  set<long> mapIds = _openDb(urlStr);

  QUrl url(urlStr);
  QStringList pList = url.path().split("/");
  QString mapName = pList[2];
  _overwriteMaps(mapName, mapIds);

  _startNewChangeSet();
}

void HootApiDbWriter::deleteMap(QString urlStr)
{
  set<long> mapIds = _openDb(urlStr);

  for (set<long>::const_iterator it = mapIds.begin(); it != mapIds.end(); ++it)
  {
    LOG_INFO("Removing map with ID: " << *it);
    _hootdb.deleteMap(*it);
    LOG_INFO("Finished removing map with ID: " << *it);
  }

  _hootdb.commit();
  _hootdb.close();
  _open = false;
}

set<long> HootApiDbWriter::_openDb(QString& urlStr)
{
  if (!isSupported(urlStr))
  {
    throw HootException("An unsupported URL was passed into HootApiDbWriter: " + urlStr);
  }
  if (_userEmail.isEmpty())
  {
    throw HootException("Please set the user's email address via the '" + emailKey() + "' "
                        "configuration setting.");
  }

  QUrl url(urlStr);

  _hootdb.open(url);
  _open = true;

  // create the user before we have a transaction so we can make sure the user gets added.
  if (_createUserIfNotFound)
  {
    _hootdb.setUserId(_hootdb.getOrCreateUser(_userEmail, _userEmail));
  }
  else
  {
    _hootdb.setUserId(_hootdb.getUserId(_userEmail, true));
  }

  // start the transaction. We'll close it when finalizePartial is called.
  _hootdb.transaction();

  QStringList pList = url.path().split("/");
  QString mapName = pList[2];
  set<long> mapIds = _hootdb.selectMapIds(mapName);

  return mapIds;
}

void HootApiDbWriter::_overwriteMaps(const QString& mapName, const set<long>& mapIds)
{
  if (mapIds.size() > 0)
  {
    if (_overwriteMap) // delete mape and overwrite it
    {
      for (set<long>::const_iterator it = mapIds.begin(); it != mapIds.end(); ++it)
      {
        LOG_INFO("Removing map with ID: " << *it);
        _hootdb.deleteMap(*it);
        LOG_INFO("Finished removing map with ID: " << *it);
      }

      _hootdb.setMapId(_hootdb.insertMap(mapName, true));
    }
    else
    {
      LOG_INFO("There are one or more maps with this name. Consider using "
               "'hootapi.db.writer.overwrite.map'. Map IDs: " << mapIds);
    }
  }
  else if ( mapIds.size() == 0 )
  {
    LOG_DEBUG("Map " << mapName << " was not found, must insert");
    _hootdb.setMapId(_hootdb.insertMap(mapName, true));
  }
}

long HootApiDbWriter::_getRemappedElementId(const ElementId& eid)
{
  if (_remapIds == false)
  {
    return eid.getId();
  }

  long retVal = -1;

  switch(eid.getType().getEnum())
  {
  case ElementType::Node:
    if (_nodeRemap.count(eid.getId()) == 1)
    {
      retVal = _nodeRemap.at(eid.getId());
    }
    else
    {
      retVal = _hootdb.reserveElementId(ElementType::Node);
      _nodeRemap[eid.getId()] = retVal;
      if ( _outputMappingFile.length() > 0 )
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
      retVal = _hootdb.reserveElementId(ElementType::Way);
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
      retVal = _hootdb.reserveElementId(ElementType::Relation);
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

vector<long> HootApiDbWriter::_remapNodes(const vector<long>& nids)
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
      throw HootException(QString("Requested ID remap for node " +  QString::number(nids[i])
        + QString(" but it did not exist in mapping table")));
    }
  }

  return result;
}

void HootApiDbWriter::setConfiguration(const Settings &conf)
{
  ConfigOptions configOptions(conf);
  setUserEmail(configOptions.getHootapiDbWriterEmail());
  setCreateUser(configOptions.getHootapiDbWriterCreateUser());
  setOverwriteMap(configOptions.getHootapiDbWriterOverwriteMap());
}

void HootApiDbWriter::_startNewChangeSet()
{
  _hootdb.endChangeset();
  Tags tags;
  tags["bot"] = "yes";
  tags["created_by"] = "hootenanny";
  _hootdb.beginChangeset(tags);
}

void HootApiDbWriter::writePartial(const shared_ptr<const Node>& n)
{
  Tags t = n->getTags();
  _addElementTags(n, t);

  if (_remapIds)
  {
    bool alreadyThere = _nodeRemap.count(n->getId()) != 0;
    long nodeId = _getRemappedElementId(n->getElementId());
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

    _hootdb.insertNode(n->getId(), n->getY(), n->getX(), t);
  }

  _countChange();
  _nodesWritten++;
}

void HootApiDbWriter::writePartial(const shared_ptr<const Way>& w)
{
  long wayId;

  Tags tags = w->getTags();
  _addElementTags(w, tags);

  if (_remapIds)
  {
    bool alreadyThere = _wayRemap.count(w->getId()) != 0;
    wayId = _getRemappedElementId(w->getElementId());
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

void HootApiDbWriter::writePartial(const shared_ptr<const Relation>& r)
{
  long relationId;

  Tags tags = r->getTags();
  _addElementTags(r, tags);

  if (!r->getType().isEmpty())
  {
    tags["type"] = r->getType();
  }

  if (_remapIds)
  {
    relationId = _getRemappedElementId(r->getElementId());
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

  _relationsWritten++;
}

}
