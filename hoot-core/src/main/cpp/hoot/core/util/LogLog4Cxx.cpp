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
#include <hoot/core/util/ConfigOptions.h>

// log4cxx
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/consoleappender.h>
#include <log4cxx/logger.h>
#include <log4cxx/patternlayout.h>
#include <log4cxx/spi/location/locationinfo.h>
using namespace log4cxx;
using namespace log4cxx::spi;

// Qt
#include <QDateTime>
#include <QDebug>

// Standard
#include <iomanip>
#include <iostream>
using namespace std;

namespace hoot
{

LoggerPtr& getLogger()
{
  return Log::getInstance()._logger;
}

LevelPtr toLog4CxxLevel(Log::WarningLevel l)
{
  switch(l)
  {
  case Log::None:
    return Level::getAll();
    break;
  case Log::Trace:
    return Level::getTrace();
    break;
  case Log::Debug:
    return Level::getDebug();
    break;
  case Log::Info:
    return Level::getInfo();
    break;
  case Log::Verbose:
    return Level::getInfo();
    break;
  case Log::Warn:
    return Level::getWarn();
    break;
  case Log::Error:
    return Level::getError();
    break;
  case Log::Fatal:
    return Level::getFatal();
    break;
  }
  Log::getInstance().log(Log::Warn, "Got unexpected log level. " +
    QString("%1").arg(l).toStdString());
  return Level::getFatal();
}

void Log::init()
{
  _logger = Logger::getLogger("hoot");
  LayoutPtr layout(new PatternLayout(ConfigOptions().getLogFormat().toStdString()));
  AppenderPtr appender(new ConsoleAppender(layout));
  _logger->removeAllAppenders();
  _logger->addAppender(appender);
  _logger->setLevel(toLog4CxxLevel(Log::getLevel()));
}

void Log::log(WarningLevel level, const string& str)
{
  if (level >= _level)
  {
    _logger->log(toLog4CxxLevel(level), str, LocationInfo::getLocationUnavailable());
  }
}

void Log::log(WarningLevel level, const string& str, const string& filename,
  const string& functionName, int lineNumber)
{
  if (level >= _level)
  {
    _logger->log(toLog4CxxLevel(level), str, LocationInfo(filename.data(), functionName.data(),
      lineNumber));
  }
}

void Log::progress(WarningLevel level, const string& str, const string& filename,
  const string& /*functionName*/, int lineNumber)
{
  if (level >= _level)
  {
    QDateTime dt = QDateTime::currentDateTime();

    // takes the form: "09:34:21.635 WARN  <filename>(<lineNumber>) <str>"
    cout << dt.toString("hh:mm:ss.zzz") << " " << setw(5) << left << getLevelString(level) << " " <<
            ellipsisStr(filename) << "(" << setw(4) << right << lineNumber << ")" << " " << str <<
            "        \r" << flush;
  }
}

void Log::setLevel(WarningLevel l)
{
  _logger->setLevel(toLog4CxxLevel(l));
  _level = l;
}

}
