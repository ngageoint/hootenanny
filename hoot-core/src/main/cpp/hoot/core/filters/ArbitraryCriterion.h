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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef ARBITRARYCRITERION_H
#define ARBITRARYCRITERION_H

// hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/filters/ElementCriterion.h>

// Qt
#include <QString>

// Boost
#include <boost/function.hpp>

namespace hoot
{
using namespace boost;

class ArbitraryCriterion : public ElementCriterion
{
public:
  static string className() { return "hoot::ArbitraryCriterion"; }

  // Do something like:
  // boost::function<bool (ConstElementPtr e)> f = boost::bind(&ScriptMatchVisitor::isMatchCandidate, this, _1);
  explicit ArbitraryCriterion(boost::function<bool (ConstElementPtr e)> f)
  {
    _f = f;
  }

  explicit ArbitraryCriterion(boost::function<bool (const shared_ptr<const Element> &e)> f)
  {
    _f = f;
  }

  virtual bool isSatisfied(const shared_ptr<const Element> &e) const
  {
    return _f(e);
  }

  virtual ElementCriterion* clone() { return new ArbitraryCriterion(_f); }

private:
  boost::function<bool (const shared_ptr<const Element> &e)> _f;
};

}

#endif // ARBITRARYCRITERION_H
