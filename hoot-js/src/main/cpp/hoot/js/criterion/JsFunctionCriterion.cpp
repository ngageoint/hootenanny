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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "JsFunctionCriterion.h"

// hoot
#include <hoot/core/util/Factory.h>

#include <hoot/js/elements/ElementJs.h>
#include <hoot/js/io/DataConvertJs.h>
#include <hoot/js/util/HootExceptionJs.h>

using namespace v8;

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, JsFunctionCriterion)

bool JsFunctionCriterion::isSatisfied(const ConstElementPtr& e) const
{
  Isolate* current = Isolate::GetCurrent();
  HandleScope handleScope(current);
  Context::Scope context_scope(current->GetCurrentContext());
  Local<Context> context = current->GetCurrentContext();

  Local<Value> jsArgs[3];

  if (_func.IsEmpty())
    throw IllegalArgumentException("JsFunctionCriterion must have a valid function.");

  Local<Object> elementObj = ElementJs::New(e);

  int argc = 0;
  jsArgs[argc++] = elementObj;

  TryCatch trycatch(current);
  MaybeLocal<Value> maybe_funcResult = ToLocal(&_func)->Call(context, context->Global(), argc, jsArgs);
  // avoids a warning, the default value of false should never be used.
  bool result = false;

  if (maybe_funcResult.IsEmpty())
  {
    Local<Value> exception = trycatch.Exception();
    if (HootExceptionJs::isHootException(exception))
    {
      std::shared_ptr<HootException> ex = toCpp<std::shared_ptr<HootException>>(exception);
      HootExceptionThrower::getInstance().rethrowPointer(ex);
    }
    else
    {
      // Matt, if this conflicts with the newer more robust handling, use that.
      throw HootException(toJson(trycatch.Message()->Get()));
    }
  }
  else if (maybe_funcResult.ToLocalChecked()->IsBoolean() == false)
    throw IllegalArgumentException("Expected a boolean to be returned from JsFunctionCriterion function.");
  else
    result = maybe_funcResult.ToLocalChecked()->BooleanValue(current);

  return result;
}

}
