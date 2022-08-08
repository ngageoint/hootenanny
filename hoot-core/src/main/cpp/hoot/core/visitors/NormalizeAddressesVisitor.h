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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#ifndef NORMALIZE_ADDRESSES_VISITOR_H
#define NORMALIZE_ADDRESSES_VISITOR_H

// hoot
#include <hoot/core/visitors/ElementVisitor.h>
#include <hoot/core/conflate/address/AddressNormalizer.h>

namespace hoot
{

/**
 * Normalizes element address tag values
 */
class NormalizeAddressesVisitor : public ElementVisitor
{
public:

  static QString className() { return "NormalizeAddressesVisitor"; }

  NormalizeAddressesVisitor() = default;
  ~NormalizeAddressesVisitor() override = default;

  /**
   * @see ElementVisitor
   */
  void visit(const ElementPtr& e) override;

  QString getInitStatusMessage() const override { return "Normalizing addresses..."; }
  QString getCompletedStatusMessage() const override
  { return "Normalized " + QString::number(_addressNormalizer.getNumNormalized()) + " addresses"; }

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override { return "Normalizes addresses"; }

private:

  friend class AddressConflateTest;

  AddressNormalizer _addressNormalizer;
};

}

#endif // NORMALIZE_ADDRESSES_VISITOR_H
