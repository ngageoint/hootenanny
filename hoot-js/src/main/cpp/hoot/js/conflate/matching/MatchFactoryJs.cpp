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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "MatchFactoryJs.h"

// hoot
#include <hoot/core/conflate/matching/MatchFactory.h>
#include <hoot/js/JsRegistrar.h>

using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(MatchFactoryJs)

void MatchFactoryJs::Init(Handle<Object> exports)
{
  Isolate* current = exports->GetIsolate();
  HandleScope scope(current);
  Handle<Object> schema = Object::New(current);
  exports->Set(String::NewFromUtf8(current, "MatchFactory"), schema);
  schema->Set(String::NewFromUtf8(current, "getAllAvailableCreators"),
    FunctionTemplate::New(current, getAllAvailableCreators)->GetFunction());
}

void MatchFactoryJs::getAllAvailableCreators(const FunctionCallbackInfo<Value>& args)
{
  HandleScope scope(args.GetIsolate());

  args.GetReturnValue().Set(toV8(MatchFactory::getInstance().getAllAvailableCreators()));
}

}
