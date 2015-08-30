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
  _open = false;
  _remapIds = true;
  _nodesWritten = 0;
  _waysWritten = 0;
  _relationsWritten = 0;

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
  if ( (_nodesWritten > 0) || (_waysWritten > 0) || (_relationsWritten > 0) )
  {
    LOG_DEBUG("Write stats:");
    LOG_DEBUG("\t    Nodes: " << QString::number(_nodesWritten));
    LOG_DEBUG("\t     Ways: " << QString::number(_waysWritten));
    LOG_DEBUG("\tRelations: " << QString::number(_relationsWritten));
  }
}

void ServicesDbWriter::_countChange()
{
  _sdb.incrementChangesetChangeCount();
}

void ServicesDbWriter::finalizePartial()
{
  //LOG_DEBUG("Inside finalize partial");
  if (_open)
  {
    //LOG_DEBUG("Ending changeset");
    _sdb.endChangeset();
    //LOG_DEBUG("Calling commit");
    _sdb.commit();
    //LOG_DEBUG("Calling close");
    _sdb.close();
    _open = false;
  }

  //LOG_DEBUG("Leaving finalize partial");
}

bool ServicesDbWriter::isSupported(QString urlStr)
{
  QUrl url(urlStr);

  return _sdb.isSupported(url);
}


void ServicesDbWriter::open(QString urlStr)
{
  _openDb(urlStr, _overwriteMap);

  _startNewChangeSet();

}

void ServicesDbWriter::deleteMap(QString urlStr)
{
  _openDb(urlStr, true); // "True" forces the map delete

  _sdb.commit();
  _sdb.close();
  _open = false;
}

void ServicesDbWriter::_openDb(QString& urlStr, bool deleteMapFlag)
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

  _sdb.open(url);
  _open = true;

  LOG_DEBUG("DB opened");

  // create the user before we have a transaction so we can make sure the user gets added.
  if (_createUserIfNotFound)
  {
    _sdb.setUserId(_sdb.getOrCreateUser(_userEmail, _userEmail));
  }
  else
  {
    _sdb.setUserId(_sdb.getUserId(_userEmail, true));
  }

  //LOG_DEBUG("DB user set");

  // start the transaction. We'll close it when finalizePartial is called.
  _sdb.transaction();

  if ( _sdb.getDatabaseType() == ServicesDb::DBTYPE_SERVICES)
  {\
    QStringList pList = url.path().split("/");
    QString mapName = pList[2];
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

        _sdb.setMapId(_sdb.insertMap(mapName, true));

      }
      else
      {
        LOG_INFO("There are one or more maps with this name. Consider using "
                 "'services.db.writer.overwrite.map'. Map IDs: " << mapIds);
      }
    }
    else if ( mapIds.size() == 0 )
    {
      LOG_DEBUG("Map " << mapName << " was not found, must insert");
      _sdb.setMapId(_sdb.insertMap(mapName, true));
    }
  }
}

long ServicesDbWriter::_getRemappedElementId(const ElementId& eid)
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
      retVal = _sdb.reserveElementId(ElementType::Node);
      _nodeRemap[eid.getId()] = retVal;
    }

    break;

  case ElementType::Way:
    if (_wayRemap.count(eid.getId()) == 1)
    {
      retVal = _wayRemap.at(eid.getId());
    }
    else
    {
      retVal = _sdb.reserveElementId(ElementType::Way);
      _wayRemap[eid.getId()] = retVal;
    }

    break;

  case ElementType::Relation:
    if (_relationRemap.count(eid.getId()) == 1)
    {
      retVal = _relationRemap.at(eid.getId());
      /*
      LOG_DEBUG("Returning established relation ID mapping, source ID = " <<
        QString::number(eid.getId()) << ", database ID = " <<
        QString::number(_relationRemap.at(eid.getId())) );
      */
    }
    else
    {
      retVal = _sdb.reserveElementId(ElementType::Relation);
      _relationRemap[eid.getId()] = retVal;

      LOG_DEBUG("Established new relation ID mapping, source ID = " <<
        QString::number(eid.getId()) << ", database ID = " <<
        QString::number(_relationRemap.at(eid.getId())) );
    }

    break;

  default:
    LOG_ERROR("Tried to create or remap ID for invalid type");
    throw NotImplementedException();

    break;
  }

  return retVal;
}

