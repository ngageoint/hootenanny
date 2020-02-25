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
#ifndef CALCULATEHASHVISITOR2_H
#define CALCULATEHASHVISITOR2_H

// hoot
#include <hoot/core/elements/ElementVisitor.h>

namespace hoot
{

/**
 * Wrapper around CalculateHashVisitor for use with MultiaryIngestCmd - This very well could be
 * replaced by CalculateHashVisitor...but just haven't figured out how to make that work yet.
 *
 * TODO: implement OperationStatusInfo
 */
class CalculateHashVisitor2 : public ElementVisitor
{
public:

  static std::string className() { return "hoot::CalculateHashVisitor2"; }

  virtual void visit(const ElementPtr& e);

  virtual QString getDescription() const
  { return "Calculates unique hash values for elements used by the multiary ingester"; }

  virtual std::string getClassName() const { return className(); }
};

}

#endif // CALCULATEHASHVISITOR_H
