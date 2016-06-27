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
#ifndef FINDWAYSVISITOR_H
#define FINDWAYSVISITOR_H

#include <hoot/core/visitors/ElementConstOsmMapVisitor.h>
#include <hoot/core/filters/ElementCriterion.h>

// For convenience functions
#include <hoot/core/filters/ContainsNodeCriterion.h>
#include <hoot/core/filters/TagCriterion.h>

namespace hoot
{

// Used to get a vector of IDs for the ways that satisfy
// the specified criterion
class FindWaysVisitor :  public ElementConstOsmMapVisitor
{
public:

  FindWaysVisitor (ElementCriterion* pCrit):
    _pCrit(pCrit)
  {
    // Nothing
  }

  virtual void setOsmMap(const OsmMap* map) { _map = map; }

  virtual void visit(const shared_ptr<const Element>& e)
  {
    if (e->getElementType() == ElementType::Way)
    {
      ConstWayPtr w = dynamic_pointer_cast<const Way>(e);
      if (_pCrit->isSatisfied(e))
      {
        _wayIds.push_back(e->getId());
      }
    }
  }

  vector<long> getIds() { return _wayIds; }

  // Convenience method for finding ways that contain the given node
  static vector<long> findWaysByNode(const ConstOsmMapPtr& map, long nodeId)
  {
    ContainsNodeCriterion crit(nodeId);
    FindWaysVisitor v(&crit);
    map->visitWaysRo(v);
    return v.getIds();
  }

  // Convenience method for finding ways that contain the given tag
  static vector<long> findWaysByTag(const ConstOsmMapPtr& map,
                                    const QString& key,
                                    const QString& value)
  {
    TagCriterion crit(key, value);
    FindWaysVisitor v(&crit);
    map->visitWaysRo(v);
    return v.getIds();
  }

private:
  const OsmMap* _map;
  vector<long> _wayIds;
  ElementCriterion * _pCrit;
};

}

#endif // FINDWAYSVISITOR_H
