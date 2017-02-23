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
#include "LogJs.h"

// hoot
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Settings.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/util/DataConvertJs.h>
#include <hoot/js/util/HootExceptionJs.h>
#include <hoot/js/util/StreamUtilsJs.h>
#include <hoot/js/util/StringUtilsJs.h>

namespace hoot
{

HOOT_JS_REGISTER(LogJs)

QHash<QString, int> LogJs::_logs;

LogJs::LogJs()
{
}

LogJs::~LogJs() {}

void LogJs::Init(Handle<Object> exports)
{
  Handle<Object> log = Object::New();
  exports->Set(String::NewSymbol("Log"), log);
  log->Set(String::NewSymbol("setLogLevel"), FunctionTemplate::New(setLogLevel)->GetFunction());
  log->Set(String::NewSymbol("init"), FunctionTemplate::New(init)->GetFunction());

  exports->Set(String::NewSymbol("log"), FunctionTemplate::New(logInfo)->GetFunction());
  exports->Set(String::NewSymbol("trace"), FunctionTemplate::New(logTrace)->GetFunction());
  exports->Set(String::NewSymbol("debug"), FunctionTemplate::New(logDebug)->GetFunction());
  exports->Set(String::NewSymbol("logTrace"), FunctionTemplate::New(logTrace)->GetFunction());
  exports->Set(String::NewSymbol("logDebug"), FunctionTemplate::New(logDebug)->GetFunction());
  exports->Set(String::NewSymbol("logInfo"), FunctionTemplate::New(logInfo)->GetFunction());
  exports->Set(String::NewSymbol("warn"), FunctionTemplate::New(logWarn)->GetFunction());
  exports->Set(String::NewSymbol("logWarn"), FunctionTemplate::New(logWarn)->GetFunction());
  exports->Set(String::NewSymbol("logError"), FunctionTemplate::New(logError)->GetFunction());
  exports->Set(String::NewSymbol("logFatal"), FunctionTemplate::New(logFatal)->GetFunction());
}

int LogJs::getLogCount(QString log)
{
  int result;

  if (_logs.contains(log) == false)
  {
    _logs[log] = 1;
    result = 1;
  }
  else
  {
    result = _logs[log] + 1;
    _logs[log] = result;
  }

  return result;
}

Handle<Value> LogJs::log(const Arguments& args, Log::WarningLevel level) {
  HandleScope scope;
  Context::Scope context_scope(Context::GetCurrent());

  if (level >= Log::getInstance().getLevel())
  {
    Local<StackTrace> stack = StackTrace::CurrentStackTrace(1);
    Local<StackFrame> frame = stack->GetFrame(0);
    int lineNumber = -1;
    QString script("<unknown>");
    QString functionName("<unknown>");

    if (stack->GetFrameCount() >= 1)
    {
      lineNumber = frame->GetLineNumber();
      script = toString(frame->GetScriptName());
      functionName = toString(frame->GetFunctionName());
    }

    std::stringstream rMessage;
    for (int i = 0; i < args.Length(); i++)
    {
      if (i != 0)
      {
        rMessage << " ";
      }
      rMessage << args[i];
    }

    QString message = QString::fromUtf8(rMessage.str().data());

    int logLimit = ConfigOptions().getLogWarnMessageLimit();
    int messageCount = getLogCount(message);

    if (messageCount == logLimit)
    {
      message = QString("Received %1 of the same message. Silencing: ").arg(messageCount) + message;
    }

    if (messageCount <= logLimit)
    {
      Log::getInstance().log(level, message, script, functionName, lineNumber);
    }
  }

  return scope.Close(Undefined());
}

Handle<Value> LogJs::logTrace(const Arguments& args)
{
  HandleScope scope;
//  Context::Scope context_scope(Context::GetCurrent());
  log(args, Log::Trace);
  return scope.Close(Undefined());
}

Handle<Value> LogJs::logDebug(const Arguments& args)
{
  HandleScope scope;
//  Context::Scope context_scope(Context::GetCurrent());
  log(args, Log::Debug);
  return scope.Close(Undefined());
}

Handle<Value> LogJs::logInfo(const Arguments& args)
{
  HandleScope scope;
//  Context::Scope context_scope(Context::GetCurrent());
  log(args, Log::Info);
  return scope.Close(Undefined());
}

Handle<Value> LogJs::logWarn(const Arguments& args)
{
  HandleScope scope;
//  Context::Scope context_scope(Context::GetCurrent());
  log(args, Log::Warn);
  return scope.Close(Undefined());
}

Handle<Value> LogJs::logError(const Arguments& args)
{
  HandleScope scope;
//  Context::Scope context_scope(Context::GetCurrent());
  log(args, Log::Error);
  return scope.Close(Undefined());
}

Handle<Value> LogJs::logFatal(const Arguments& args)
{
  HandleScope scope;
//  Context::Scope context_scope(Context::GetCurrent());
  log(args, Log::Fatal);
  return scope.Close(Undefined());
}

Handle<Value> LogJs::init(const Arguments& /*args*/)
{
  HandleScope scope;
  Log::getInstance().init();
  return scope.Close(Undefined());
}

Handle<Value> LogJs::setLogLevel(const Arguments& args)
{
  HandleScope scope;
  Context::Scope context_scope(Context::GetCurrent());

  try
  {
    QString logLevel = toCpp<QString>(args[0]);
    Log::getInstance().setLevel(Log::getLevelFromString(logLevel));
  }
  catch (const HootException& e)
  {
    return v8::ThrowException(HootExceptionJs::create(e));
  }

  return scope.Close(Undefined());
}

}
