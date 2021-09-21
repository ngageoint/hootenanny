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

#include "Log.h"

// hoot
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/LogColor.h>
#include <hoot/core/util/StringUtils.h>

// GDAL
#include <cpl_error.h>

// Standard
#include <iomanip>
#include <iostream>
using namespace std;

// Qt
#include <QDateTime>

namespace hoot
{

QString Log::LOG_WARN_LIMIT_REACHED_MESSAGE = "Reached the maximum number of allowed warning messages for this class set by the setting log.warn.message.limit.  Silencing additional warning messages for this class...";
int Log::_warnMessageLimit = 0;

Log& Log::getInstance()
{
  //  Local static singleton instance
  static Log instance;
  return instance;
}

void myLoggerFunction(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
  Log::WarningLevel l = Log::Fatal;
  switch(type)
  {
  case QtDebugMsg:      l = Log::Debug;   break;
  case QtWarningMsg:    l = Log::Warn;    break;
  case QtCriticalMsg:   l = Log::Error;   break;
  case QtFatalMsg:      l = Log::Fatal;   break;
  case QtInfoMsg:       l = Log::Info;    break;
  default: break;
  }
  Log::getInstance().log(l, msg, context.file, context.function, context.line);
}

static void cplErrorHandler(CPLErr eErrClass, int err_no, const char *msg)
{
  Log::WarningLevel l = Log::Fatal;
  switch (eErrClass)
  {
  case CE_None:         l = Log::None;    break;
  case CE_Debug:        l = Log::Debug;   break;
  case CE_Warning:      l = Log::Warn;    break;
  case CE_Failure:      l = Log::Error;   break;
  case CE_Fatal:        l = Log::Fatal;   break;
  default: break;
  }
  stringstream ss;
  ss << "CPL Error #: " << err_no << " " << msg;
  Log::getInstance().log(l, ss.str());
}

Log::Log() :
_level(Log::Info),
_decorateLogs(false)
{
  qInstallMessageHandler(myLoggerFunction);
  CPLSetErrorHandler(cplErrorHandler);
}

void Log::init() const
{
}

void Log::log(WarningLevel level, const string& str)
{
  //  Mimic log4cxx with unknown file name (...?), function name ("") and line number (-1)
  log(level, str, "...?", "", -1);
}

void Log::log(WarningLevel level, const string& str, const string& filename,
              const string& prettyFunction, int lineNumber)
{
  if (level >= _level && _passesFilter(prettyFunction))
  {
    QDateTime dt = QDateTime::currentDateTime();

    // takes the form: "09:34:21.635 WARN  <filename>(<lineNumber>) <str>"
    cout << beginDecoration(level)
         << dt.toString("hh:mm:ss.zzz") << " " << setw(6) << left << Log::levelToString(level)
         << " " << ellipsisStr(filename) << "(" << setw(4) << right << lineNumber << ")" << " "
         << str
         << endDecoration()
         << endl;
  }
}

void Log::progress(WarningLevel level, const string& str, const string& filename,
                   const string& prettyFunction, int lineNumber)
{
  if (level >= _level && _passesFilter(prettyFunction))
  {
    QDateTime dt = QDateTime::currentDateTime();

    // takes the form: "09:34:21.635 WARN  <filename>(<lineNumber>) <str>"
    cout << beginDecoration(level)
         << dt.toString("hh:mm:ss.zzz") << " " << setw(6) << left << Log::levelToString(level)
         << " " << ellipsisStr(filename) << "(" << setw(4) << right << lineNumber << ")" << " "
         << str << "        \r"
         << endDecoration()
         << flush;
  }
}

int Log::getWarnMessageLimit()
{
  if (_warnMessageLimit == 0)
    _warnMessageLimit = ConfigOptions().getLogWarnMessageLimit();
  return _warnMessageLimit;
}

Log::WarningLevel Log::levelFromString(QString& l)
{
  l = l.toLower();
  if (l == "none")
    return None;
  else if (l == "trace")
    return Trace;
  else if (l == "debug")
    return Debug;
  else if (l == "info")
    return Info;
  else if (l == "status")
    return Status;
  else if (l == "warn")
    return Warn;
  else if (l == "error")
    return Error;
  else if (l == "fatal")
    return Fatal;
  else
    throw IllegalArgumentException("Unexpected log level string: " + l);
}

QString Log::levelToString(WarningLevel l)
{
  switch(l)
  {
  case None:      return "NONE";
  case Trace:     return "TRACE";
  case Debug:     return "DEBUG";
  case Info:      return "INFO";
  case Status:    return "STATUS";
  case Warn:      return "WARN";
  case Error:     return "ERROR";
  case Fatal:     return "FATAL";
  default:        return "UNK";
  }
}

QString Log::getLevelAsString() const
{
  return levelToString(_level);
}

void Log::log(
  WarningLevel level, const QString& str, const QString& filename, const QString& prettyFunction,
  int lineNumber)
{
  log(
    level, string(str.toUtf8().data()), string(filename.toUtf8().data()),
    string(prettyFunction.toUtf8().data()), lineNumber);
}

string Log::ellipsisStr(const string& str, uint count)
{
  return (str.length() <= count) ? str : ("..." + str.substr(str.length() - count + 3));
}

bool Log::_passesFilter(const string& prettyFunction)
{
  // Currently, this initialization call needs to be executed each time here, rather than in a
  // constructor. There may be a way to optimize this.
  if (_includeClassFilter.isEmpty() && _excludeClassFilter.isEmpty())
    _setFilter();

  if (_includeClassFilter.isEmpty() && _excludeClassFilter.isEmpty())
    return true;

  QString prettyFunctionQt = QString::fromStdString(prettyFunction);
  QString name;
  if (!prettyFunctionQt.endsWith(".js"))    // call from C++
  {
    // split arguments from function call name
    QStringList nameParts = prettyFunctionQt.split("(");
    if (nameParts.length() < 1) return true;

    // split class name from function name
    nameParts = nameParts[0].split("::", QString::SkipEmptyParts);
    const int listLen = nameParts.size();
    if (listLen <= 1)
    {
      return false;
    }
    name = nameParts[listLen - 2];
  }
  else  // call from a JS generic conflate script
  {
    // split arguments from script path
    const QStringList nameParts = prettyFunctionQt.split("/", QString::SkipEmptyParts);
    if (nameParts.length() < 1) return true;

    name = nameParts[nameParts.size() - 1];
  }

  // It would be nice if we could throw here if the class name isn't recognized, so we don't
  // wonder why a typo caused us not to get the logging we thought we were going to get. However,
  // Factory::hasClass as a check won't work here since not all classes doing logging are
  // factories and not sure of any other way to validate it.

  // If anything was added to the exclude filter and this class was explicitly excluded, we'll skip
  // logging.
  if (!_excludeClassFilter.isEmpty() && StringUtils::matchesWildcard(name, _excludeClassFilter))
  {
    return false;
  }
  // If nothing was added to the include list, everything is allowed to log. Otherwise, only allow
  // this class to log if it has been added to the include list.
  return _includeClassFilter.isEmpty() || StringUtils::matchesWildcard(name, _includeClassFilter);
}

void Log::setLevel(WarningLevel l)
{
  _level = l;
}

void Log::_setFilter()
{
  _includeClassFilter = ConfigOptions().getLogClassIncludeFilter();
  _includeClassFilter.removeDuplicates();
  _excludeClassFilter = ConfigOptions().getLogClassExcludeFilter();
  _excludeClassFilter.removeDuplicates();
}

const char* Log::beginDecoration(WarningLevel level) const
{
  if (_decorateLogs)
  {
    switch (level)
    {
    default:
    case None:    return LogColor::None();
    case Trace:   return LogColor::Trace();
    case Debug:   return LogColor::Debug();
    case Info:    return LogColor::Info();
    case Status:  return LogColor::Status();
    case Warn:    return LogColor::Warn();
    case Error:   return LogColor::Error();
    case Fatal:   return LogColor::Fatal();
    }
  }
  else
    return "";
}

const char* Log::endDecoration() const
{
  if (_decorateLogs)
    return LogColor::None();
  else
    return "";
}

}
