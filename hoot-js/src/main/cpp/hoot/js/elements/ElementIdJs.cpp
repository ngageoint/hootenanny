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
#include "ElementIdJs.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/util/StringUtilsJs.h>
#include <hoot/core/criterion/ElementCriterion.h>

// Qt
#include <QStringList>

using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(ElementIdJs)

Persistent<Function> ElementIdJs::_constructor;

void ElementIdJs::Init(Local<Object> target)
{
  Isolate* current = target->GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(current, New);
  tpl->SetClassName(String::NewFromUtf8(current, ElementId::className().toStdString().data()));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  // Prototype
  tpl->PrototypeTemplate()->Set(PopulateConsumersJs::baseClass(),
      String::NewFromUtf8(current, ElementId::className().toStdString().data()));
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(current, "getId"),
      FunctionTemplate::New(current, getType));
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(current, "getType"),
      FunctionTemplate::New(current, getType));
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(current, "toString"),
      FunctionTemplate::New(current, toString));

  _constructor.Reset(current, tpl->GetFunction(context).ToLocalChecked());
  target->Set(String::NewFromUtf8(current, "ElementId"), ToLocal(&_constructor));
}

Local<Object> ElementIdJs::New(ElementId eid)
{
  Isolate* current = v8::Isolate::GetCurrent();
  EscapableHandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();

  Local<Object> result = ToLocal(&_constructor)->NewInstance(context).ToLocalChecked();
  ElementIdJs* from = ObjectWrap::Unwrap<ElementIdJs>(result);
  from->_eid = eid;

  return scope.Escape(result);
}

void ElementIdJs::New(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  ElementIdJs* obj = new ElementIdJs();
  //  node::ObjectWrap::Wrap takes ownership of the pointer in a v8::Persistent<v8::Object>
  obj->Wrap(args.This());

  args.GetReturnValue().Set(args.This());
}

void ElementIdJs::getType(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  ElementId eid = ObjectWrap::Unwrap<ElementIdJs>(args.This())->getElementId();

  args.GetReturnValue().Set(String::NewFromUtf8(current, eid.getType().toString().toUtf8().data()));
}

void ElementIdJs::toString(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  ElementId eid = ObjectWrap::Unwrap<ElementIdJs>(args.This())->getElementId();

  args.GetReturnValue().Set(String::NewFromUtf8(current, eid.toString().toUtf8().data()));
}

}

