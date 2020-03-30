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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "RelationJs.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Settings.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/util/HootExceptionJs.h>
#include <hoot/js/util/PopulateConsumersJs.h>
#include <hoot/js/util/StringUtilsJs.h>

// Qt
#include <QStringList>

using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(RelationJs)

Persistent<Function> RelationJs::_constructor;

RelationJs::RelationJs(ConstRelationPtr r) : _constRelation(r)
{
}

RelationJs::RelationJs()
{
}

RelationJs::~RelationJs()
{
}

void RelationJs::Init(Handle<Object> target)
{
  Isolate* current = target->GetIsolate();
  HandleScope scope(current);
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(current, New);
  tpl->SetClassName(String::NewFromUtf8(current, Relation::className().data()));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  // Prototype
  ElementJs::_addBaseFunctions(tpl);
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(current, "replaceElement"),
      FunctionTemplate::New(current, replaceElement));
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(current, "getType"),
      FunctionTemplate::New(current, getType));

  _constructor.Reset(current, tpl->GetFunction());
  target->Set(String::NewFromUtf8(current, "Relation"), ToLocal(&_constructor));
}

Handle<Object> RelationJs::New(ConstRelationPtr relation)
{
  Isolate* current = v8::Isolate::GetCurrent();
  EscapableHandleScope scope(current);

  Handle<Object> result = ToLocal(&_constructor)->NewInstance();
  RelationJs* from = ObjectWrap::Unwrap<RelationJs>(result);
  from->_setRelation(relation);

  return scope.Escape(result);
}

Handle<Object> RelationJs::New(RelationPtr relation)
{
  Isolate* current = v8::Isolate::GetCurrent();
  EscapableHandleScope scope(current);

  Handle<Object> result = ToLocal(&_constructor)->NewInstance();
  RelationJs* from = ObjectWrap::Unwrap<RelationJs>(result);
  from->_setRelation(relation);

  return scope.Escape(result);
}

void RelationJs::New(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  RelationJs* obj = new RelationJs();
  //  node::ObjectWrap::Wrap takes ownership of the pointer in a v8::Persistent<v8::Object>
  obj->Wrap(args.This());

  args.GetReturnValue().Set(args.This());
}

void RelationJs::replaceElement(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  try
  {
    RelationJs* obj = ObjectWrap::Unwrap<RelationJs>(args.This());

    ConstElementPtr e1 = toCpp<ConstElementPtr>(args[0]->ToObject());
    ConstElementPtr e2 = toCpp<ConstElementPtr>(args[1]->ToObject());

    obj->getRelation()->replaceElement(e1, e2);

    args.GetReturnValue().SetUndefined();
  }
  catch (const HootException& e)
  {
    args.GetReturnValue().Set(current->ThrowException(HootExceptionJs::create(e)));
  }
}

void RelationJs::getType(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  ConstRelationPtr relation = ObjectWrap::Unwrap<RelationJs>(args.This())->getConstRelation();

  args.GetReturnValue().Set(String::NewFromUtf8(current, relation->getType().toUtf8().data()));
}

}

