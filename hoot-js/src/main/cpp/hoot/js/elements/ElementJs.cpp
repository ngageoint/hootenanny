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
#include "ElementJs.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/filters/ElementCriterionConsumer.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Settings.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/elements/NodeJs.h>
#include <hoot/js/elements/RelationJs.h>
#include <hoot/js/elements/TagsJs.h>
#include <hoot/js/elements/WayJs.h>
#include <hoot/js/util/PopulateConsumersJs.h>
#include <hoot/js/util/StringUtilsJs.h>
#include <hoot/js/util/HootExceptionJs.h>

// Qt
#include <QStringList>

// Tgs
#include <tgs/SharedPtr.h>

#include "ElementIdJs.h"

namespace hoot
{

ElementJs::ElementJs()
{
}

ElementJs::~ElementJs()
{
}

void ElementJs::_addBaseFunctions(Local<FunctionTemplate> tpl)
{
  tpl->PrototypeTemplate()->Set(PopulateConsumersJs::baseClass(),
      String::New(Element::className().data()));
  tpl->PrototypeTemplate()->Set(String::NewSymbol("getCircularError"),
      FunctionTemplate::New(getCircularError)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("getElementId"),
      FunctionTemplate::New(getElementId)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("getId"),
      FunctionTemplate::New(getId)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("getStatusString"),
      FunctionTemplate::New(getStatusString)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("getTags"),
      FunctionTemplate::New(getTags)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("setStatusString"),
      FunctionTemplate::New(setStatusString)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("setTags"),
      FunctionTemplate::New(setTags)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("toJSON"),
      FunctionTemplate::New(toString)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("toString"),
      FunctionTemplate::New(toString)->GetFunction());
}

Handle<Value> ElementJs::getCircularError(const Arguments& args) {
  HandleScope scope;

  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args.This())->getConstElement();

  return scope.Close(toV8(e->getCircularError()));
}

Handle<Value> ElementJs::getElementId(const Arguments& args) {
  HandleScope scope;

  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args.This())->getConstElement();

  return scope.Close(ElementIdJs::New(e->getElementId()));
}

Handle<Value> ElementJs::getId(const Arguments& args) {
  HandleScope scope;

  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args.This())->getConstElement();

  return scope.Close(v8::Integer::New(e->getId()));
}

Handle<Value> ElementJs::getStatusString(const Arguments& args)
{
  HandleScope scope;

  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args.This())->getConstElement();

  return scope.Close(toV8(e->getStatusString()));
}

Handle<Value> ElementJs::getTags(const Arguments& args) {
  HandleScope scope;

  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args.This())->getConstElement();

  return scope.Close(TagsJs::New(e->getTags()));
}

Handle<Object> ElementJs::New(ConstElementPtr e)
{
  HandleScope scope;

  Handle<Object> result;

  switch (e->getElementType().getEnum())
  {
  case ElementType::Node:
    {
      ConstNodePtr n = dynamic_pointer_cast<const Node>(e);
      result = NodeJs::New(n);
      break;
    }
  case ElementType::Way:
    {
      ConstWayPtr w = dynamic_pointer_cast<const Way>(e);
      result = WayJs::New(w);
      break;
    }
  case ElementType::Relation:
    {
      ConstRelationPtr r = dynamic_pointer_cast<const Relation>(e);
      result = RelationJs::New(r);
      break;
    }
  default:
    throw IllegalArgumentException("Unexpected element type.");
  }

  return scope.Close(result);
}

Handle<Object> ElementJs::New(ElementPtr e)
{
  HandleScope scope;

  Handle<Object> result;

  switch (e->getElementType().getEnum())
  {
  case ElementType::Node:
    {
      NodePtr n = dynamic_pointer_cast<Node>(e);
      result = NodeJs::New(n);
      break;
    }
  case ElementType::Way:
    {
      WayPtr w = dynamic_pointer_cast<Way>(e);
      result = WayJs::New(w);
      break;
    }
  case ElementType::Relation:
    {
      RelationPtr r = dynamic_pointer_cast<Relation>(e);
      result = RelationJs::New(r);
      break;
    }
  default:
    throw IllegalArgumentException("Unexpected element type.");
  }

  return scope.Close(result);
}

Handle<Value> ElementJs::setStatusString(const Arguments& args)
{
  HandleScope scope;

  try
  {
    QString statusStr = toCpp<QString>(args[0]);

    Status s = Status::fromString(statusStr);

    ElementPtr e = ObjectWrap::Unwrap<ElementJs>(args.This())->getElement();

    e->setStatus(s);
  }
  catch ( const HootException& e )
  {
    return v8::ThrowException(HootExceptionJs::create(e));
  }

  return scope.Close(Undefined());
}

Handle<Value> ElementJs::setTags(const Arguments& args)
{
  HandleScope scope;

  ElementPtr e = ObjectWrap::Unwrap<ElementJs>(args.This())->getElement();

  if (!e)
  {
    return v8::ThrowException(HootExceptionJs::create(IllegalArgumentException(
      "Unable to set tags on a const Element.")));
  }

  Tags& tags = ObjectWrap::Unwrap<TagsJs>(args[0]->ToObject())->getTags();
  e->setTags(tags);

  return scope.Close(Undefined());
}

Handle<Value> ElementJs::toString(const Arguments& args)
{
  HandleScope scope;

  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args.This())->getConstElement();

  return scope.Close(String::New(e->toString().toUtf8().data()));
}

}

