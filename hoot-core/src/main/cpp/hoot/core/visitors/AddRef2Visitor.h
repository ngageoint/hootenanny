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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef ADDREF2VISITOR_H
#define ADDREF2VISITOR_H

// hoot
#include <hoot/core/visitors/ElementVisitor.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

class AddRef2Visitor : public ElementVisitor, public Configurable
{
public:

  static QString className() { return "AddRef2Visitor"; }

  AddRef2Visitor();
  ~AddRef2Visitor() override = default;

  void setConfiguration(const Settings& conf) override;

  /**
   * Adds a REF2 tag with a "todo" to all valid elements. If the Tags::getNonDebugCount() is greater
   * than zero then the element is tagged with a REF2.
   */
  void visit(const ElementPtr& e) override;

  QString getDescription() const override { return "Adds REF2 tags"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

private:

  bool _informationOnly;
};

}

#endif // ADDREF1VISITOR_H
