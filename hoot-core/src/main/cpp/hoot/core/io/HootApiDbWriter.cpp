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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "HootApiDbWriter.h"

// Standard
#include <fstream>

// hoot
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/DbUtils.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QtSql/QSqlDatabase>
#include <QUrl>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapWriter, HootApiDbWriter)

HootApiDbWriter::HootApiDbWriter()
  : _nodesWritten(0),
    _waysWritten(0),
    _relationsWritten(0),
    _remapIds(true),
    _includeIds(false),
    _jobId(-1),
    _preserveVersionOnInsert(false),
    _open(false)
{
  setConfiguration(conf());
}

HootApiDbWriter::~HootApiDbWriter()
{
  close();
}

void HootApiDbWriter::_addElementTags(const std::shared_ptr<const Element>& e, Tags& t) const
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
  LOG_TRACE("Closing database writer...");
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
  LOG_TRACE("Finalizing write operation...");
  if (_open)
  {
    _hootdb.endChangeset();
    if (_hootdb.inTransaction())
      _hootdb.commit();
    _hootdb.updateImportSequences();
    if (_hootdb.isOpen())
      _hootdb.close();
    _open = false;
  }
}

bool HootApiDbWriter::isSupported(const QString& urlStr) const
{
  QUrl url(urlStr);
  return _hootdb.isSupported(url) && !_copyBulkInsertActivated;
}

void HootApiDbWriter::open(const QString& urlStr)
{
  LOG_DEBUG("Opening database writer for: " << urlStr << "...");

  const long mapId = _openDb(urlStr);
  LOG_VARD(mapId);
  const QString mapName = _getMapNameFromUrl(urlStr);
  LOG_VARD(mapName);

  if (mapId != -1)
  {
    _hootdb.verifyCurrentUserMapUse(mapId, true);
    if (_overwriteMap) // delete map and overwrite it
    {
      _hootdb.deleteMap(mapId);
      _hootdb.setMapId(_hootdb.insertMap(mapName));
    }
    else
    {
      _hootdb.setMapId(mapId);
      LOG_DEBUG("Updating map with ID: " << _hootdb.getMapId() << "...");
    }
  }
  else
  {
    LOG_DEBUG("Map " << mapName << " was not found, must insert.");
    _hootdb.setMapId(_hootdb.insertMap(mapName));
  }

  LOG_VARD(_jobId);
  if (!_jobId.trimmed().isEmpty())
  {
    // a job id was passed in by the services; write the id of the map being written to the job
    // status table
    _hootdb.updateJobStatusResourceId(_jobId, _hootdb.getMapId());
  }

  _startNewChangeSet();
}

QString HootApiDbWriter::_getMapNameFromUrl(const QString& urlStr) const
{
  QUrl url(urlStr);
  const QStringList pList = url.path().split("/");
  return pList[2];
}

long HootApiDbWriter::_openDb(const QString& urlStr)
{
  if (!isSupported(urlStr))
    throw HootException("An unsupported URL was passed into HootApiDbWriter: " + urlStr);

  if (_userEmail.isEmpty())
    throw HootException("Please set the user's email address via the '" + ConfigOptions::getApiDbEmailKey() + "' configuration setting.");

  // URL must have name in it
  QUrl url(urlStr);
  const QString mapName = _getMapNameFromUrl(urlStr);
  LOG_VARD(mapName);

  _hootdb.open(url);
  _open = true;

  // create the user before we have a transaction so we can make sure the user gets added
  if (_createUserIfNotFound)
    _hootdb.setUserId(_hootdb.getOrCreateUser(_userEmail, _userEmail));
  else
    _hootdb.setUserId(_hootdb.getUserId(_userEmail, true));

  LOG_VARD(_hootdb.getCurrentUserId());

  // start the transaction. We'll close it when finalizePartial is called.
  _hootdb.transaction();

  return _hootdb.getMapIdByNameForCurrentUser(mapName);
}

