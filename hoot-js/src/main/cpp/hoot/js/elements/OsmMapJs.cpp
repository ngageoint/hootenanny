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
#define BUILDING_NODE_EXTENSION

#include "OsmMapJs.h"

// hoot
#include <hoot/core/ops/RemoveElementByEid.h>
#include <hoot/core/util/Factory.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/SystemNodeJs.h>
#include <hoot/js/elements/ElementIdJs.h>
#include <hoot/js/util/HootExceptionJs.h>
#include <hoot/js/util/PopulateConsumersJs.h>
#include <hoot/js/io/StreamUtilsJs.h>
#include <hoot/js/visitors/ElementVisitorJs.h>
#include <hoot/js/visitors/JsFunctionVisitor.h>

using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(OsmMapJs)

Persistent<Function> OsmMapJs::_constructor;

OsmMapJs::OsmMapJs()
{
  _setMap(std::make_shared<OsmMap>());
}

void OsmMapJs::Init(Local<Object> target)
{
  Isolate* current = target->GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();

  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(current, New);
  tpl->SetClassName(String::NewFromUtf8(current, "OsmMap").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  tpl->PrototypeTemplate()->Set(current, "clone", FunctionTemplate::New(current, clone));
  tpl->PrototypeTemplate()->Set(current, "getElement", FunctionTemplate::New(current, getElement));
  tpl->PrototypeTemplate()->Set(current, "getElementCount", FunctionTemplate::New(current, getElementCount));
  tpl->PrototypeTemplate()->Set(current, "visit", FunctionTemplate::New(current, visit));

  tpl->PrototypeTemplate()->Set(PopulateConsumersJs::baseClass(), toV8(OsmMap::className()));

  _constructor.Reset(current, tpl->GetFunction(context).ToLocalChecked());
  target->Set(context, toV8("OsmMap"), ToLocal(&_constructor));
}

Local<Object> OsmMapJs::create(ConstOsmMapPtr map)
{
  Isolate* current = v8::Isolate::GetCurrent();
  EscapableHandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();

  Local<Object> result = ToLocal(&_constructor)->NewInstance(context).ToLocalChecked();
  OsmMapJs* from = ObjectWrap::Unwrap<OsmMapJs>(result);
  from->_setMap(map);

  return scope.Escape(result);
}

Local<Object> OsmMapJs::create(OsmMapPtr map)
{
  Isolate* current = v8::Isolate::GetCurrent();
  EscapableHandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();

  Local<Object> result = ToLocal(&_constructor)->NewInstance(context).ToLocalChecked();
  OsmMapJs* from = ObjectWrap::Unwrap<OsmMapJs>(result);
  from->_setMap(map);

  return scope.Escape(result);
}

void OsmMapJs::New(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  OsmMapJs* obj = new OsmMapJs();
  obj->Wrap(args.This());

  args.GetReturnValue().Set(args.This());
}

void OsmMapJs::clone(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();

  OsmMapJs* from = ObjectWrap::Unwrap<OsmMapJs>(args.This());

  // The const map always takes precedence over the non-const map. If we have a const map populated
  // on our js object, then the map is treated as const.
  ConstOsmMapPtr newConstMap;
  OsmMapPtr newMap;
  if (from->getConstMap())
  {
    newConstMap = std::make_shared<OsmMap>(from->getConstMap());
  }
  else
  {
    newMap = std::make_shared<OsmMap>(from->getMap());
  }

  const unsigned argc = 1;
  Local<Value> argv[argc] = { args[0] };
  Local<Object> result = ToLocal(&_constructor)->NewInstance(context, argc, argv).ToLocalChecked();
  OsmMapJs* obj = ObjectWrap::Unwrap<OsmMapJs>(result);
  if (newConstMap)
  {
    obj->_constMap = newConstMap;
  }
  else
  {
    obj->_map = newMap;
  }

  args.GetReturnValue().Set(result);
}

OsmMapPtr& OsmMapJs::getMap()
{
  if (_map.get() == nullptr && _constMap.get())
  {
    throw IllegalArgumentException("This map is const and may not be modified.");
  }
  assert(_map.get());
  return _map;
}

void OsmMapJs::getElement(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  try
  {
    OsmMapJs* obj = ObjectWrap::Unwrap<OsmMapJs>(args.This());

    ElementId eid = toCpp<ElementId>(args[0]);
    if (obj->isConst())
    {
      args.GetReturnValue().Set(toV8(obj->getConstMap()->getElement(eid)));
    }
    else
    {
      args.GetReturnValue().Set(toV8(obj->getMap()->getElement(eid)));
    }
  }
  catch (const HootException& e)
  {
    args.GetReturnValue().Set(current->ThrowException(HootExceptionJs::create(e)));
  }
}

void OsmMapJs::getElementCount(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  OsmMapJs* obj = ObjectWrap::Unwrap<OsmMapJs>(args.This());

  args.GetReturnValue().Set(Number::New(current, obj->getConstMap()->getElementCount()));
}

void OsmMapJs::visit(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();

  try
  {
    OsmMapJs* map = ObjectWrap::Unwrap<OsmMapJs>(args.This());

    if (args[0]->IsFunction())
    {
      Local<Function> func(Local<Function>::Cast(args[0]));

      JsFunctionVisitor v;
      v.addFunction(current, func);

      map->getMap()->visitRw(v);
    }
    else
    {
      std::shared_ptr<ElementVisitor> v =
        ObjectWrap::Unwrap<ElementVisitorJs>(args[0]->ToObject(context).ToLocalChecked())->getVisitor();

      map->getMap()->visitRw(*v);
    }
    args.GetReturnValue().SetUndefined();
  }
  catch (const HootException& err)
  {
    LOG_VARE(err.getWhat());
    args.GetReturnValue().Set(current->ThrowException(HootExceptionJs::create(err)));
  }
}

}
