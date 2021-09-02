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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef ADDUSERIDVISITOR_H
#define ADDUSERIDVISITOR_H

// hoot
#include <hoot/core/visitors/ElementVisitor.h>

namespace hoot
{

/**
 * Adds the username and user id as tags. If you want to limit the features that will be tagged then look
 * into FilteredVisitor.
 */
class AddUserIdVisitor : public ElementVisitor
{
public:

  static QString className() { return "AddUserIdVisitor"; }

  AddUserIdVisitor() = default;

  /**
   * Adds the user name and user id as tags to all valid elements.
   */
  void visit(const ElementPtr& e) override;

  QString getDescription() const override
  { return "Adds the user name and user id to elements as tags"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
};

}




#endif // ADDUSERIDVISITOR_H
