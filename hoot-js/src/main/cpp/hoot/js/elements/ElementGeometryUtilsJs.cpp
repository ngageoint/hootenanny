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
 * @copyright Copyright (C) 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "ElementGeometryUtilsJs.h"

// hoot
#include <hoot/core/elements/ElementGeometryUtils.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/geometry/GeometricRelationship.h>
#include <hoot/core/util/Settings.h>

#include <hoot/js/JsRegistrar.h>
#include <hoot/js/elements/ElementJs.h>
#include <hoot/js/elements/OsmMapJs.h>
#include <hoot/js/io/DataConvertJs.h>
#include <hoot/js/io/StreamUtilsJs.h>
#include <hoot/js/util/HootExceptionJs.h>
#include <hoot/js/util/StringUtilsJs.h>

using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(ElementGeometryUtilsJs)

void ElementGeometryUtilsJs::Init(Local<Object> exports)
{
  Isolate* current = exports->GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();
  Local<Object> thisObj = Object::New(current);
  exports->Set(context, toV8("ElementGeometryUtils"), thisObj);
  thisObj->Set(context, toV8("calculateLength"),
               FunctionTemplate::New(current, calculateLength)->GetFunction(context).ToLocalChecked());
}

void ElementGeometryUtilsJs::calculateLength(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
 
  try
  {
    Context::Scope context_scope(current->GetCurrentContext());

    ConstOsmMapPtr m = toCpp<ConstOsmMapPtr>(args[0]);
    ConstElementPtr e = toCpp<ConstElementPtr>(args[1]);

    args.GetReturnValue().Set(toV8(ElementGeometryUtils::calculateLength(e, m)));
  }
  catch (const HootException& err)
  {
    args.GetReturnValue().Set(current->ThrowException(HootExceptionJs::create(err)));
  }
}

}

