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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef ADDUUIDVISITOR_H
#define ADDUUIDVISITOR_H

// hoot
#include <hoot/core/util/Configurable.h>
#include <hoot/core/visitors/ElementVisitor.h>

namespace hoot
{

/**
 * Adds a UUID with a specific key. If you want to limit the features that will be tagged then look
 * into FilteredVisitor.
 */
class AddUuidVisitor : public ElementVisitor, public Configurable
{
public:

  static QString className() { return "AddUuidVisitor"; }

  AddUuidVisitor() = default;
  AddUuidVisitor(QString key);
  ~AddUuidVisitor() override = default;

  /**
   * Adds a tag with a UUID to all valid elements.
   */
  void visit(const ElementPtr& e) override;

  /**
   * @see Configurable
   */
  void setConfiguration(const Settings& conf) override;

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override { return "Adds unique ID tags"; }

private:

  QString _key;
};

}

#endif // ADDUUIDVISITOR_H
