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
#include "HootApiDbWriter.h"

#include <iostream>
#include <fstream>

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MetadataTags.h>
#include <hoot/core/util/NotImplementedException.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/DbUtils.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QtSql/QSqlDatabase>
#include <QUrl>

using namespace std;

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
  _includeIds = false;

  setConfiguration(conf());
}

HootApiDbWriter::~HootApiDbWriter()
{
  close();
}

void HootApiDbWriter::_addElementTags(const boost::shared_ptr<const Element> &e, Tags& t)
{
  LOG_TRACE("Adding element tags to: " << e->getElementId());
  if (!t.contains(MetadataTags::HootStatus()))
  {
    if ((_textStatus && t.getNonDebugCount() > 0) || (_includeDebug && _textStatus))
      t[MetadataTags::HootStatus()] = e->getStatus().toTextStatus();
    else if (_includeDebug && !_textStatus)
      t[MetadataTags::HootStatus()] = QString::number(e->getStatus().getEnum());
  }
  if (e->hasCircularError() && _includeCircularError)
    t[MetadataTags::ErrorCircular()] = QString::number(e->getCircularError());
  if (_includeDebug || _includeIds)
    t[MetadataTags::HootId()] = QString("%1").arg(e->getId());
}

void HootApiDbWriter::close()
{
  LOG_DEBUG("Closing database writer...");
  finalizePartial();
  if ((_nodesWritten > 0) || (_waysWritten > 0) || (_relationsWritten > 0))
  {
    LOG_DEBUG("Write stats:");
    LOG_DEBUG("\t    Nodes: " << StringUtils::formatLargeNumber(_nodesWritten));
    LOG_DEBUG("\t     Ways: " << StringUtils::formatLargeNumber(_waysWritten));
    LOG_DEBUG("\tRelations: " << StringUtils::formatLargeNumber(_relationsWritten));
  }
}

void HootApiDbWriter::_countChange()
{
  _hootdb.incrementChangesetChangeCount();
}

void HootApiDbWriter::finalizePartial()
{
  LOG_DEBUG("Finalizing write operation...");
  if (_open)
  {
    _hootdb.endChangeset();
    if (_hootdb.inTransaction())
      _hootdb.commit();
    if (_hootdb.isOpen())
      _hootdb.close();
    _open = false;
  }
}

bool HootApiDbWriter::isSupported(QString urlStr)
{
  QUrl url(urlStr);
  return _hootdb.isSupported(url) && !_copyBulkInsertActivated;
}

void HootApiDbWriter::open(QString urlStr)
{
  LOG_DEBUG("Opening database writer for: " << urlStr << "...");

  set<long> mapIds = _openDb(urlStr);

  QUrl url(urlStr);
  QStringList pList = url.path().split("/");
  QString mapName = pList[2];
  _overwriteMaps(mapName, mapIds);

  _startNewChangeSet();
}

void HootApiDbWriter::deleteMap(QString urlStr)
{
  LOG_TRACE("Deleting map at " + urlStr << "...");

  set<long> mapIds = _openDb(urlStr);

  for (set<long>::const_iterator it = mapIds.begin(); it != mapIds.end(); ++it)
  {
    LOG_INFO("Removing map with ID: " << *it);
    _hootdb.deleteMap(*it);
    LOG_DEBUG("Finished removing map with ID: " << *it);
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
    throw HootException("Please set the user's email address via the '" +
                        ConfigOptions::getApiDbEmailKey() + "' configuration setting.");
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
    if (_overwriteMap) // delete map and overwrite it
    {
      for (set<long>::const_iterator it = mapIds.begin(); it != mapIds.end(); ++it)
      {
        LOG_DEBUG("Removing map with ID: " << *it << "...");
        _hootdb.deleteMap(*it);
        LOG_DEBUG("Finished removing map with ID: " << *it);
      }

      _hootdb.setMapId(_hootdb.insertMap(mapName, true));
    }
    else if (mapIds.size() > 1)
    {
      LOG_ERROR("There are multiple maps with this name. Consider using "
                "'hootapi.db.writer.overwrite.map'. Map IDs: " << mapIds);
    }
    else
    {
      set<long>::const_iterator idItr = mapIds.begin();
      _hootdb.setMapId(*idItr);
      LOG_DEBUG("Updating map with ID: " << _hootdb.getMapId() << "...");
    }
  }
  else if (mapIds.size() == 0)
  {
    LOG_DEBUG("Map " << mapName << " was not found, must insert.");
    _hootdb.setMapId(_hootdb.insertMap(mapName, true));
  }
}

