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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "HootExceptionJs.h"

#include <hoot/core/util/Factory.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/io/DataConvertJs.h>
#include <hoot/js/util/PopulateConsumersJs.h>
#include <hoot/js/util/StringUtilsJs.h>

using namespace std;
using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(HootExceptionJs)

Persistent<Function> HootExceptionJs::_constructor;

HootExceptionJs::HootExceptionJs()
{
}

Handle<Object> HootExceptionJs::create(const std::shared_ptr<HootException>& e)
{
  Isolate* current = v8::Isolate::GetCurrent();
  EscapableHandleScope scope(current);

  Handle<Object> result = ToLocal(&_constructor)->NewInstance();
  HootExceptionJs* from = ObjectWrap::Unwrap<HootExceptionJs>(result);
  from->_e = e;

  return scope.Escape(result);
}

void HootExceptionJs::Init(Handle<Object> target)
{
  Isolate* current = target->GetIsolate();
  HandleScope scope(current);
  vector<string> opNames =
    Factory::getInstance().getObjectNamesByBase(HootException::className());

  for (size_t i = 0; i < opNames.size(); i++)
  {
    QString noNamespace = QString::fromStdString(opNames[i]).replace("hoot::", "");

    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(current, New);
    tpl->SetClassName(Handle<String>::Cast(toV8(opNames[i])));
    tpl->InstanceTemplate()->SetInternalFieldCount(2);
    // Prototype
    tpl->PrototypeTemplate()->Set(String::NewFromUtf8(current, "toString"),
        FunctionTemplate::New(current, toString));
    tpl->PrototypeTemplate()->Set(String::NewFromUtf8(current, "toJSON"),
        FunctionTemplate::New(current, toJSON));
    tpl->PrototypeTemplate()->Set(PopulateConsumersJs::baseClass(),
                                  toV8(HootException::className()));

    _constructor.Reset(current, tpl->GetFunction());
    target->Set(toV8(noNamespace), ToLocal(&_constructor));
  }
}

bool HootExceptionJs::isHootException(Handle<Value> v)
{
  bool result = false;

  if (v->IsObject())
  {
    Handle<Object> obj = Handle<Object>::Cast(v);
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

void HootExceptionJs::New(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  HootExceptionJs* obj = new HootExceptionJs();
  //  node::ObjectWrap::Wrap takes ownership of the pointer in a v8::Persistent<v8::Object>
  obj->Wrap(args.This());

  args.GetReturnValue().Set(args.This());
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
    std::shared_ptr<HootException> e = toCpp<std::shared_ptr<HootException>>(exception);
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
    // if this is a generic error (e.g. throw Error("blah");) then just report the string.
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

void HootExceptionJs::toJSON(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  std::shared_ptr<HootException> e = ObjectWrap::Unwrap<HootExceptionJs>(args.This())->getException();

  QVariantMap m;
  m["message"] = e->getWhat();
  m["classname"] = QString::fromStdString(e->getClassName());

  args.GetReturnValue().Set(toV8(m));
}

void HootExceptionJs::toString(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  std::shared_ptr<HootException> e = ObjectWrap::Unwrap<HootExceptionJs>(args.This())->getException();

  args.GetReturnValue().Set(toV8(e->getWhat()));
}

}
