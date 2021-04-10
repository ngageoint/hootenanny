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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "TagsJs.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/elements/TagsJs.h>
#include <hoot/js/io/DataConvertJs.h>
#include <hoot/js/util/PopulateConsumersJs.h>
#include <hoot/js/util/StringUtilsJs.h>

// Qt
#include <QStringList>

using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(TagsJs)

Persistent<Function> TagsJs::_constructor;

void TagsJs::contains(const FunctionCallbackInfo<Value>& args)
{
  HandleScope scope(args.GetIsolate());

  Tags& t = ObjectWrap::Unwrap<TagsJs>(args.This())->getTags();

  QString key = toCpp<QString>(args[0]);

  args.GetReturnValue().Set(toV8(t.contains(key)));
}

void TagsJs::Init(Handle<Object> target)
{
  Isolate* current = v8::Isolate::GetCurrent();
  HandleScope scope(current);
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(current, New);
  tpl->SetClassName(String::NewFromUtf8(current, Tags::className().toStdString().data()));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  // Prototype
  tpl->PrototypeTemplate()->Set(PopulateConsumersJs::baseClass(),
    String::NewFromUtf8(current, Tags::className().toStdString().data()));
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(current, "contains"),
      FunctionTemplate::New(current, contains));
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(current, "get"),
      FunctionTemplate::New(current, get));
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(current, "getInformationCount"),
      FunctionTemplate::New(current, getInformationCount));
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(current, "getNames"),
      FunctionTemplate::New(current, getNames));
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(current, "set"),
      FunctionTemplate::New(current, set));
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(current, "toDict"),
      FunctionTemplate::New(current, toDict));
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(current, "toString"),
      FunctionTemplate::New(current, toString));

  _constructor.Reset(current, tpl->GetFunction());
  target->Set(String::NewFromUtf8(current, "Tags"), ToLocal(&_constructor));
}

Handle<Object> TagsJs::New(const Tags& t)
{
  EscapableHandleScope scope(v8::Isolate::GetCurrent());

  Handle<Object> result = ToLocal(&_constructor)->NewInstance();
  TagsJs* from = ObjectWrap::Unwrap<TagsJs>(result);
  from->_setTags(t);

  return scope.Escape(result);
}

void TagsJs::New(const FunctionCallbackInfo<Value>& args)
{
  HandleScope scope(args.GetIsolate());

  TagsJs* obj = new TagsJs();
  //  node::ObjectWrap::Wrap takes ownership of the pointer in a v8::Persistent<v8::Object>
  obj->Wrap(args.This());

  args.GetReturnValue().Set(args.This());
}

void TagsJs::get(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  Tags& t = ObjectWrap::Unwrap<TagsJs>(args.This())->getTags();

  QString key = str(args[0]->ToString());
  if (t.contains(key))
  {
    QString value = t.get(key);
    args.GetReturnValue().Set(String::NewFromUtf8(current, value.toUtf8().data()));
  }
  else
  {
    args.GetReturnValue().SetUndefined();
  }
}

void TagsJs::getInformationCount(const FunctionCallbackInfo<Value>& args)
{
  HandleScope scope(args.GetIsolate());

  Tags& t = ObjectWrap::Unwrap<TagsJs>(args.This())->getTags();

  args.GetReturnValue().Set(toV8(t.getInformationCount()));
}

void TagsJs::getNames(const FunctionCallbackInfo<Value>& args)
{
  HandleScope scope(args.GetIsolate());

  Tags& t = ObjectWrap::Unwrap<TagsJs>(args.This())->getTags();

  args.GetReturnValue().Set(toV8(t.getNames()));
}

void TagsJs::set(const FunctionCallbackInfo<Value>& args)
{
  HandleScope scope(args.GetIsolate());

  Tags& t = ObjectWrap::Unwrap<TagsJs>(args.This())->getTags();

  QString key = toCpp<QString>(args[0]);
  QString value = toCpp<QString>(args[1]);
  t.set(key, value);

  args.GetReturnValue().SetUndefined();
}

void TagsJs::toDict(const FunctionCallbackInfo<Value>& args)
{
  HandleScope scope(args.GetIsolate());

  Tags& t = ObjectWrap::Unwrap<TagsJs>(args.This())->getTags();

  args.GetReturnValue().Set(toV8(t));
}

void TagsJs::toString(const FunctionCallbackInfo<Value>& args)
{
  HandleScope scope(args.GetIsolate());

  Tags& t = ObjectWrap::Unwrap<TagsJs>(args.This())->getTags();

  args.GetReturnValue().Set(toV8(t.toString()));
}

}

