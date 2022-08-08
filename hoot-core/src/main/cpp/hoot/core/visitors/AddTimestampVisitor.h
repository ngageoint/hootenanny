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
 * @copyright Copyright (C) 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef ADDTIMESTAMPVISITOR_H
#define ADDTIMESTAMPVISITOR_H

// hoot
#include <hoot/core/visitors/ElementVisitor.h>

namespace hoot
{

/**
 * Adds the timestamp from the element as a tag. If you want to limit the features that will be tagged then look
 * into FilteredVisitor.
 */
class AddTimestampVisitor : public ElementVisitor
{
public:

  static QString className() { return "AddTimestampVisitor"; }

  AddTimestampVisitor() = default;

  /**
   * Adds the element timestamp as a tag to all valid elements.
   */
  void visit(const ElementPtr& e) override;

  QString getDescription() const override
  { return "Adds the element timestamp as a tag"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
};

}

#endif // ADDTIMESTAMPVISITOR_H
