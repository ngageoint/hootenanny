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
#include "NodeJs.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/filters/ElementCriterionConsumer.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Settings.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/util/PopulateConsumersJs.h>
#include <hoot/js/util/StringUtilsJs.h>

// Qt
#include <QStringList>

// Tgs
#include <tgs/SharedPtr.h>

namespace hoot
{

HOOT_JS_REGISTER(NodeJs)

Persistent<Function> NodeJs::_constructor;

NodeJs::NodeJs(ConstNodePtr n) : _constNode(n)
{
}

NodeJs::NodeJs()
{
}

NodeJs::~NodeJs()
{
}

Handle<Value> NodeJs::getX(const Arguments& args) {
  HandleScope scope;

  ConstNodePtr n = ObjectWrap::Unwrap<NodeJs>(args.This())->getConstNode();

  return scope.Close(Number::New(n->getX()));
}

Handle<Value> NodeJs::getY(const Arguments& args) {
  HandleScope scope;

  ConstNodePtr n = ObjectWrap::Unwrap<NodeJs>(args.This())->getConstNode();

  return scope.Close(Number::New(n->getY()));
}

void NodeJs::Init(Handle<Object> target)
{
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol(Node::className().data()));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  // Prototype
  tpl->PrototypeTemplate()->Set(String::NewSymbol("getX"),
      FunctionTemplate::New(getX)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("getY"),
      FunctionTemplate::New(getY)->GetFunction());
  ElementJs::_addBaseFunctions(tpl);

  _constructor = Persistent<Function>::New(tpl->GetFunction());
  target->Set(String::NewSymbol("Node"), _constructor);
}

Handle<Object> NodeJs::New(ConstNodePtr node)
{
  HandleScope scope;

  Handle<Object> result = _constructor->NewInstance();
  NodeJs* from = ObjectWrap::Unwrap<NodeJs>(result);
  from->_setNode(node);

  return scope.Close(result);
}

Handle<Object> NodeJs::New(NodePtr node)
{
  HandleScope scope;

  Handle<Object> result = _constructor->NewInstance();
  NodeJs* from = ObjectWrap::Unwrap<NodeJs>(result);
  from->_setNode(node);

  return scope.Close(result);
}

Handle<Value> NodeJs::New(const Arguments& args)
{
  HandleScope scope;

  NodeJs* obj = new NodeJs();
  obj->Wrap(args.This());

  return args.This();
}

}

