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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */
#include "SublineStringMatcherFactoryJs.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Settings.h>

#include <hoot/js/JsRegistrar.h>
#include <hoot/js/algorithms/subline-matching/SublineStringMatcherJs.h>
#include <hoot/js/elements/OsmMapJs.h>
#include <hoot/js/util/HootExceptionJs.h>

using namespace std;
using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(SublineStringMatcherFactoryJs)

void SublineStringMatcherFactoryJs::Init(Local<Object> exports)
{
  Isolate* current = exports->GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();
  Local<Object> factory = Object::New(current);
  exports->Set(context, toV8("SublineStringMatcherFactory"), factory);

  factory->Set(context, toV8("getMatcher"),
               FunctionTemplate::New(current, getMatcher)->GetFunction(context).ToLocalChecked());
}

void SublineStringMatcherFactoryJs::getMatcher(const v8::FunctionCallbackInfo<v8::Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  const CreatorDescription::BaseFeatureType featureType =
    CreatorDescription::stringToBaseFeatureType(toCpp<QString>(args[0]));
  ConstOsmMapPtr map;
  if (args.Length() == 2)
  {
    map = toCpp<ConstOsmMapPtr>(args[1]);
  }

  SublineStringMatcherPtr matcher = SublineStringMatcherFactory::getMatcher(featureType, map);
  args.GetReturnValue().Set(toV8(matcher));
}

}
