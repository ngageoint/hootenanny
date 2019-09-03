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
 * @copyright Copyright (C) 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef NONCONFLATABLECRITERION_H
#define NONCONFLATABLECRITERION_H

// hoot
#include <hoot/core/criterion/ElementCriterion.h>

// Qt
#include <QList>

namespace hoot
{

/**
 * A filter that will remove elements that aren't conflatable by Hootenanny. These are elements
 * for which we have no matchers defined.
 */
class NonConflatableCriterion : public ElementCriterion
{

public:

  static std::string className() { return "hoot::NonConflatableCriterion"; }

  NonConflatableCriterion();

  virtual bool isSatisfied(const ConstElementPtr& e) const override;

  virtual ElementCriterionPtr clone()
  {
    return ElementCriterionPtr(new NonConflatableCriterion());
  }

  virtual QString getDescription() const { return "Identifies features that are not conflatable"; }

  virtual QString toString() const override
  { return QString::fromStdString(className()).remove("hoot::"); }

};

}

#endif // NONCONFLATABLECRITERION_H
