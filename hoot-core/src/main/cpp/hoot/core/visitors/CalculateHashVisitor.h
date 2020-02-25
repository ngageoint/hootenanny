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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef CALCULATEHASHVISITOR_H
#define CALCULATEHASHVISITOR_H

// hoot
#include <hoot/core/elements/ElementVisitor.h>

namespace hoot
{

/**
 * Calculates a hash for each element and store the hash in the MetadataTags::HootHash() tag. Any
 * existing value will be overwritten.
 *
 * See this for details: https://github.com/ngageoint/hootenanny/issues/1663
 *
 * This implementation only works for nodes and uses some hacked together string creation
 * code rather than re-using the GeoJson code in: https://github.com/ngageoint/hootenanny/pull/1658
 * When #1658 is merged it probably makes sense to refactor this to use the GeoJSON writer.
 */
class CalculateHashVisitor : public ElementVisitor
{
public:

  static std::string className() { return "hoot::CalculateHashVisitor"; }

  CalculateHashVisitor();

  virtual void visit(const ElementPtr &e);

  static QString toJsonString(const ConstElementPtr& e);

  static QByteArray toHash(const ConstElementPtr& e);

  static QString toHashString(const ConstElementPtr& e);

  virtual QString getDescription() const { return "Calculates unique hash values for elements"; }

  virtual std::string getClassName() const { return className(); }
};

}

#endif // CALCULATEHASHVISITOR_H
