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
#include "LogJs.h"

// hoot
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Settings.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/io/DataConvertJs.h>
#include <hoot/js/io/StreamUtilsJs.h>
#include <hoot/js/util/HootExceptionJs.h>
#include <hoot/js/util/StringUtilsJs.h>

//TEST
#include <thread>
//ENDTEST

using namespace std;
using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(LogJs)

QHash<QString, int> LogJs::_logs;

void LogJs::Init(Local<Object> exports)
{
  Isolate* current = exports->GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();
  Local<Object> log = Object::New(current);
  exports->Set(context, toV8("Log"), log);
  log->Set(context, toV8("setLogLevel"), FunctionTemplate::New(current, setLogLevel)->GetFunction(context).ToLocalChecked());
  log->Set(context, toV8("init"), FunctionTemplate::New(current, init)->GetFunction(context).ToLocalChecked());

  exports->Set(context, toV8("log"), FunctionTemplate::New(current, logInfo)->GetFunction(context).ToLocalChecked());
  exports->Set(context, toV8("trace"), FunctionTemplate::New(current, logTrace)->GetFunction(context).ToLocalChecked());
  exports->Set(context, toV8("debug"), FunctionTemplate::New(current, logDebug)->GetFunction(context).ToLocalChecked());
  exports->Set(context, toV8("logTrace"), FunctionTemplate::New(current, logTrace)->GetFunction(context).ToLocalChecked());
  exports->Set(context, toV8("logDebug"), FunctionTemplate::New(current, logDebug)->GetFunction(context).ToLocalChecked());
  exports->Set(context, toV8("logInfo"), FunctionTemplate::New(current, logInfo)->GetFunction(context).ToLocalChecked());
  exports->Set(context, toV8("logStatus"), FunctionTemplate::New(current, logStatus)->GetFunction(context).ToLocalChecked());
  exports->Set(context, toV8("status"), FunctionTemplate::New(current, logStatus)->GetFunction(context).ToLocalChecked());
  exports->Set(context, toV8("warn"), FunctionTemplate::New(current, logWarn)->GetFunction(context).ToLocalChecked());
  exports->Set(context, toV8("logWarn"), FunctionTemplate::New(current, logWarn)->GetFunction(context).ToLocalChecked());
  exports->Set(context, toV8("logError"), FunctionTemplate::New(current, logError)->GetFunction(context).ToLocalChecked());
  exports->Set(context, toV8("logFatal"), FunctionTemplate::New(current, logFatal)->GetFunction(context).ToLocalChecked());
}

int LogJs::getLogCount(QString log)
{
  if (_logs.contains(log) == false)
    _logs[log] = 1;
  else
    _logs[log] += 1;

  return _logs[log];
}

void LogJs::log(const FunctionCallbackInfo<Value>& args, Log::WarningLevel level)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  Context::Scope context_scope(current->GetCurrentContext());

  if (level >= Log::getInstance().getLevel())
  {
    Local<StackTrace> stack = StackTrace::CurrentStackTrace(current, 1);
    Local<StackFrame> frame = stack->GetFrame(current, 0);
    int lineNumber = -1;
    QString script("<unknown>");
    QString functionName("<unknown>");

    if (stack->GetFrameCount() >= 1)
    {
      lineNumber = frame->GetLineNumber();
      Local<String> script_name = frame->GetScriptNameOrSourceURL();
      if (!script_name.IsEmpty())
        script = toString(script_name).replace("\"", "");

      Local<String> function_name = frame->GetFunctionName();
      if (!function_name.IsEmpty())
        functionName = toString(function_name);
    }

    std::stringstream rMessage;
    for (int i = 0; i < args.Length(); i++)
    {
      if (i != 0)
        rMessage << " ";
      rMessage << args[i];
    }

    QString message = QString::fromUtf8(rMessage.str().data());
    bool logMessage = true;

    if (Log::getInstance().getLevel() == Log::Warn)
    {
      const int logLimit = ConfigOptions().getLogWarnMessageLimit();
      const int messageCount = getLogCount(message);

      if (messageCount == logLimit)
        message = QString("Received %1 of the same message. Silencing: ").arg(messageCount) + message;

      if (messageCount >= logLimit)
        logMessage = false;
    }

    if (logMessage)
    {
      //  Pass the script name in twice since the function in JS doesn't work with filtering
      Log::getInstance().log(level, message, script, script, lineNumber);
    }
  }

  args.GetReturnValue().SetUndefined();
}

void LogJs::logTrace(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  log(args, Log::Trace);
  args.GetReturnValue().SetUndefined();
}

void LogJs::logDebug(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  log(args, Log::Debug);
  args.GetReturnValue().SetUndefined();
}

void LogJs::logInfo(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  log(args, Log::Info);
  args.GetReturnValue().SetUndefined();
}

void LogJs::logStatus(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  log(args, Log::Status);
  args.GetReturnValue().SetUndefined();
}

void LogJs::logWarn(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  log(args, Log::Warn);
  args.GetReturnValue().SetUndefined();
}

void LogJs::logError(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  log(args, Log::Error);
  args.GetReturnValue().SetUndefined();
}

void LogJs::logFatal(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  log(args, Log::Fatal);
  args.GetReturnValue().SetUndefined();
}

void LogJs::init(const FunctionCallbackInfo<Value>& args)
{
  HandleScope scope(args.GetIsolate());
  Log::getInstance().init();
  args.GetReturnValue().SetUndefined();
}

void LogJs::setLogLevel(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  Context::Scope context_scope(current->GetCurrentContext());

  try
  {
    QString logLevel = toCpp<QString>(args[0]);
    Log::getInstance().setLevel(Log::levelFromString(logLevel));

    args.GetReturnValue().SetUndefined();
  }
  catch (const HootException& e)
  {
    args.GetReturnValue().Set(current->ThrowException(HootExceptionJs::create(e)));
  }
}

}
