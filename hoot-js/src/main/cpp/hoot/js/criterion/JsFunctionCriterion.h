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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
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
 * A criterion that uses a JS function to evaluate elements.
 */
class JsFunctionCriterion : public ElementCriterion, public JsFunctionConsumer
{
public:

  static QString className() { return "hoot::JsFunctionCriterion"; }

  JsFunctionCriterion() = default;
  ~JsFunctionCriterion() = default;

  void addFunction(v8::Isolate* isolate, v8::Local<v8::Function>& func) override
  { _func.Reset(isolate, func); }

  bool isSatisfied(const ConstElementPtr& e) const override;

  ElementCriterionPtr clone() override
  { return ElementCriterionPtr(new JsFunctionCriterion(_func)); }

  QString getDescription() const override { return ""; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

private:

  JsFunctionCriterion(v8::Persistent<v8::Function>& func)
  { _func.Reset(v8::Isolate::GetCurrent(), func); }

  v8::Persistent<v8::Function> _func;
};

}


#endif // CUSTOMCRITERION_H
