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
#include "ElementJs.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Settings.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/elements/ElementIdJs.h>
#include <hoot/js/elements/NodeJs.h>
#include <hoot/js/elements/RelationJs.h>
#include <hoot/js/elements/TagsJs.h>
#include <hoot/js/elements/WayJs.h>
#include <hoot/js/util/PopulateConsumersJs.h>
#include <hoot/js/util/StringUtilsJs.h>
#include <hoot/js/util/HootExceptionJs.h>

// Qt
#include <QStringList>

using namespace v8;

namespace hoot
{

void ElementJs::_addBaseFunctions(Local<FunctionTemplate> tpl)
{
  Isolate* current = v8::Isolate::GetCurrent();
  HandleScope scope(current);
  tpl->PrototypeTemplate()->Set(PopulateConsumersJs::baseClass(),
      String::NewFromUtf8(current, Element::className().toStdString().data()));
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(current, "getCircularError"),
      FunctionTemplate::New(current, getCircularError));
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(current, "getElementId"),
      FunctionTemplate::New(current, getElementId));
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(current, "getId"),
      FunctionTemplate::New(current, getId));
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(current, "getStatusInput"),
      FunctionTemplate::New(current, getStatusString));
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(current, "getStatusString"),
      FunctionTemplate::New(current, getStatusString));
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(current, "getTags"),
      FunctionTemplate::New(current, getTags));
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(current, "setStatusString"),
      FunctionTemplate::New(current, setStatusString));
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(current, "setTags"),
      FunctionTemplate::New(current, setTags));
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(current, "setTag"),
      FunctionTemplate::New(current, setTag));
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(current, "toJSON"),
      FunctionTemplate::New(current, toString));
  tpl->PrototypeTemplate()->Set(String::NewFromUtf8(current, "toString"),
      FunctionTemplate::New(current, toString));
}

void ElementJs::getCircularError(const FunctionCallbackInfo<Value>& args)
{
  HandleScope scope(args.GetIsolate());

  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args.This())->getConstElement();

  args.GetReturnValue().Set(toV8(e->getCircularError()));
}

void ElementJs::getElementId(const FunctionCallbackInfo<Value>& args)
{
  HandleScope scope(args.GetIsolate());

  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args.This())->getConstElement();

  args.GetReturnValue().Set(ElementIdJs::New(e->getElementId()));
}

void ElementJs::getId(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args.This())->getConstElement();

  args.GetReturnValue().Set(Integer::New(current, e->getId()));
}

void ElementJs::getStatusInput(const FunctionCallbackInfo<Value>& args)
{
  HandleScope scope(args.GetIsolate());

  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args.This())->getConstElement();

  args.GetReturnValue().Set(toV8(e->getStatus().getInput()));
}

void ElementJs::getStatusString(const FunctionCallbackInfo<Value>& args)
{
  HandleScope scope(args.GetIsolate());

  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args.This())->getConstElement();

  args.GetReturnValue().Set(toV8(e->getStatusString()));
}

void ElementJs::getTags(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args.This())->getConstElement();

  args.GetReturnValue().Set(TagsJs::New(e->getTags()));
}

Local<Object> ElementJs::New(ConstElementPtr e)
{
  EscapableHandleScope scope(v8::Isolate::GetCurrent());

  Local<Object> result;

  switch (e->getElementType().getEnum())
  {
  case ElementType::Node:
    {
      ConstNodePtr n = std::dynamic_pointer_cast<const Node>(e);
      result = NodeJs::New(n);
      break;
    }
  case ElementType::Way:
    {
      ConstWayPtr w = std::dynamic_pointer_cast<const Way>(e);
      result = WayJs::New(w);
      break;
    }
  case ElementType::Relation:
    {
      ConstRelationPtr r = std::dynamic_pointer_cast<const Relation>(e);
      result = RelationJs::New(r);
      break;
    }
  default:
    throw IllegalArgumentException("Unexpected element type.");
  }

  return scope.Escape(result);
}

Local<Object> ElementJs::New(ElementPtr e)
{
  EscapableHandleScope scope(v8::Isolate::GetCurrent());

  Local<Object> result;

  switch (e->getElementType().getEnum())
  {
  case ElementType::Node:
    {
      NodePtr n = std::dynamic_pointer_cast<Node>(e);
      result = NodeJs::New(n);
      break;
    }
  case ElementType::Way:
    {
      WayPtr w = std::dynamic_pointer_cast<Way>(e);
      result = WayJs::New(w);
      break;
    }
  case ElementType::Relation:
    {
      RelationPtr r = std::dynamic_pointer_cast<Relation>(e);
      result = RelationJs::New(r);
      break;
    }
  default:
    throw IllegalArgumentException("Unexpected element type.");
  }

  return scope.Escape(result);
}

void ElementJs::setStatusString(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  try
  {
    QString statusStr = toCpp<QString>(args[0]);

    Status s = Status::fromString(statusStr);

    ElementPtr e = ObjectWrap::Unwrap<ElementJs>(args.This())->getElement();

    e->setStatus(s);

    args.GetReturnValue().SetUndefined();
  }
  catch ( const HootException& e )
  {
    args.GetReturnValue().Set(current->ThrowException(HootExceptionJs::create(e)));
  }
}

void ElementJs::setTags(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();

  ElementPtr e = ObjectWrap::Unwrap<ElementJs>(args.This())->getElement();

  if (!e)
  {
    args.GetReturnValue().Set(
      current->ThrowException(
        HootExceptionJs::create(IllegalArgumentException("Unable to set tags on a const Element."))));
  }
  else
  {
    const Tags& tags = ObjectWrap::Unwrap<TagsJs>(args[0]->ToObject(context).ToLocalChecked())->getTags();
    e->setTags(tags);
    args.GetReturnValue().SetUndefined();
  }
}

void ElementJs::setTag(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  ElementPtr e = ObjectWrap::Unwrap<ElementJs>(args.This())->getElement();

  if (!e)
  {
    args.GetReturnValue().Set(
      current->ThrowException(
        HootExceptionJs::create(IllegalArgumentException("Unable to set tag on a const Element."))));
  }
  else
  {
    const QString tagKey = toCpp<QString>(args[0]);
    const QString tagVal = toCpp<QString>(args[1]);
    e->setTag(tagKey, tagVal);
    args.GetReturnValue().SetUndefined();
  }
}

void ElementJs::toString(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  ConstElementPtr e = ObjectWrap::Unwrap<ElementJs>(args.This())->getConstElement();

  args.GetReturnValue().Set(String::NewFromUtf8(current, e->toString().toUtf8().data()));
}

}

