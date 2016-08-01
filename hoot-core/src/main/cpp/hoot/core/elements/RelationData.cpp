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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "RelationData.h"

namespace hoot
{

RelationData::RelationData(long id, long changeset, long version, unsigned int timestamp,
                           QString user, long uid, bool visible) :
ElementData(id, Tags(), -1, changeset, version, timestamp, user, uid, visible)
{
}

RelationData::RelationData(const RelationData& rd) :
ElementData(rd.getId(), rd.getTags(), rd.getCircularError(), rd.getChangeset(), rd.getVersion(),
            rd.getTimestamp(), rd.getUser(), rd.getUid(), rd.getVisible()),
_type(rd._type),
_members(rd._members)
{

}

void RelationData::addElement(const QString& role, ElementId eid)
{
  _members.push_back(Entry(role, eid));
}

void RelationData::clear()
{
  ElementData::clear();
  _members.clear();
  _type.clear();
}

void RelationData::removeElement(const QString& role, ElementId eid)
{
  size_t destination = 0;
  for (size_t i = 0; i < _members.size(); i++)
  {
    const Entry& e = _members[i];
    if (e.role == role && e.getElementId() == eid)
    {
      // pass
    }
    else
    {
      if (i != destination)
      {
        _members[destination] = _members[i];
      }
      destination++;
    }
  }
  _members.resize(destination);
}

void RelationData::removeElement(ElementId eid)
{
  size_t destination = 0;
  for (size_t i = 0; i < _members.size(); i++)
  {
    const Entry& e = _members[i];
    if (e.getElementId() == eid)
    {
      // pass
    }
    else
    {
      if (i != destination)
      {
        _members[destination] = _members[i];
      }
      destination++;
    }
  }
  _members.resize(destination);
}

void RelationData::replaceElement(ElementId from, ElementId to)
{
  for (size_t i = 0; i < _members.size(); i++)
  {
    Entry& e = _members[i];
    if (e.getElementId() == from)
    {
      e.setElementId(to);
    }
  }
}

void RelationData::replaceElement(ElementId from, const QList<ElementId>& to)
{
  vector<Entry> newCopy;
  for (size_t i = 0; i < _members.size(); i++)
  {
    Entry& e = _members[i];
    if (e.getElementId() == from)
    {
      for (int i = 0; i < to.size(); ++i)
      {
        newCopy.push_back(Entry(e.role, to[i]));
      }
    }
    else
    {
      newCopy.push_back(e);
    }
  }
  _members = newCopy;
}

}