long HootApiDbWriter::_getRemappedElementId(const ElementId& eid)
{
  LOG_TRACE("Getting remapped ID for element ID: " << eid << "...");

  LOG_VART(_remapIds);
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
      LOG_VART(retVal);
    }
    else
    {
      retVal = _hootdb.reserveElementId(ElementType::Node);
      _nodeRemap[eid.getId()] = retVal;
      if ( _outputMappingFile.length() > 0 )
      {
        _sourceNodeIds.insert(eid.getId());
      }
      LOG_VART(retVal);
    }

    break;

  case ElementType::Way:
    if (_wayRemap.count(eid.getId()) == 1)
    {
      retVal = _wayRemap.at(eid.getId());
      LOG_VART(retVal);
    }
    else
    {
      retVal = _hootdb.reserveElementId(ElementType::Way);
      _wayRemap[eid.getId()] = retVal;
      if ( _outputMappingFile.length() > 0 )
      {
        _sourceWayIds.insert(eid.getId());
      }
      LOG_VART(retVal);
    }

    break;

  case ElementType::Relation:
    if (_relationRemap.count(eid.getId()) == 1)
    {
      retVal = _relationRemap.at(eid.getId());
      LOG_VART(retVal);
    }
    else
    {
      retVal = _hootdb.reserveElementId(ElementType::Relation);
      _relationRemap[eid.getId()] = retVal;
      if ( _outputMappingFile.length() > 0 )
      {
        _sourceRelationIds.insert(eid.getId());
      }
      LOG_VART(retVal);
    }

    break;

  default:
    LOG_ERROR("Tried to create or remap ID for invalid type");
    throw NotImplementedException();

    break;
  }

  LOG_TRACE("Remapped ID for element type " << eid.getType().toString() << " from " <<
            eid.getId() << " to " << retVal);

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
  setUserEmail(configOptions.getApiDbEmail());
  setCreateUser(configOptions.getHootapiDbWriterCreateUser());
  setOverwriteMap(configOptions.getHootapiDbWriterOverwriteMap());
  setIncludeDebug(configOptions.getWriterIncludeDebugTags());
  setTextStatus(configOptions.getWriterTextStatus());
  setIncludeCircularError(configOptions.getWriterIncludeCircularErrorTags());
  setRemap(configOptions.getHootapiDbWriterRemapIds());
  setCopyBulkInsertActivated(configOptions.getHootapiDbWriterCopyBulkInsert());
}

void HootApiDbWriter::_startNewChangeSet()
{
  LOG_DEBUG("Starting changeset...");
  _hootdb.endChangeset();
  Tags tags;
  tags["bot"] = "yes";
  tags["created_by"] = "hootenanny";
  _hootdb.beginChangeset(tags);
}

void HootApiDbWriter::writeChange(const Change& change)
{
  switch (change.getType())
  {
    case Change::Create:
      _createElement(change.getElement());
      break;
    case Change::Modify:
      _modifyElement(change.getElement());
      break;
    case Change::Delete:
      _deleteElement(change.getElement());
      break;
    default:
      throw IllegalArgumentException("Unexpected change type.");
  }
}

void HootApiDbWriter::_createElement(ConstElementPtr element)
{
  switch (element->getElementType().getEnum())
  {
    case ElementType::Node:
      _hootdb.insertNode(boost::dynamic_pointer_cast<const Node>(element));
      break;
    //only supporting nodes for now
    default:
      throw HootException("Unsupported element type");
  }
}

void HootApiDbWriter::_modifyElement(ConstElementPtr element)
{
  switch (element->getElementType().getEnum())
  {
    case ElementType::Node:
      _hootdb.updateNode(boost::dynamic_pointer_cast<const Node>(element));
      break;
    //only supporting nodes for now
    default:
      throw HootException("Unsupported element type");
  }
}

void HootApiDbWriter::_deleteElement(ConstElementPtr element)
{
  switch (element->getElementType().getEnum())
  {
    case ElementType::Node:
      _hootdb.deleteNode(boost::dynamic_pointer_cast<const Node>(element));
      break;
    //only supporting nodes for now
    default:
      throw HootException("Unsupported element type");
  }
}

void HootApiDbWriter::writePartial(const ConstNodePtr& n)
{
  LOG_TRACE("Writing node: " << n->getElementId());

  Tags tags = n->getTags();
  _addElementTags(n, tags);

  if (_remapIds)
  {
    bool alreadyThere = _nodeRemap.count(n->getId()) != 0;
    long nodeId = _getRemappedElementId(n->getElementId());
    LOG_VART(nodeId);

    if (alreadyThere)
    {
      _hootdb.updateNode(nodeId, n->getY(), n->getX(), n->getVersion() + 1, tags);
    }
    else
    {
      _hootdb.insertNode(nodeId, n->getY(), n->getX(), tags);
    }
  }
  else
  {
    if (n->getId() < 1)
    {
      throw HootException("Writing non-positive IDs without remap is not supported by "
                          "HootApiDbWriter.");
    }

    LOG_VART(n->getId());
    _hootdb.insertNode(n->getId(), n->getY(), n->getX(), tags);
  }

  LOG_VART(n->getVersion());

  _countChange();
  _nodesWritten++;
}

void HootApiDbWriter::writePartial(const ConstWayPtr& w)
{
  LOG_TRACE("Writing way: " << w->getElementId());

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

void HootApiDbWriter::writePartial(const ConstRelationPtr& r)
{
  LOG_TRACE("Writing relation: " << r->getElementId());

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

  LOG_TRACE("All members added to relation " << QString::number(relationId));

  _countChange();

  _relationsWritten++;
}

}
