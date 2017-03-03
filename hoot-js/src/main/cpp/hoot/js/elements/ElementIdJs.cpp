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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ElementIdJs.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/util/StringUtilsJs.h>
#include <hoot/core/filters/ElementCriterion.h>

// Qt
#include <QStringList>

// Tgs
#include <tgs/SharedPtr.h>

namespace hoot
{

HOOT_JS_REGISTER(ElementIdJs)

Persistent<Function> ElementIdJs::_constructor;

ElementIdJs::ElementIdJs()
{
}

ElementIdJs::~ElementIdJs()
{
}

void ElementIdJs::Init(Handle<Object> target)
{
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol(Element::className().data()));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  // Prototype
  tpl->PrototypeTemplate()->Set(PopulateConsumersJs::baseClass(),
    String::New(ElementId::className().data()));
  tpl->PrototypeTemplate()->Set(String::NewSymbol("getId"),
      FunctionTemplate::New(getType)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("getType"),
      FunctionTemplate::New(getType)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("toString"),
      FunctionTemplate::New(toString)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("toJSON"),
      FunctionTemplate::New(toJSON)->GetFunction());
  tpl->PrototypeTemplate()->Set(PopulateConsumersJs::baseClass(),
                                String::New(ElementId::className().data()));


  _constructor = Persistent<Function>::New(tpl->GetFunction());
  target->Set(String::NewSymbol("ElementId"), _constructor);
}

Handle<Object> ElementIdJs::New(ElementId eid)
{
  HandleScope scope;

  Handle<Object> result = _constructor->NewInstance();
  ElementIdJs* from = ObjectWrap::Unwrap<ElementIdJs>(result);
  from->_eid = eid;

  return scope.Close(result);
}

Handle<Value> ElementIdJs::New(const Arguments& args)
{
  HandleScope scope;

  ElementIdJs* obj = new ElementIdJs();
  obj->Wrap(args.This());

  return args.This();
}

Handle<Value> ElementIdJs::getType(const Arguments& args)
{
  HandleScope scope;

  ElementId eid = ObjectWrap::Unwrap<ElementIdJs>(args.This())->getElementId();

  return scope.Close(String::New(eid.getType().toString().toUtf8().data()));
}

Handle<Value> ElementIdJs::toJSON(const Arguments& args)
{
  HandleScope scope;

  ElementId eid = ObjectWrap::Unwrap<ElementIdJs>(args.This())->getElementId();

  Handle<Object> result = Object::New();
  result->Set(String::NewSymbol("type"), String::New(eid.getType().toString().toUtf8().data()));
  result->Set(String::NewSymbol("id"), v8::Integer::New(eid.getId()));

  return scope.Close(result);
}

Handle<Value> ElementIdJs::toString(const Arguments& args)
{
  HandleScope scope;

  ElementId eid = ObjectWrap::Unwrap<ElementIdJs>(args.This())->getElementId();

  return scope.Close(String::New(eid.toString().toUtf8().data()));
}

}

