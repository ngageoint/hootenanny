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

Local<Object> HootExceptionJs::create(const std::shared_ptr<HootException>& e)
{
  Isolate* current = v8::Isolate::GetCurrent();
  EscapableHandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();

  Local<Object> result = ToLocal(&_constructor)->NewInstance(context).ToLocalChecked();
  HootExceptionJs* from = ObjectWrap::Unwrap<HootExceptionJs>(result);
  from->_e = e;

  return scope.Escape(result);
}

void HootExceptionJs::Init(Local<Object> target)
{
  Isolate* current = target->GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();
  vector<QString> opNames =
    Factory::getInstance().getObjectNamesByBase(HootException::className());

  for (size_t i = 0; i < opNames.size(); i++)
  {
    QString noNamespace = opNames[i].replace("hoot::", "");

    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(current, New);
    tpl->SetClassName(Local<String>::Cast(toV8(opNames[i])));
    tpl->InstanceTemplate()->SetInternalFieldCount(2);
    // Prototype
    tpl->PrototypeTemplate()->Set(current, "toString", FunctionTemplate::New(current, toString));
    tpl->PrototypeTemplate()->Set(PopulateConsumersJs::baseClass(),
                                  toV8(HootException::className()));

    _constructor.Reset(current, tpl->GetFunction(context).ToLocalChecked());
    target->Set(context, toV8(noNamespace), ToLocal(&_constructor));
  }
}

bool HootExceptionJs::isHootException(Local<Value> v)
{
  bool result = false;

  if (v->IsObject())
  {
    Local<Object> obj = Local<Object>::Cast(v);
    HootExceptionJs* e = nullptr;
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

void HootExceptionJs::checkV8Exception(Local<Value> result, TryCatch& tc)
{
  if (result.IsEmpty())
  {
    throwAsHootException(tc);
  }
}

void HootExceptionJs::throwAsHootException(TryCatch& tc)
{
  v8::Isolate* current = v8::Isolate::GetCurrent();
  v8::HandleScope scope(current);
  v8::Local<v8::Context> context = current->GetCurrentContext();
  Local<Value> exception = tc.Exception();

  if (isHootException(exception))
  {
    std::shared_ptr<HootException> e = toCpp<std::shared_ptr<HootException>>(exception);
    HootExceptionThrower::getInstance().rethrowPointer(e);
  }
  else
  {
    Local<Message> msg = tc.Message();
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
      str(exception->ToObject(context).ToLocalChecked()->GetConstructorName()) == "Error")
    {
      throw HootException(str(exception->ToDetailString(context).ToLocalChecked()));
    }
    else if (exception->IsString())
    {
      throw HootException(str(exception));
    }
    else
    {
      //  See ReportException in https://github.com/v8/v8/blob/master/samples/shell.cc
      QString exception_string = (str(tc.Exception()));
      QString filename_string = (str(msg->GetScriptOrigin().ResourceName()));
      int linenum = msg->GetLineNumber(context).FromJust();
      QString sourceline_string = (str(msg->GetSourceLine(context).ToLocalChecked()));
      int start = msg->GetStartColumn(context).FromJust();
      QString start_string(start, ' ');
      int end = msg->GetEndColumn(context).FromJust();
      QString end_string(end - start, '^');
      QString stack_trace_string;
      v8::Local<v8::Value> stack_trace;
      if (tc.StackTrace(context).ToLocal(&stack_trace) &&
          stack_trace->IsString() &&
          stack_trace.As<v8::String>()->Length() > 0)
      {
        stack_trace_string = (str(stack_trace));
      }
      throw HootException(QString("%1:%2: %3\n%4\n%5%6\n%7")
          .arg(filename_string)
          .arg(linenum)
          .arg(exception_string)
          .arg(sourceline_string)
          .arg(start_string)
          .arg(end_string)
          .arg(stack_trace_string));
    }
  }
}

void HootExceptionJs::toString(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);

  std::shared_ptr<HootException> e = ObjectWrap::Unwrap<HootExceptionJs>(args.This())->getException();

  args.GetReturnValue().Set(toV8(e->getWhat()));
}

}
