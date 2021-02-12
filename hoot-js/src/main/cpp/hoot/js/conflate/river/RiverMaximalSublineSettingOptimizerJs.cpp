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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "RiverMaximalSublineSettingOptimizerJs.h"

// hoot
#include <hoot/core/conflate/river/RiverMaximalSublineSettingOptimizer.h>
#include <hoot/core/util/Log.h>

#include <hoot/js/JsRegistrar.h>
#include <hoot/js/io/DataConvertJs.h>
#include <hoot/js/elements/OsmMapJs.h>

using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(RiverMaximalSublineSettingOptimizerJs)

void RiverMaximalSublineSettingOptimizerJs::Init(Handle<Object> exports)
{
  Isolate* current = exports->GetIsolate();
  HandleScope scope(current);
  Handle<Object> thisObj = Object::New(current);
  exports->Set(String::NewFromUtf8(current, "RiverMaximalSublineSettingOptimizer"), thisObj);
  thisObj->Set(String::NewFromUtf8(current, "getFindBestMatchesMaxRecursions"),
               FunctionTemplate::New(current, getFindBestMatchesMaxRecursions)->GetFunction());
}

void RiverMaximalSublineSettingOptimizerJs::getFindBestMatchesMaxRecursions(
  const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  OsmMapJs* mapJs = ObjectWrap::Unwrap<OsmMapJs>(args[0]->ToObject());
  const int maxRecursions =
    RiverMaximalSublineSettingOptimizer().getFindBestMatchesMaxRecursions(mapJs->getConstMap());
  LOG_VARD(maxRecursions);

  args.GetReturnValue().Set(Number::New(current, maxRecursions));
}

}
