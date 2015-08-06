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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ServicesDbWriter.h"

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

HOOT_FACTORY_REGISTER(OsmMapWriter, ServicesDbWriter)

ServicesDbWriter::ServicesDbWriter()
{
  _mapId = -1;
  _numChangeSetChanges = 0;
  _open = false;
  _remapIds = true;
  setConfiguration(conf());
}

ServicesDbWriter::~ServicesDbWriter()
{
  close();
}

void ServicesDbWriter::_addElementTags(const shared_ptr<const Element> &e, Tags& t)
{
  if (e->getCircularError() >= 0.0)
  {
    t["error:circular"] = QString::number(e->getCircularError());
  }
  t["hoot:status"] = QString::number(e->getStatus().getEnum());
}

void ServicesDbWriter::close()
{
  finalizePartial();
}

void ServicesDbWriter::_countChange()
{
  _numChangeSetChanges++;

  if (_numChangeSetChanges >= ServicesDb::maximumChangeSetEdits())
  {
    _startNewChangeSet();
  }
}

void ServicesDbWriter::finalizePartial()
{
  if (_open)
  {
    if (_numChangeSetChanges > 0)
    {
      _sdb.closeChangeSet(_mapId, _changeSetId, _env, _numChangeSetChanges);
    }
    _sdb.commit();
    _sdb.close();
    _open = false;
  }
}

bool ServicesDbWriter::isSupported(QString urlStr)
{
  QUrl url(urlStr);

  return _sdb.isSupported(url);
}


void ServicesDbWriter::open(QString urlStr)
{
  QString mapName = _openDb(urlStr, _overwriteMap);

  _mapId = _sdb.insertMap(mapName);

  _startNewChangeSet();
}

void ServicesDbWriter::deleteMap(QString urlStr)
{
  QString mapName = _openDb(urlStr, true); // "True" forces the map delete

  _sdb.commit();
  _sdb.close();
  _open = false;
}

QString ServicesDbWriter::_openDb(QString& urlStr, bool deleteMapFlag)
{
  if (!isSupported(urlStr))
  {
    throw HootException("An unsupported URL was passed in.");
  }
  if (_userEmail.isEmpty())
  {
    throw HootException("Please set the user's email address via the '" + emailKey() + "' "
                        "configuration setting.");
  }

  QUrl url(urlStr);

  QStringList pList = url.path().split("/");
  QString mapName = pList[2];

  _numChangeSetChanges = 0;
  _env.init();
  _sdb.open(url);
  _open = true;

  // create the user before we have a transaction so we can make sure the user gets added.
  if (_createUserIfNotFound)
  {
    _sdb.setUserId(_sdb.getOrCreateUser(_userEmail, _userEmail));
  }
  else
  {
    _sdb.setUserId(_sdb.getUserId(_userEmail, true));
  }

  // start the transaction. We'll close it when finalizePartial is called.
  _sdb.transaction();

  set<long> mapIds = _sdb.selectMapIds(mapName);

  if (mapIds.size() > 0)
  {
    if (deleteMapFlag) // deleteMapFlag is either True or _overwriteMap
    {
      for (set<long>::const_iterator it = mapIds.begin(); it != mapIds.end(); ++it)
      {
        LOG_INFO("Removing map with ID: " << *it);
        _sdb.deleteMap(*it);
        LOG_INFO("Finished removing map with ID: " << *it);
      }
    }
    else
    {
      LOG_INFO("There are one or more maps with this name. Consider using "
               "'services.db.writer.overwrite.map'. Map IDs: " << mapIds);
    }
  }

  return mapName;
}

ElementId ServicesDbWriter::_remapOrCreateElementId(ElementId eid, const Tags& tags)
{
  if (_remapIds == false)
  {
    return eid;
  }

  switch(eid.getType().getEnum())
  {
  case ElementType::Node:
    {
      if (_nodeRemap.count(eid.getId()) == 1)
      {
        return ElementId::node(_nodeRemap.at(eid.getId()));
      }
      long newId = _sdb.insertNode(_mapId, eid.getId(), 0, 0, _changeSetId, tags, true);
      _countChange();
      _nodeRemap[eid.getId()] = newId;
      return ElementId::node(newId);
    }
  case ElementType::Way:
    {
      if (_wayRemap.count(eid.getId()) == 1)
      {
        return ElementId::way(_wayRemap.at(eid.getId()));
      }
      long newId = _sdb.insertWay(_mapId, eid.getId(), _changeSetId, tags, true);
      _countChange();
      _wayRemap[eid.getId()] = newId;
      return ElementId::way(newId);
    }
  case ElementType::Relation:
    {
      if (_relationRemap.count(eid.getId()) == 1)
      {
        return ElementId::relation(_relationRemap.at(eid.getId()));
      }
      long newId = _sdb.insertRelation(_mapId, eid.getId(), _changeSetId, tags, true);
      _countChange();
      _relationRemap[eid.getId()] = newId;
      return ElementId::relation(newId);
    }
  default:
    throw NotImplementedException();
  }
}

