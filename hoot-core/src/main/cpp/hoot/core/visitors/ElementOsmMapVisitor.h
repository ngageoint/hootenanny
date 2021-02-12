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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 Maxar (http://www.maxar.com/)
 */
#ifndef ELEMENTOSMMAPVISITOR_H
#define ELEMENTOSMMAPVISITOR_H

// hoot
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/elements/ConstOsmMapConsumer.h>

namespace hoot
{

/**
 * Base class to ease OsmMapConsumer usage.
 */
class ElementOsmMapVisitor : public ConstElementVisitor, public ConstOsmMapConsumer
{
public:

  ElementOsmMapVisitor() = default;
  virtual ~ElementOsmMapVisitor() = default;

  virtual void setOsmMap(OsmMap* map) { _map = map; }
  virtual void setOsmMap(const OsmMap* /*map*/) { throw NotImplementedException(); }

  virtual void visit(const ConstElementPtr& e);
  virtual void visit(const std::shared_ptr<Element>& e) = 0;

protected:

  OsmMap* _map;
};

typedef std::shared_ptr<ElementOsmMapVisitor> ElementOsmMapVisitorPtr;
}

#endif // ELEMENTOSMMAPVISITOR_H
