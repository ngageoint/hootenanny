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
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/ConfigOptions.h>

// GDAL
#include <cpl_error.h>

// Qt
#include <QDebug>

// Standard
#include <iostream>
using namespace std;

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
_level(Log::Info)
{
  qInstallMessageHandler(myLoggerFunction);
  CPLSetErrorHandler(cplErrorHandler);
}

int Log::getWarnMessageLimit()
{
  if (_warnMessageLimit == 0)
    _warnMessageLimit = ConfigOptions().getLogWarnMessageLimit();
  return _warnMessageLimit;
}

Log::WarningLevel Log::levelFromString(QString l)
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

void Log::log(WarningLevel level, const QString& str, const QString& filename,
  const QString& prettyFunction, int lineNumber)
{
  log(level, string(str.toUtf8().data()), string(filename.toUtf8().data()),
      string(prettyFunction.toUtf8().data()), lineNumber);
}

string Log::ellipsisStr(const string& str, uint count)
{
  return (str.length() <= count) ? str : ("..." + str.substr(str.length() - count + 3));
}

}
