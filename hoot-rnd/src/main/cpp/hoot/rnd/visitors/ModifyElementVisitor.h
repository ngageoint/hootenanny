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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef MODIFYELEMENTVISITOR_H
#define MODIFYELEMENTVISITOR_H

// boost
#include <boost/shared_ptr.hpp>

// hoot
#include <hoot/core/elements/Element.h>

namespace hoot
{

class ModifyElementVisitor
{
public:
    virtual ~ModifyElementVisitor() {}

    static std::string className() { return "hoot::ModifyElementVisitor"; }

    virtual void visit(const ElementPtr& e) = 0;

    /**
      Returns a string representation of the visitor
      */
    virtual QString toString() { return ""; }
};

typedef boost::shared_ptr<ModifyElementVisitor> ModifyElementVisitorPtr;
typedef boost::shared_ptr<const ModifyElementVisitor> ConstModifyElementVisitorPtr;

}

#endif // MODIFYELEMENTVISITOR_H
