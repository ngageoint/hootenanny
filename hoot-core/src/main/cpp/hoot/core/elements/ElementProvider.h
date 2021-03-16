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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef __ELEMENT_PROVIDER_H__
#define __ELEMENT_PROVIDER_H__

// std
#include <memory>

#include <hoot/core/elements/Element.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/elements/Way.h>

// GDAL
class OGRSpatialReference;

namespace hoot
{

class ElementProvider
{
public:

  ElementProvider() = default;
  virtual ~ElementProvider() = default;

  virtual std::shared_ptr<OGRSpatialReference> getProjection() const = 0;

  virtual bool containsElement(const ElementId& eid) const = 0;
  virtual ConstElementPtr getElement(const ElementId& id) const = 0;

  virtual ConstNodePtr getNode(long id) const = 0;
  virtual NodePtr getNode(long id) = 0;

  virtual ConstRelationPtr getRelation(long id) const = 0;
  virtual RelationPtr getRelation(long id) = 0;

  virtual ConstWayPtr getWay(long id) const = 0;
  virtual WayPtr getWay(long id) = 0;

  virtual bool containsNode(long id) const = 0;
  virtual bool containsRelation(long id) const = 0;
  virtual bool containsWay(long id) const = 0;
};

typedef std::shared_ptr<const ElementProvider> ConstElementProviderPtr;
typedef std::shared_ptr<ElementProvider> ElementProviderPtr;

}

#endif // __ELEMENT_PROVIDER_H__
