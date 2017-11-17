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
 * @copyright Copyright (C) 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef ONEWAYCRITERION_H
#define ONEWAYCRITERION_H

// hoot
#include <hoot/core/filters/ElementCriterion.h>

namespace hoot
{

/**
 * Keeps all the one-way elements
 */
class OneWayCriterion : public ElementCriterion
{
public:

  static std::string className() { return "hoot::OneWayCriterion"; }

  OneWayCriterion(bool isOneWay = true): _isOneWay(isOneWay) { }

  virtual bool isSatisfied(const boost::shared_ptr<const Element> &e) const;

  ElementCriterionPtr clone() { return ElementCriterionPtr(new OneWayCriterion()); }

private:

  bool _isOneWay;
};

}

#endif // ONEWAYCRITERION_H
