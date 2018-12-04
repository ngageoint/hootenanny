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
 * @copyright Copyright (C) 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef MULTILINESTRING_CRITERION_H
#define MULTILINESTRING_CRITERION_H

#include <hoot/core/criterion/ElementCriterion.h>

namespace hoot
{

/**
 * Identifies multi-line string features
 */
class MultiLineStringCriterion : public ElementCriterion
{
public:

  static std::string className() { return "hoot::MultiLineStringCriterion"; }

  MultiLineStringCriterion() {}

  virtual bool isSatisfied(const ConstElementPtr& e) const;

  virtual ElementCriterionPtr clone()
  { return ElementCriterionPtr(new MultiLineStringCriterion()); }

  virtual QString getDescription() const { return "Identifies multi-line string features"; }
};

}

#endif // MULTILINESTRING_CRITERION_H
