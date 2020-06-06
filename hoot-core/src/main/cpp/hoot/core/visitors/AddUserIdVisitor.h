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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef ADDUSERIDVISITOR_H
#define ADDUSERIDVISITOR_H

// hoot
#include <hoot/core/elements/ElementVisitor.h>

namespace hoot
{

/**
 * Adds the username and user id as tags. If you want to limit the features that will be tagged then look
 * into FilteredVisitor.
 */
class AddUserIdVisitor : public ElementVisitor
{
public:

  static std::string className() { return "hoot::AddUserIdVisitor"; }

  AddUserIdVisitor();

  /**
   * Adds the user name and user id as tags to all valid elements.
   */
  virtual void visit(const ElementPtr& e);

  virtual QString getDescription() const { return "Adds the user name and user id to elements as tags"; }

  virtual std::string getClassName() const { return className(); }

//private:
};

}




#endif // ADDUSERIDVISITOR_H
