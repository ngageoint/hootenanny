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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#define BUILDING_NODE_EXTENSION
// #include <nodejs/node.h>
#include <hoot/js/SystemNodeJs.h>

#include "OsmMapJs.h"

// hoot
#include <hoot/js/elements/ElementIdJs.h>
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/js/util/HootExceptionJs.h>
#include <hoot/js/util/PopulateConsumersJs.h>
#include <hoot/js/util/StreamUtilsJs.h>
#include <hoot/js/visitors/ElementVisitorJs.h>
#include <hoot/js/visitors/JsFunctionVisitor.h>
#include <hoot/core/ops/RemoveElementOp.h>
#include "JsRegistrar.h"

using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(OsmMapJs)

Persistent<Function> OsmMapJs::_constructor;

OsmMapJs::OsmMapJs()
{
  _setMap(shared_ptr<OsmMap>(new OsmMap()));
}

OsmMapJs::OsmMapJs(OsmMapPtr map)
{
  _setMap(map);
}

OsmMapJs::~OsmMapJs()
{
  while (!v8::V8::IdleNotification());
}

void OsmMapJs::Init(Handle<Object> target) {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol("OsmMap"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  // Prototype
  tpl->PrototypeTemplate()->Set(String::NewSymbol("clone"),
      FunctionTemplate::New(clone)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("getElement"),
      FunctionTemplate::New(getElement)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("getElementCount"),
      FunctionTemplate::New(getElementCount)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("getParents"),
      FunctionTemplate::New(getParents)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("removeElement"),
      FunctionTemplate::New(removeElement)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("visit"),
      FunctionTemplate::New(visit)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("setIdGenerator"),
      FunctionTemplate::New(setIdGenerator)->GetFunction());
  tpl->PrototypeTemplate()->Set(PopulateConsumersJs::baseClass(),
                                String::New(OsmMap::className().data()));

  _constructor = Persistent<Function>::New(tpl->GetFunction());
  target->Set(String::NewSymbol("OsmMap"), _constructor);
}

Handle<Object> OsmMapJs::create(ConstOsmMapPtr map)
{
  HandleScope scope;

  Handle<Object> result = _constructor->NewInstance();
  OsmMapJs* from = ObjectWrap::Unwrap<OsmMapJs>(result);
  from->_setMap(map);

  return scope.Close(result);
}

Handle<Value> OsmMapJs::setIdGenerator(const Arguments& args)
{
  HandleScope scope;

  OsmMapJs* obj = ObjectWrap::Unwrap<OsmMapJs>(args.This());

  shared_ptr<IdGenerator> idGen =  toCpp<shared_ptr<IdGenerator> >(args[0]);

  if (obj->getMap()) {
    obj->getMap()->setIdGenerator(idGen);
  }

  return scope.Close(Undefined());
}

Handle<Object> OsmMapJs::create(OsmMapPtr map)
{
  HandleScope scope;

  Handle<Object> result = _constructor->NewInstance();
  OsmMapJs* from = ObjectWrap::Unwrap<OsmMapJs>(result);
  from->_setMap(map);

  return scope.Close(result);
}

Handle<Value> OsmMapJs::New(const Arguments& args)
{
  HandleScope scope;

  OsmMapJs* obj = new OsmMapJs();
  obj->Wrap(args.This());

  return args.This();
}

Handle<Value> OsmMapJs::clone(const Arguments& args)
{
  HandleScope scope;

  OsmMapJs* from = ObjectWrap::Unwrap<OsmMapJs>(args.This());

  OsmMapPtr newMap(new OsmMap(from->getMap()));

  const unsigned argc = 1;
  Handle<Value> argv[argc] = { args[0] };
  Local<Object> result = _constructor->NewInstance(argc, argv);
  OsmMapJs* obj = ObjectWrap::Unwrap<OsmMapJs>(result);
  obj->_map = newMap;

  return scope.Close(result);
}

OsmMapPtr& OsmMapJs::getMap()
{
  if (_map.get() == 0 && _constMap.get())
  {
    throw IllegalArgumentException("This map is const and may not be modified.");
  }
  assert(_map.get());
  return _map;
}

Handle<Value> OsmMapJs::getElement(const Arguments& args)
{
  HandleScope scope;

  try
  {
    OsmMapJs* obj = ObjectWrap::Unwrap<OsmMapJs>(args.This());

    LOG_VAR(args[0]->ToObject());
    ElementId eid = toCpp<ElementId>(args[0]);

    if (obj->isConst())
    {
      return scope.Close(toV8(obj->getConstMap()->getElement(eid)));
    }
    else
    {
      return scope.Close(toV8(obj->getMap()->getElement(eid)));
    }
  }
  catch (const HootException& e)
  {
    return v8::ThrowException(HootExceptionJs::create(e));
  }
}

Handle<Value> OsmMapJs::getElementCount(const Arguments& args) {
  HandleScope scope;

  OsmMapJs* obj = ObjectWrap::Unwrap<OsmMapJs>(args.This());

  return scope.Close(Number::New(obj->getConstMap()->getElementCount()));
}

Handle<Value> OsmMapJs::getParents(const Arguments& args) {
  HandleScope scope;

  try
  {
    OsmMapJs* obj = ObjectWrap::Unwrap<OsmMapJs>(args.This());

    ElementId eid = toCpp<ElementId>(args[0]->ToObject());

    return scope.Close(toV8(obj->getConstMap()->getParents(eid)));
  }
  catch (const HootException& e)
  {
    return v8::ThrowException(HootExceptionJs::create(e));
  }
}

Handle<Value> OsmMapJs::removeElement(const Arguments& args) {
  HandleScope scope;

  OsmMapJs* obj = ObjectWrap::Unwrap<OsmMapJs>(args.This());

  ElementId eid = toCpp<ElementId>(args[0]);

  RemoveElementOp::removeElement(obj->getMap(), eid);

  return scope.Close(Undefined());
}

Handle<Value> OsmMapJs::visit(const Arguments& args)
{
  HandleScope scope;

  try
  {
    OsmMapJs* map = ObjectWrap::Unwrap<OsmMapJs>(args.This());

    if (args[0]->IsFunction())
    {
      Persistent<Function> func = Persistent<Function>::New(Handle<Function>::Cast(args[0]));

      JsFunctionVisitor v;
      v.addFunction(func);

      map->getMap()->visitRw(v);
    }
    else
    {
      shared_ptr<ElementVisitor> v =
          ObjectWrap::Unwrap<ElementVisitorJs>(args[0]->ToObject())->getVisitor();

      map->getMap()->visitRw(*v);
    }
  }
  catch (const HootException& err)
  {
    LOG_VAR(err.getWhat());
    return v8::ThrowException(HootExceptionJs::create(err));
  }

  return scope.Close(Undefined());
}

}
