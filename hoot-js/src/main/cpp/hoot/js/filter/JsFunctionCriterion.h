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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef CUSTOMCRITERION_H
#define CUSTOMCRITERION_H

// hoot
#include <hoot/core/filters/ElementCriterion.h>
#include <hoot/js/util/JsFunctionConsumer.h>
#include <hoot/js/SystemNodeJs.h>

namespace hoot
{

/**
 * A filter that will either keep or remove matches.
 */
class JsFunctionCriterion : public ElementCriterion, public JsFunctionConsumer
{
public:

  static string className() { return "hoot::JsFunctionCriterion"; }

  JsFunctionCriterion() {}

  virtual void addFunction(Persistent<Function> func) { _func = Persistent<Function>(func); }

  bool isSatisfied(const boost::shared_ptr<const Element> &e) const;

  virtual ElementCriterion* clone() { return new JsFunctionCriterion(_func); }

private:
  JsFunctionCriterion(Persistent<Function> func) { _func = func; }

  Persistent<Function> _func;
};

}


#endif // CUSTOMCRITERION_H
