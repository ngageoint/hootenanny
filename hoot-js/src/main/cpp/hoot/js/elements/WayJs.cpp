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
#include "WayJs.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Configurable.h>

#include <hoot/core/util/Settings.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/util/PopulateConsumersJs.h>
#include <hoot/js/util/StringUtilsJs.h>

// Qt


using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(WayJs)

Persistent<Function> WayJs::_constructor;

void WayJs::Init(Local<Object> target)
{
  Isolate* current = target->GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(current, New);
  tpl->SetClassName(String::NewFromUtf8(current, Way::className().toStdString().data()).ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  // Prototype
  ElementJs::_addBaseFunctions(tpl);
  tpl->PrototypeTemplate()->Set(current, "getNodeCount", FunctionTemplate::New(current, getNodeCount));

  _constructor.Reset(current, tpl->GetFunction(context).ToLocalChecked());
  target->Set(context, toV8("Way"), ToLocal(&_constructor));
}

Local<Object> WayJs::New(ConstWayPtr way)
{
  Isolate* current = v8::Isolate::GetCurrent();
  EscapableHandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();

  Local<Object> result = ToLocal(&_constructor)->NewInstance(context).ToLocalChecked();
  WayJs* from = ObjectWrap::Unwrap<WayJs>(result);
  from->_setWay(way);

  return scope.Escape(result);
}

Local<Object> WayJs::New(WayPtr way)
{
  Isolate* current = v8::Isolate::GetCurrent();
  EscapableHandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();

  Local<Object> result = ToLocal(&_constructor)->NewInstance(context).ToLocalChecked();
  WayJs* from = ObjectWrap::Unwrap<WayJs>(result);
  from->_setWay(way);

  return scope.Escape(result);
}

void WayJs::New(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  WayJs* obj = new WayJs();
  //  node::ObjectWrap::Wrap takes ownership of the pointer in a v8::Persistent<v8::Object>
  obj->Wrap(args.This());

  args.GetReturnValue().Set(args.This());
}

void WayJs::getNodeCount(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  ConstWayPtr way = ObjectWrap::Unwrap<WayJs>(args.This())->getConstWay();
  args.GetReturnValue().Set(Number::New(current, way->getNodeCount()));
}

}
