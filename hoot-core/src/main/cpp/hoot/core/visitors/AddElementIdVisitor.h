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
 * @copyright Copyright (C) 2022 Maxar (http://www.maxar.com/)
 */
#ifndef ADDELEMENTIDVISITOR_H
#define ADDELEMENTIDVISITOR_H

// hoot
#include <hoot/core/visitors/ElementVisitor.h>

namespace hoot
{

/**
 * Adds a hoot:id tag with the ID of the element as the value.
 * For OGR exports, this can also be done with the "-D writer.include.id.tag"
 * config option. The config option uses the "AddExportTags" visitor which runs
 * BEFORE translation with OGR but AFTER translation when running convert.
 * If you want to limit the features that will be tagged then
 * look into FilteredVisitor.
 */
class AddElementIdVisitor : public ElementVisitor
{
public:

  static QString className() { return "AddElementIdVisitor"; }

  AddElementIdVisitor() = default;

  /**
   * Adds the user name and user id as tags to all valid elements.
   */
  void visit(const ElementPtr& e) override;

  QString getDescription() const override
  { return "Adds a hoot:id tag with the ID of the element as the value"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
};

}

#endif // ADDELEMENTIDVISITOR_H
