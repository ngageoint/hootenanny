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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef CUSTOMCRITERION_H
#define CUSTOMCRITERION_H

// hoot
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/js/SystemNodeJs.h>
#include <hoot/js/util/JsFunctionConsumer.h>

namespace hoot
{

/**
 * A criterion that will either keep or remove matches.
 */
class JsFunctionCriterion : public ElementCriterion, public JsFunctionConsumer
{
public:

  static std::string className() { return "hoot::JsFunctionCriterion"; }

  JsFunctionCriterion() {}

  virtual void addFunction(v8::Isolate* isolate, v8::Local<v8::Function>& func)
  { _func.Reset(isolate, func); }

  virtual bool isSatisfied(const ConstElementPtr& e) const override;

  virtual ElementCriterionPtr clone()
  { return ElementCriterionPtr(new JsFunctionCriterion(_func)); }

  virtual QString getDescription() const { return ""; }

  virtual QString toString() const override { return ""; }

private:

  JsFunctionCriterion(v8::Persistent<v8::Function>& func)
  { _func.Reset(v8::Isolate::GetCurrent(), func); }

  v8::Persistent<v8::Function> _func;
};

}


#endif // CUSTOMCRITERION_H
