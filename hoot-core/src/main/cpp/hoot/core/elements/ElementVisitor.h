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
 * @copyright Copyright (C) 2015, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef ELEMENTVISITOR_H
#define ELEMENTVISITOR_H

// hoot
#include <hoot/core/elements/Element.h>

namespace hoot
{

/**
 * Visits elements in a collection in a that they can be modified.
 *
 * @note This interface hasn't been propagated throughout the code base so it will not likely be
 * honored in the same was as a ConstElementVisitor. Developers are encouraged to add ElementVisitor
 * support wherever it is relevant.
 */
class ElementVisitor
{
public:
  virtual ~ElementVisitor() {}

  static std::string className() { return "hoot::ElementVisitor"; }

  virtual void visit(const ElementPtr& e) = 0;

  /**
    Returns a string representation of the visitor
    */
  virtual QString toString() { return ""; }

  /**
   * Returns a one sentence description for the visitor.
   *
   * Keep this as short as possible, capitalize the first letter, and check to see that it stays
   * on one line when displayed with the 'operators' command.
   *
   * To prevent a visitor from being displayed by the 'operators' command, this
   * may be implemented as returning an empty string.
   */
  virtual QString getDescription() const = 0;
};

typedef boost::shared_ptr<ElementVisitor> ElementVisitorPtr;

}

#endif // ELEMENTVISITOR_H
