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
 * @copyright Copyright (C) 2015, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef __ELEMENT_CRITERIONCONSUMER_H
#define __ELEMENT_CRITERIONCONSUMER_H

// Hoot
#include <hoot/core/criterion/ElementCriterion.h>

namespace hoot
{

class ElementCriterionConsumer
{
public:

  static QString className() { return "ElementCriterionConsumer"; }

  ElementCriterionConsumer() = default;
  virtual ~ElementCriterionConsumer() = default;

  virtual void addCriterion(const ElementCriterionPtr& crit) = 0;
};

}


#endif // __ELEMENT_CRITERIONCONSUMER_H
