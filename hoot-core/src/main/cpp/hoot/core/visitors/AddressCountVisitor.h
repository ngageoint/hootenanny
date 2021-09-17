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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef ADDRESS_COUNT_VISITOR_H
#define ADDRESS_COUNT_VISITOR_H

// hoot
#include <hoot/core/visitors/ConstElementVisitor.h>
#include <hoot/core/info/SingleStatistic.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/conflate/address/AddressParser.h>

namespace hoot
{

/**
 * Counts the total number of valid element addresses where an element may have more than one
 * address.
 */
class AddressCountVisitor : public ConstElementVisitor, public SingleStatistic, public Configurable
{
public:

  static QString className() { return "AddressCountVisitor"; }

  AddressCountVisitor();
  ~AddressCountVisitor() override = default;

  void setConfiguration(const Settings& conf) override;

  double getStat() const override { return _totalCount; }

  void visit(const ConstElementPtr& e) override;

  QString getDescription() const override
  { return "Counts the total number of valid element addresses"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

private:

  int _totalCount;
  AddressParser _addressParser;
};

}

#endif // ADDRESS_COUNT_VISITOR_H