vector<long> ServicesDbWriter::_remapNodes(const vector<long>& nids)
{
  vector<long> result(nids.size());

  Tags empty;
  for (size_t i = 0; i < nids.size(); i++)
  {
    if (_remapIds)
    {
      result[i] = _remapOrCreateElementId(ElementId::node(nids[i]), empty).getId();
    }
    else
    {
      result[i] = nids[i];
    }
  }

  return result;
}

void ServicesDbWriter::setConfiguration(const Settings &conf)
{
  setUserEmail(conf.getString(emailKey(), ""));
  setCreateUser(ConfigOptions(conf).getServicesDbWriterCreateUser());
  setOverwriteMap(conf.getBool(overwriteMapKey(), false));
}

void ServicesDbWriter::_startNewChangeSet()
{
  if (_numChangeSetChanges > 0)
  {
    _sdb.closeChangeSet(_mapId, _changeSetId, _env, _numChangeSetChanges);
  }
  _env.init();
  _numChangeSetChanges = 0;
  Tags tags;
  tags["bot"] = "yes";
  tags["created_by"] = "hootenanny";
  _changeSetId = _sdb.insertChangeSet(_mapId, tags);
}

void ServicesDbWriter::writePartial(const shared_ptr<const Node>& n)
{
  long newId;
  bool countChange = true;

  Tags t = n->getTags();
  _addElementTags(n, t);

  if (n->getId() < 1 && _remapIds == false)
  {
    throw HootException("Writing non-positive IDs without remap is not supported by "
                        "ServicesDbWriter.");
  }
  else if (_remapIds && _nodeRemap.count(n->getId()) != 0)
  {
    newId = _nodeRemap.at(n->getId());
    _sdb.updateNode(_mapId, n->getId(), n->getY(), n->getX(), _changeSetId, t);
    countChange = false;
  }
  else
  {
    newId = _sdb.insertNode(_mapId, n->getId(), n->getY(), n->getX(), _changeSetId, t,
                            _remapIds);
    if (_remapIds)
    {
      _nodeRemap[n->getId()] = newId;
    }
  }

  _env.expandToInclude(n->getX(), n->getY());

  if (countChange)
  {
    _countChange();
  }
}

void ServicesDbWriter::writePartial(const shared_ptr<const Way>& w)
{
  long wayId;

  Tags tags = w->getTags();
  _addElementTags(w, tags);

  if (_remapIds)
  {
    bool alreadyThere = _wayRemap.count(w->getId()) != 0;
    wayId = _remapOrCreateElementId(w->getElementId(), tags).getId();
    if (alreadyThere)
    {
      _sdb.updateWay(_mapId, wayId, _changeSetId, tags);
    }
  }
  else if (w->getId() < 1)
  {
    throw HootException("Non-positive IDs are not supported by ServicesDbWriter.");
  }
  else
  {
    wayId = _sdb.insertWay(_mapId, w->getId(), _changeSetId, tags, false);
  }

  _sdb.insertWayNodes(_mapId, wayId, _remapNodes(w->getNodeIds()));

  _countChange();
}

void ServicesDbWriter::writePartial(const shared_ptr<const Relation>& r)
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
    bool alreadyThere = _wayRemap.count(r->getId()) != 0;
    relationId = _remapOrCreateElementId(r->getElementId(), tags).getId();
    if (alreadyThere)
    {
      _sdb.updateRelation(_mapId, relationId, _changeSetId, tags);
    }
  }
  else if (r->getId() < 1)
  {
    throw HootException("Non-positive IDs are not supported by ServicesDbWriter.");
  }
  else
  {
    relationId = _sdb.insertRelation(_mapId, r->getId(), _changeSetId, tags, false);
  }

  Tags empty;
  for (size_t i = 0; i < r->getMembers().size(); ++i)
  {
    RelationData::Entry e = r->getMembers()[i];
    ElementId eid = _remapOrCreateElementId(e.getElementId(), empty);
    _sdb.insertRelationMembers(_mapId, relationId, eid.getType(), eid.getId(), e.role, i);
  }

  _countChange();
}

}
