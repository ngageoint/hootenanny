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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ElementToRelationMap.h"

// hoot
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

ElementToRelationMap::ElementToRelationMap()
{
}

void ElementToRelationMap::addRelation(const OsmMap& map, const boost::shared_ptr<const Relation>& r)
{
  class AddMemberVisitor : public ElementVisitor
  {
  public:

    HashMap< ElementId, set<long> >& _mapping;
    long _rid;

    AddMemberVisitor(HashMap< ElementId, set<long> >& mapping, long rid) : _mapping(mapping)
    {
      _rid = rid;
    }

    virtual void visit(const ConstElementPtr& e)
    {
      ElementId eid = e->getElementId();
      // no need to map it to itself.
      if (eid != ElementId::relation(_rid))
      {
        _mapping[ElementId(e->getElementType(), e->getId())].insert(_rid);
      }
    }
  };

  AddMemberVisitor filter(_mapping, r->getId());
  r->visitRo(map, filter);
}

const set<long>& ElementToRelationMap::getRelationByElement(ElementId eid) const
{
  HashMap< ElementId, set<long> >::const_iterator it = _mapping.find(eid);
  if (it == _mapping.end())
  {
    return _emptySet;
  }
  else
  {
    return it->second;
  }
}

const set<long>& ElementToRelationMap::getRelationByElement(const boost::shared_ptr<const Element>& e)
  const
{
  return getRelationByElement(e->getElementId());
}

const set<long>& ElementToRelationMap::getRelationByElement(const Element* e) const
{
  return getRelationByElement(e->getElementId());
}

void ElementToRelationMap::removeRelation(const OsmMap& map, const boost::shared_ptr<const Relation>& r)
{
  class RemoveMemberVisitor : public ElementVisitor
  {
  public:

    HashMap< ElementId, set<long> >& _mapping;
    long _rid;

    RemoveMemberVisitor(HashMap< ElementId, set<long> >& mapping, long rid) : _mapping(mapping)
    {
      _rid = rid;
    }

    virtual void visit(const ConstElementPtr& e)
    {
      ElementId ep(e->getElementType(), e->getId());
      set<long>& relations = _mapping[ep];
      relations.erase(_rid);
      if (relations.size() == 0)
      {
        _mapping.erase(ep);
      }
    }
  };

  RemoveMemberVisitor filter(_mapping, r->getId());
  r->visitRo(map, filter);
}

bool ElementToRelationMap::validate(const OsmMap& map) const
{
  class ContainsElementVisitor : public ElementVisitor
  {
  public:
    ContainsElementVisitor(const OsmMap& map, const ElementId& eid) :
      _eid(eid),
      _map(map)
    {
      _found = false;
    }

    virtual void visit(const ConstElementPtr& e)
    {
      if (e->getElementId() == _eid)
      {
        _found = true;
      }
    }

    bool isFound() const { return _found; }

  private:
    ElementId _eid;
    const OsmMap& _map;
    bool _found;
  };

  class CheckVisitor : public ElementVisitor
  {
  public:
    CheckVisitor(const OsmMap& map, const ElementToRelationMap& mapping) :
      _map(map),
      _mapping(mapping),
      _logWarnCount(0)
    {
      _good = true;
    }

    bool containsRecursive(ConstRelationPtrR r, ElementId eid)
    {
      ContainsElementVisitor v(_map, eid);
      r->visitRo(_map, v);
      return v.isFound();
    }

    virtual void visit(const ConstElementPtr& e)
    {
      ElementType type = e->getElementType();
      long id = e->getId();

      // first check to see that this element maps to the right relations.
      const set<long>& mappedRelations = _mapping.getRelationByElement(ElementId(type, id));
      const RelationMap& relationMap = _map.getRelations();
      for (RelationMap::const_iterator it = relationMap.begin(); it != relationMap.end(); ++it)
      {
        bool inMappedRelation = mappedRelations.find(it->first) != mappedRelations.end();
        const boost::shared_ptr<const Relation>& r = it->second;
        ElementId childEid(type, id);
        if (inMappedRelation)
        {
          if (childEid != r->getElementId() && containsRecursive(r, ElementId(type, id)) == false)
          {
            if (_logWarnCount < ConfigOptions().getLogWarnMessageLimit())
            {
              LOG_WARN("ElementToRelationMap expected relation " << *it <<
                " to contain: " << type.toString() << " " << id << " but it does not.");
            }
            else if (_logWarnCount == ConfigOptions().getLogWarnMessageLimit())
            {
              LOG_WARN(typeid(this).name() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
            }
            _logWarnCount++;
            _good = false;
          }
        }
        else
        {
          if (containsRecursive(r, ElementId(type, id)) == true &&
            r->getElementId() != ElementId(type, id))
          {
            if (_logWarnCount < ConfigOptions().getLogWarnMessageLimit())
            {
              LOG_WARN("ElementToRelationMap didn't expect relation " << *it <<
                " to contain: " << type.toString() << " " << id << " but it does.");
            }
            else if (_logWarnCount == ConfigOptions().getLogWarnMessageLimit())
            {
              LOG_WARN(typeid(this).name() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
            }
            _logWarnCount++;
            _good = false;
          }
        }
      }
    }

    bool isGood() const { return _good; }

  private:
    const OsmMap& _map;
    const ElementToRelationMap& _mapping;
    bool _good;
    //this should be static, but there's no header file
    unsigned int _logWarnCount;
  };

  CheckVisitor visitor(map, *this);
  map.visitRo(visitor);
  return visitor.isGood();
}

}
