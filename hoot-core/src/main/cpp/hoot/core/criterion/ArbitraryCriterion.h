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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef ARBITRARYCRITERION_H
#define ARBITRARYCRITERION_H

// hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QString>

// Standard
#include <functional>

namespace hoot
{

class ArbitraryCriterion : public ElementCriterion
{
public:

  static QString className() { return "hoot::ArbitraryCriterion"; }

  explicit ArbitraryCriterion(std::function<bool (ConstElementPtr e)> f)
    : _f(f) { }
  explicit ArbitraryCriterion(std::function<bool (const std::shared_ptr<const Element> &e)> f)
    : _f(f) { }
  virtual ~ArbitraryCriterion() = default;

  virtual bool isSatisfied(const std::shared_ptr<const Element>& e) const
  {
    const bool result = _f(e);
    LOG_VART(result);
    return result;
  }

  virtual ElementCriterionPtr clone() { return ElementCriterionPtr(new ArbitraryCriterion(_f)); }

  virtual QString getDescription() const { return ""; }

  virtual QString getName() const override { return className(); }

private:

  std::function<bool (const std::shared_ptr<const Element>& e)> _f;
};

}

#endif // ARBITRARYCRITERION_H
