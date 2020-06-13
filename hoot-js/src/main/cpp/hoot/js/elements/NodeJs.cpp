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
#include "NodeJs.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Settings.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/util/PopulateConsumersJs.h>
#include <hoot/js/util/StringUtilsJs.h>

// Qt
#include <QStringList>

using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(NodeJs)

Persistent<Function> NodeJs::_constructor;

void NodeJs::getX(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  ConstNodePtr n = ObjectWrap::Unwrap<NodeJs>(args.This())->getConstNode();

  args.GetReturnValue().Set(Number::New(current, n->getX()));
}

void NodeJs::getY(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  ConstNodePtr n = ObjectWrap::Unwrap<NodeJs>(args.This())->getConstNode();

  args.GetReturnValue().Set(Number::New(current, n->getY()));
}

void NodeJs::Init(Handle<Object> target)
{
  Isolate* current = target->GetIsolate();
  HandleScope scope(current);
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(current, New);
  tpl->SetClassName(String::NewFromUtf8(current, Node::className().data()));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  // Prototype
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(current, "getX"),
      FunctionTemplate::New(current, getX));
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(current, "getY"),
      FunctionTemplate::New(current, getY));
  ElementJs::_addBaseFunctions(tpl);

  _constructor.Reset(current, tpl->GetFunction());
  target->Set(String::NewFromUtf8(current, "Node"), ToLocal(&_constructor));
}

Handle<Object> NodeJs::New(ConstNodePtr node)
{
  Isolate* current = v8::Isolate::GetCurrent();
  EscapableHandleScope scope(current);

  Handle<Object> result = ToLocal(&_constructor)->NewInstance();
  NodeJs* from = ObjectWrap::Unwrap<NodeJs>(result);
  from->_setNode(node);

  return scope.Escape(result);
}

Handle<Object> NodeJs::New(NodePtr node)
{
  Isolate* current = v8::Isolate::GetCurrent();
  EscapableHandleScope scope(current);

  Handle<Object> result = ToLocal(&_constructor)->NewInstance();
  NodeJs* from = ObjectWrap::Unwrap<NodeJs>(result);
  from->_setNode(node);

  return scope.Escape(result);
}

void NodeJs::New(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  NodeJs* obj = new NodeJs();
  //  node::ObjectWrap::Wrap takes ownership of the pointer in a v8::Persistent<v8::Object>
  obj->Wrap(args.This());

  args.GetReturnValue().Set(args.This());
}

}

