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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef ELEMENTVISITOR_H
#define ELEMENTVISITOR_H

// hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/info/ApiEntityInfo.h>

namespace hoot
{

/**
 * Visits elements in a collection in a way that they can be modified.
 */
class ElementVisitor : public ApiEntityInfo
{
public:

  ElementVisitor() : _numAffected(0), _numProcessed(0) {}
  virtual ~ElementVisitor() {}

  static std::string className() { return "hoot::ElementVisitor"; }

  virtual void visit(const ElementPtr& e) = 0;

protected:

  // These will only be used by those implementing OperationStatusInfo.
  long _numAffected;    // how many elements the operation actually counted or did something to
  long _numProcessed;   // how many elements the operation processed total
};

typedef boost::shared_ptr<ElementVisitor> ElementVisitorPtr;

}

#endif // ELEMENTVISITOR_H