void HootApiDbWriter::deleteMap(const QString& urlStr)
{
  LOG_DEBUG("Deleting map at " + urlStr << "...");

  const long mapId = _openDb(urlStr/*, true*/);
  LOG_VARD(mapId);
  if (mapId != -1)
  {
    _hootdb.verifyCurrentUserMapUse(mapId, true);
    _hootdb.deleteMap(mapId);
  }
  // Considered adding a throw here when the map is not found, but there are existing callers to
  // this that expect silence when a map requested for delete doesn't exist...so not making that
  // change.

  _hootdb.commit();
  _hootdb.close();
  _open = false;
}

long HootApiDbWriter::_getRemappedElementId(const ElementId& eid)
{
  LOG_TRACE("Getting remapped ID for element ID: " << eid << "...");

  if (_remapIds == false)
    return eid.getId();

  long retVal = -1;

  switch (eid.getType().getEnum())
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
        _sourceNodeIds.insert(eid.getId());
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
        _sourceWayIds.insert(eid.getId());
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
        _sourceRelationIds.insert(eid.getId());
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
      result[i] = _nodeRemap.at(nids[i]);
    else
      throw HootException(QString("Requested ID remap for node %1 but it did not exist in mapping table").arg(QString::number(nids[i])));
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
  setJobId(configOptions.getJobId());
  setPreserveVersionOnInsert(configOptions.getHootapiDbWriterPreserveVersionOnInsert());
}

void HootApiDbWriter::_startNewChangeSet()
{
  LOG_DEBUG("Starting changeset...");
  _hootdb.endChangeset();
  Tags tags;
  tags["bot"] = "yes";
  tags["created_by"] = HOOT_PACKAGE_NAME;
  _hootdb.beginChangeset(tags);
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
      _hootdb.updateNode(nodeId, n->getY(), n->getX(), n->getVersion() + 1, tags);
    else if (_preserveVersionOnInsert && n->getVersion() > 0)
      _hootdb.insertNode(nodeId, n->getY(), n->getX(), tags, n->getVersion());
    else
      _hootdb.insertNode(nodeId, n->getY(), n->getX(), tags);
  }
  else
  {
    LOG_VART(n->getId());
    if (_preserveVersionOnInsert && n->getVersion() > 0)
      _hootdb.insertNode(n->getId(), n->getY(), n->getX(), tags, n->getVersion());
    else
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
      _hootdb.updateWay(wayId, w->getVersion() + 1, tags);
    else if (_preserveVersionOnInsert && w->getVersion() > 0)
      _hootdb.insertWay(wayId, tags, w->getVersion());
    else
      _hootdb.insertWay(wayId, tags);
  }
  else
  {
    wayId = w->getId();
    if (_preserveVersionOnInsert && w->getVersion() > 0)
      _hootdb.insertWay(w->getId(), tags, w->getVersion());
    else
      _hootdb.insertWay(w->getId(), tags);
  }

  if (_remapIds == true)
    _hootdb.insertWayNodes(wayId, _remapNodes(w->getNodeIds()));
  else
    _hootdb.insertWayNodes(wayId, w->getNodeIds());

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
    tags["type"] = r->getType();

  if (_remapIds)
  {
    relationId = _getRemappedElementId(r->getElementId());
    _hootdb.insertRelation(relationId, tags);
  }
  else if (_preserveVersionOnInsert && r->getVersion() > 0)
  {
    _hootdb.insertRelation(r->getId(), tags, r->getVersion());
    relationId = r->getId();
  }
  else
  {
    _hootdb.insertRelation(r->getId(), tags);
    relationId = r->getId();
  }

  for (size_t i = 0; i < r->getMembers().size(); ++i)
  {
    RelationData::Entry e = r->getMembers()[i];

    // May need to create new ID mappings for items we've not yet seen
    ElementId relationMemberElementId = e.getElementId();

    if (_remapIds == true)
      relationMemberElementId = ElementId(relationMemberElementId.getType(), _getRemappedElementId(relationMemberElementId));

    _hootdb.insertRelationMember(relationId, relationMemberElementId.getType(), relationMemberElementId.getId(), e.getRole(), i);
  }

  LOG_TRACE("All members added to relation " << QString::number(relationId));

  _countChange();

  _relationsWritten++;
}

}
