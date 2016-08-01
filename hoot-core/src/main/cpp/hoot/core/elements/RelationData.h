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
#ifndef RELATIONDATA_H
#define RELATIONDATA_H

#include "ElementData.h"
#include "ElementId.h"
#include "ElementType.h"

// Standard
#include <ostream>
#include <vector>

namespace hoot
{
using namespace std;

class RelationData : public ElementData
{
public:
  struct Entry
  {
    QString role;

    Entry() {}

    Entry(QString role, ElementId eid)
    {
      this->role = role;
      _eid = eid;
    }

    Entry(ElementId eid) :
      _eid(eid)
    {
    }

    ElementId getElementId() const { return _eid; }
    void setElementId(ElementId eid) { _eid = eid; }
    const QString& getRole() const { return role; }

    QString toString() const
    {
      return QString("Entry: role: %1, eid: %2").arg(role).arg(_eid.toString());
    }

  private:
    ElementId _eid;
  };

  RelationData(long id, long changeset = ElementData::CHANGESET_EMPTY,
               long version = ElementData::VERSION_EMPTY,
               unsigned int timestamp = ElementData::TIMESTAMP_EMPTY,
               QString user = ElementData::USER_EMPTY, long uid = ElementData::UID_EMPTY,
               bool visible = ElementData::VISIBLE_EMPTY);

  RelationData(const RelationData& rd);

  virtual ~RelationData() {}

  void addElement(const QString& role, ElementId eid);

  virtual void clear();

  const vector<Entry>& getElements() const { return _members; }

  const QString& getType() const { return _type; }

  /**
   * @optimize This is very slow and may copy the whole entries list. This should be fine for
   * most relations as they're pretty small, but there may be instances where we have big relations
   * and this causes a slowdown.
   */
  void removeElement(const QString& role, ElementId eid);
  void removeElement(ElementId eid);

  void replaceElement(ElementId from, ElementId to);
  void replaceElement(ElementId from, const QList<ElementId> &to);

  /**
   * See Relation::replaceElements for a description.
   * @optimize Replacing _members with a linked list would speed things up.
   */
  template<typename IT>
  void replaceElements(Entry old, IT start, IT end);

  void setMembers(const vector<Entry>& members) { _members = members; }

  void setType(const QString& type) { _type = type; }

private:

  QString _type;

  vector<Entry> _members;
};

inline std::ostream& operator<<(std::ostream& o, const RelationData::Entry& e)
{
  o << e.toString().toStdString();
  return o;
}

template<typename IT>
void RelationData::replaceElements(Entry old, IT start, IT end)
{
  // make a copy of the old members list to work from.
  vector<Entry> oldMembers = _members;
  _members.resize(0);
  _members.reserve(oldMembers.size() + (end - start));

  // got through all the members.
  for (size_t i = 0; i < oldMembers.size(); ++i)
  {
    // if the roles match, or the old role is empty and the other attributes match.
    if ((old.role.isEmpty() || old.role == oldMembers[i].role) &&
        old.getElementId() == oldMembers[i].getElementId())
    {
      // go through all the new members
      for (IT it = start; it != end; ++it)
      {
        // if the old role is empty, then assign the role that the old member has
        if (old.role.isEmpty())
        {
          Entry e = *it;
          e.role = oldMembers[i].role;
          _members.push_back(e);
        }
        // if the old role is specified then just add the child as is.
        else
        {
          _members.push_back(*it);
        }
      }
    }
    else
    {
      _members.push_back(oldMembers[i]);
    }
  }
}


}

#endif // RELATIONDATA_H
