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
#include "HootExceptionJs.h"

#include <hoot/core/Factory.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/util/DataConvertJs.h>
#include <hoot/js/util/PopulateConsumersJs.h>
#include <hoot/js/util/StringUtilsJs.h>

namespace hoot
{

HOOT_JS_REGISTER(HootExceptionJs)

Persistent<Function> HootExceptionJs::_constructor;

HootExceptionJs::HootExceptionJs()
{
}

Handle<Object> HootExceptionJs::create(shared_ptr<HootException> e)
{
  HandleScope scope;

  Handle<Object> result = _constructor->NewInstance();
  HootExceptionJs* from = ObjectWrap::Unwrap<HootExceptionJs>(result);
  from->_e = e;

  return scope.Close(result);
}

void HootExceptionJs::Init(Handle<Object> target)
{
  vector<string> opNames =
    Factory::getInstance().getObjectNamesByBase(HootException::className());

  for (size_t i = 0; i < opNames.size(); i++)
  {
    QString noNamespace = QString::fromStdString(opNames[i]).replace("hoot::", "");

    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
    tpl->SetClassName(Handle<String>::Cast(toV8(opNames[i])));
    tpl->InstanceTemplate()->SetInternalFieldCount(2);
    // Prototype
    tpl->PrototypeTemplate()->Set(String::NewSymbol("toString"),
        FunctionTemplate::New(toString)->GetFunction());
    tpl->PrototypeTemplate()->Set(String::NewSymbol("toJSON"),
        FunctionTemplate::New(toJSON)->GetFunction());
    tpl->PrototypeTemplate()->Set(PopulateConsumersJs::baseClass(),
                                  toV8(HootException::className()));

    _constructor = Persistent<Function>::New(tpl->GetFunction());
    target->Set(toV8(noNamespace), _constructor);
  }
}

bool HootExceptionJs::isHootException(Handle<Value> v)
{
  bool result = false;

  if (v->IsObject())
  {
    v8::Handle<v8::Object> obj = v8::Handle<v8::Object>::Cast(v);
    HootExceptionJs* e = 0;
    if (obj->InternalFieldCount() >= 1)
    {
      e = node::ObjectWrap::Unwrap<HootExceptionJs>(obj);
    }
    if (e)
    {
      result = true;
    }
  }

  return result;
}

v8::Handle<v8::Value> HootExceptionJs::New(const v8::Arguments& args)
{
  HandleScope scope;

  HootExceptionJs* obj = new HootExceptionJs();
  obj->Wrap(args.This());

  return args.This();
}

void HootExceptionJs::checkV8Exception(Handle<Value> result, TryCatch& tc)
{
  if (result.IsEmpty())
  {
    throwAsHootException(tc);
  }
}

void HootExceptionJs::throwAsHootException(TryCatch& tc)
{
  Local<Value> exception = tc.Exception();

  if (isHootException(exception))
  {
    shared_ptr<HootException> e = toCpp< shared_ptr<HootException> >(exception);
    HootExceptionThrower::getInstance().rethrowPointer(e);
  }
  else
  {
    Handle<Message> msg = tc.Message();
    if (msg.IsEmpty())
    {
      if (exception.IsEmpty())
      {
        throw HootException("Unidentified JavaScript Exception");
      }
      else
      {
        throw HootException(toJson(exception));
      }
    }
    // if this is a generic error (e.g. throw new Errro("blah");) then just report the string.
    else if (exception->IsNativeError() &&
      str(exception->ToObject()->GetConstructorName()) == "Error")
    {
      throw HootException(str(exception->ToDetailString()));
    }
    else if (exception->IsString())
    {
      throw HootException(str(exception));
    }
    else
    {
      // See ReportException in http://v8.googlecode.com/svn/trunk/samples/shell.cc
      QString fileName = toCpp<QString>(msg->GetScriptResourceName());
      int lineNumber = msg->GetLineNumber();

      QString sourceLine = toCpp<QString>(msg->GetSourceLine());

      // Put a cool wavy line under the error
      int start = msg->GetStartColumn();
      int end = msg->GetEndColumn();
      QString blank(start,' ');
      QString wave(end - start,'^');

      throw HootException(QString("%1 (%2) \n%3\n%4 \n%5").arg(fileName).arg(lineNumber).arg(sourceLine).arg(blank + wave).arg(str(exception->ToString())));
    }
  }
}

v8::Handle<v8::Value> HootExceptionJs::toJSON(const v8::Arguments& args)
{
  HandleScope scope;

  shared_ptr<HootException> e = ObjectWrap::Unwrap<HootExceptionJs>(args.This())->getException();

  QVariantMap m;
  m["message"] = e->getWhat();
  m["classname"] = QString::fromStdString(e->getClassName());

  return scope.Close(toV8(m));
}

v8::Handle<v8::Value> HootExceptionJs::toString(const v8::Arguments& args)
{
  HandleScope scope;

  shared_ptr<HootException> e = ObjectWrap::Unwrap<HootExceptionJs>(args.This())->getException();

  return scope.Close(toV8(e->getWhat()));
}

}