vector<long> ServicesDbWriter::_remapNodes(const vector<long>& nids)
{
  vector<long> result(nids.size()); // Creates the vector and fills nids.size number of zeroes

  Tags empty;

  for (size_t i = 0; i < nids.size(); i++)
  {
    // This is only called when adding nodes for a way.  If a way has a node we
    //    did not successfully create a mapping for when importing nodes,
    //    we can't continue
    if ( _nodeRemap.count(nids[i]) == 1 )
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

void ServicesDbWriter::setConfiguration(const Settings &conf)
{
  setUserEmail(conf.getString(emailKey(), ""));
  setCreateUser(ConfigOptions(conf).getServicesDbWriterCreateUser());
  setOverwriteMap(conf.getBool(overwriteMapKey(), false));
}

void ServicesDbWriter::_startNewChangeSet()
{
  _sdb.endChangeset();
  Tags tags;
  tags["bot"] = "yes";
  tags["created_by"] = "hootenanny";
  _sdb.beginChangeset(tags);
}

void ServicesDbWriter::writePartial(const shared_ptr<const Node>& n)
{
  bool countChange = true;

  //LOG_DEBUG("Inside writePartial for Node");

  Tags t = n->getTags();
  // Only add tags for servicesDB, not good for OSM API
  if ( _sdb.getDatabaseType() == ServicesDb::DBTYPE_SERVICES)
  {
    _addElementTags(n, t);
  }

  //LOG_DEBUG("Incoming node ID: " << n->getId());


  if (_remapIds)
  {
    bool alreadyThere = _nodeRemap.count(n->getId()) != 0;
    long nodeId = _getRemappedElementId(n->getElementId());
    if (alreadyThere)
    {
      _sdb.updateWay(nodeId, t);
    }
    else
    {
      _sdb.insertNode(nodeId, n->getY(), n->getX(), t);
    }
  }
  else
  {
    if ( n->getId() < 1 )
    {
      throw HootException("Writing non-positive IDs without remap is not supported by "
                          "ServicesDbWriter.");
    }

    //LOG_DEBUG("Inserted node " << QString::number(n->getId()) << ", no remapping" );
    _sdb.insertNode(n->getId(), n->getY(), n->getX(), t);
  }

  if (countChange)
  {
    _countChange();
    _nodesWritten++;
  }
}

void ServicesDbWriter::writePartial(const shared_ptr<const Way>& w)
{
  long wayId;

  //LOG_DEBUG("Inside writePartial for Way " << QString::number(w->getId()));

  Tags tags = w->getTags();

  if ( _sdb.getDatabaseType() == ServicesDb::DBTYPE_SERVICES)
  {
    _addElementTags(w, tags);
  }

  if (_remapIds)
  {
    bool alreadyThere = _wayRemap.count(w->getId()) != 0;
    wayId = _getRemappedElementId(w->getElementId());
    if (alreadyThere)
    {
      _sdb.updateWay(wayId, tags);
    }
    else
    {
      _sdb.insertWay(wayId, tags);
    }
  }
  else if (w->getId() < 1)
  {
    throw HootException("Non-positive IDs are not supported by ServicesDbWriter.");
  }
  else
  {
    wayId = w->getId();
    _sdb.insertWay(w->getId(), tags);
  }

  if ( _remapIds == true )
  {
    _sdb.insertWayNodes(wayId, _remapNodes(w->getNodeIds()));
  }
  else
  {
    _sdb.insertWayNodes(wayId, w->getNodeIds());
  }

  _countChange();

  _waysWritten++;
}

void ServicesDbWriter::writePartial(const shared_ptr<const Relation>& r)
{
  long relationId;

  //LOG_DEBUG("Inside writePartial for Relation");

  Tags tags = r->getTags();
  if ( _sdb.getDatabaseType() == ServicesDb::DBTYPE_SERVICES)
  {
    _addElementTags(r, tags);
  }

  if (!r->getType().isEmpty())
  {
    tags["type"] = r->getType();
  }

  if (_remapIds)
  {
    bool alreadyThere = _relationRemap.count(r->getId()) != 0;
    relationId = _getRemappedElementId(r->getElementId());

    LOG_DEBUG("Inserting relation with source ID = " <<
              QString::number(r->getId()) << " which maps to DB ID = " <<
              QString::number(relationId) );

      _sdb.insertRelation(relationId, tags);
  }
  else
  {
    if (r->getId() < 1)
    {
      throw HootException("Non-positive IDs are not supported by ServicesDbWriter without remapping");
    }

    _sdb.insertRelation(r->getId(), tags);
    relationId = r->getId();
  }

  Tags emptyTags;
  for (size_t i = 0; i < r->getMembers().size(); ++i)
  {
    RelationData::Entry e = r->getMembers()[i];

    // All nodes and ways are already inserted in the database. May need to
    //    find mappings for those.  May need to create new ID mappings
    //    for relations if we've not yet seen them
    ElementId relationMemberElementId = e.getElementId();

    if ( _remapIds == true )
    {
      relationMemberElementId = ElementId(relationMemberElementId.getType(),
        _getRemappedElementId(relationMemberElementId));
    }

    _sdb.insertRelationMember(relationId, relationMemberElementId.getType(),
                              relationMemberElementId.getId(), e.role, i);
  }

  //LOG_DEBUG("All members added to relation " << QString::number(relationId));

  _countChange();

  //LOG_DEBUG("Leaving relation write cleanly");

  _relationsWritten++;
}

}
