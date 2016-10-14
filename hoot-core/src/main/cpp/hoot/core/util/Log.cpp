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

#include "Log.h"

// hoot
#include <hoot/core/util/HootException.h>

// GDAL
#include <cpl_error.h>

// Qt
#include <QDebug>

// Standard
#include <iostream>
using namespace std;

namespace hoot
{

Log* Log::_theInstance = 0;

void myLoggerFunction(QtMsgType type, const char* msg)
{
  Log::WarningLevel l = Log::Fatal;
  switch(type)
  {
  case QtDebugMsg:
    l = Log::Debug;
    break;
  case QtWarningMsg:
    l = Log::Warn;
    break;
  case QtCriticalMsg:
    l = Log::Error;
    break;
  case QtFatalMsg:
    l = Log::Fatal;
    break;
  }

  Log::getInstance().log(l, msg);
}

static void cplErrorHandler(CPLErr eErrClass, int err_no, const char *msg)
{
  Log::WarningLevel l = Log::Fatal;
  switch (eErrClass)
  {
  case CE_None:
    l = Log::None;
    break;
  case CE_Debug:
    l = Log::Debug;
    break;
  case CE_Warning:
    l = Log::Warn;
    break;
  case CE_Failure:
    l = Log::Error;
    break;
  case CE_Fatal:
    l = Log::Fatal;
    break;
  }

  stringstream ss;
  ss << "CPL Error #: " << err_no << " " << msg;
  Log::getInstance().log(l, ss.str());
}

Log::Log()
{
  _level = Log::Info;
  qInstallMsgHandler(myLoggerFunction);
  CPLSetErrorHandler(cplErrorHandler);
}

Log::WarningLevel Log::getLevelFromString(QString l)
{
  l = l.toLower();
  if (l == "none")
  {
    return None;
  }
  if (l == "trace")
  {
    return Trace;
  }
  if (l == "debug")
  {
    return Debug;
  }
  if (l == "warn")
  {
    return Warn;
  }
  if (l == "error")
  {
    return Error;
  }
  if (l == "fatal")
  {
    return Fatal;
  }

  throw IllegalArgumentException("Unexpected log level string: " + l);
}

QString Log::getLevelString(WarningLevel l)
{
  switch(l)
  {
  case None:
    return "NONE";
  case Trace:
    return "TRACE";
  case Debug:
    return "DEBUG";
  case Verbose:
    return "VERBOSE";
  case Info:
    return "INFO";
  case Warn:
    return "WARN";
  case Error:
    return "ERROR";
  case Fatal:
    return "FATAL";
  default:
    return "UNK";
  }
}

void Log::log(WarningLevel level, const QString& str, const QString& filename,
  const QString& functionName, int lineNumber)
{
  log(level, string(str.toUtf8().data()), string(filename.toUtf8().data()),
      string(functionName.toUtf8().data()), lineNumber);
}

string Log::ellipsisStr(const string& str, uint count)
{
  return (str.length() <= count) ? str : ("..." + str.substr(str.length() - count + 3));
}

}
