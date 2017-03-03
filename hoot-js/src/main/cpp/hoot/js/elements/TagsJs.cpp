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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "TagsJs.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/elements/TagsJs.h>
#include <hoot/js/util/DataConvertJs.h>
#include <hoot/js/util/PopulateConsumersJs.h>
#include <hoot/js/util/StringUtilsJs.h>

// Qt
#include <QStringList>

// Tgs
#include <tgs/SharedPtr.h>

namespace hoot
{

HOOT_JS_REGISTER(TagsJs)

Persistent<Function> TagsJs::_constructor;

TagsJs::TagsJs()
{
}

TagsJs::~TagsJs()
{
}

Handle<Value> TagsJs::contains(const Arguments& args)
{
  HandleScope scope;

  Tags& t = ObjectWrap::Unwrap<TagsJs>(args.This())->getTags();

  QString key = toCpp<QString>(args[0]);

  return scope.Close(toV8(t.contains(key)));
}

void TagsJs::Init(Handle<Object> target)
{
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol(Tags::className().data()));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  // Prototype
  tpl->PrototypeTemplate()->Set(PopulateConsumersJs::baseClass(),
    String::New(Tags::className().data()));
  tpl->PrototypeTemplate()->Set(String::NewSymbol("contains"),
      FunctionTemplate::New(contains)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("get"),
      FunctionTemplate::New(get)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("getCreateUuid"),
      FunctionTemplate::New(getCreateUuid)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("getLengthInMeters"),
      FunctionTemplate::New(getLengthInMeters)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("getVelocityInMeters"),
      FunctionTemplate::New(getVelocityInMeters)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("getInformationCount"),
      FunctionTemplate::New(getInformationCount)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("set"),
      FunctionTemplate::New(set)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("toDict"),
      FunctionTemplate::New(toDict)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("toString"),
      FunctionTemplate::New(toString)->GetFunction());

  _constructor = Persistent<Function>::New(tpl->GetFunction());
  target->Set(String::NewSymbol("Tags"), _constructor);
}

Handle<Object> TagsJs::New(const Tags& t)
{
  HandleScope scope;

  Handle<Object> result = _constructor->NewInstance();
  TagsJs* from = ObjectWrap::Unwrap<TagsJs>(result);
  from->_setTags(t);

  return scope.Close(result);
}

Handle<Value> TagsJs::New(const Arguments& args)
{
  HandleScope scope;

  TagsJs* obj = new TagsJs();
  obj->Wrap(args.This());

  return args.This();
}

Handle<Value> TagsJs::get(const Arguments& args)
{
  HandleScope scope;

  Tags& t = ObjectWrap::Unwrap<TagsJs>(args.This())->getTags();

  QString key = str(args[0]->ToString());
  if (t.contains(key))
  {
    QString value = t.get(key);
    return scope.Close(String::New(value.toUtf8().data()));
  }
  else
  {
    return scope.Close(Undefined());
  }
}

Handle<Value> TagsJs::getCreateUuid(const Arguments& args)
{
  HandleScope scope;

  Tags& t = ObjectWrap::Unwrap<TagsJs>(args.This())->getTags();

  return scope.Close(toV8(t.getCreateUuid()));
}

Handle<Value> TagsJs::getLengthInMeters(const Arguments& args)
{
  HandleScope scope;

  Tags& t = ObjectWrap::Unwrap<TagsJs>(args.This())->getTags();

  QString key = str(args[0]->ToString());

  return scope.Close(toV8(t.getLength(key).value()));
}

Handle<Value> TagsJs::getVelocityInMeters(const Arguments& args)
{
  HandleScope scope;

  Tags& t = ObjectWrap::Unwrap<TagsJs>(args.This())->getTags();

  QString key = str(args[0]->ToString());

  return scope.Close(toV8(t.getVelocity(key).value()));
}

Handle<Value> TagsJs::getInformationCount(const Arguments& args)
{
  HandleScope scope;

  Tags& t = ObjectWrap::Unwrap<TagsJs>(args.This())->getTags();

  return scope.Close(toV8(t.getInformationCount()));
}

Handle<Value> TagsJs::set(const Arguments& args)
{
  HandleScope scope;

  Tags& t = ObjectWrap::Unwrap<TagsJs>(args.This())->getTags();

  QString key = toCpp<QString>(args[0]);
  QString value = toCpp<QString>(args[1]);
  t.set(key, value);

  return scope.Close(Undefined());
}

Handle<Value> TagsJs::toDict(const Arguments& args)
{
  HandleScope scope;

  Tags& t = ObjectWrap::Unwrap<TagsJs>(args.This())->getTags();

  return scope.Close(toV8(t));
}


Handle<Value> TagsJs::toString(const Arguments& args)
{
  HandleScope scope;

  Tags& t = ObjectWrap::Unwrap<TagsJs>(args.This())->getTags();

  return scope.Close(toV8(t.toString()));
}

}

