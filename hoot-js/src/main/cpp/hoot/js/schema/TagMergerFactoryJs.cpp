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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "TagMergerFactoryJs.h"

// hoot
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/js/elements/TagsJs.h>
#include <hoot/js/JsRegistrar.h>

using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(TagMergerFactoryJs)

TagMergerFactoryJs::TagMergerFactoryJs()
{
}

TagMergerFactoryJs::~TagMergerFactoryJs() {}

void TagMergerFactoryJs::Init(Handle<Object> exports)
{
  Isolate* current = exports->GetIsolate();
  HandleScope scope(current);
  Handle<Object> tagMergerFactory = Object::New(current);
  exports->Set(String::NewFromUtf8(current, "TagMergerFactory"), tagMergerFactory);
  tagMergerFactory->Set(String::NewFromUtf8(current, "mergeTags"),
    FunctionTemplate::New(current, mergeTags)->GetFunction());
}

void TagMergerFactoryJs::mergeTags(const FunctionCallbackInfo<Value>& args)
{
  HandleScope scope(args.GetIsolate());

  Tags t1 = toCpp<Tags>(args[0]->ToObject());
  Tags t2 = toCpp<Tags>(args[1]->ToObject());

  args.GetReturnValue().Set(TagsJs::New(TagMergerFactory::mergeTags(t1, t2, ElementType::Unknown)));
}

}
