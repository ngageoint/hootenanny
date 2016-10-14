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

#ifndef LOG_H
#define LOG_H

#include <hoot/core/HootConfig.h>

// Qt
#include <QMap>
#include <QString>
#include <QVariant>

// Standard
#include <deque>
#include <set>
#include <string>
#include <vector>

// Tgs
#include <tgs/HashMap.h>
#include <tgs/TgsException.h>

#if HOOT_HAVE_LIBLOG4CXX
# include <log4cxx/logger.h>
#endif

namespace hoot
{
using namespace std;

/**
 * This class is here to abstract out the logging interface. I only have mild confidence in log4cxx
 * and I don't really need all the complicated goodies.
 */
class Log
{

public:
  enum WarningLevel
  {
    None = 0,
    /**
     * Used for logging variable states and other info for tracing code. You want to know the value
     * of 'i'? Use trace.
     */
    Trace = 500,
    /**
     * Used for program state information that may be useful when debugging other parts of the code.
     * E.g. noting the projection or bounds of a map.
     */
    Debug = 1000,
    Verbose = 1500,
    /**
     * Status information that would be useful to a user while the application is running. E.g.
     * completion of phases of processing, or the number of features being processed.
     */
    Info = 2000,
    Warn = 3000,
    Error = 4000,
    Fatal = 5000
  };

  static Log& getInstance()
  {
    if (_theInstance == 0)
    {
      _theInstance = new Log();
      _theInstance->init();
      _theInstance->setLevel(_theInstance->_level);
    }
    return *_theInstance;
  }

  WarningLevel getLevel() { return _level; }

  static WarningLevel getLevelFromString(QString l);

  QString getLevelString(WarningLevel l);

  /**
   * May get called multiple times (e.g. before and after config settings are finalized).
   */
  void init();

  bool isDebugEnabled() { return _level <= Debug; }

  bool isInfoEnabled() { return _level <= Info; }

  void log(WarningLevel level, const string& str);

  void log(WarningLevel level, const string& str, const string& filename,
           const string &functionName, int lineNumber);

  void log(WarningLevel level, const QString& str, const QString& filename,
    const QString& functionName, int lineNumber);

  void progress(WarningLevel level, const string& str, const string& filename,
    const string& functionName, int lineNumber);

  void setLevel(WarningLevel l);

#if HOOT_HAVE_LIBLOG4CXX
  log4cxx::LoggerPtr _logger;
#endif

  static string ellipsisStr(const string& str, uint count = 33);

private:
  WarningLevel _level;
  static Log* _theInstance;

  Log();
};

/**
 * This class temporarily sets the log level to tmpLevel. When the object is destroyed or reset is
 * called the log level will be set back to its original level.
 */
class DisableLog
{
public:
  DisableLog(Log::WarningLevel tmpLevel = Log::Error)
  {
    _oldLevel = Log::getInstance().getLevel();
    Log::getInstance().log(Log::Debug, "Disabling logging.");
    Log::getInstance().setLevel(tmpLevel);
  }

  ~DisableLog()
  {
    reset();
  }

  void reset()
  {
    Log::getInstance().setLevel(_oldLevel);
    Log::getInstance().log(Log::Debug, "Enabled logging.");
  }

private:
  Log::WarningLevel _oldLevel;
};

// recreate the TGS stream utils within our namespace. I haven't found an elegant way to reliably
// use operator<< within the Tgs namespace from within hoot. :(
#include <tgs/StreamUtils.hh>
}

#define PROGRESS_LEVEL(level, message) {\
  if ((level) >= hoot::Log::getInstance().getLevel()) \
  { \
    std::stringstream ss_; \
    ss_ << message; \
    hoot::Log::getInstance().progress((level), ss_.str(), __FILE__, "", __LINE__); \
  }}

#define PROGRESS_INFO(str) { PROGRESS_LEVEL(hoot::Log::Info, str) }

/// print out a variable along w/ it's value. E.g. int a = 3; LOG_VAR(a); => logs "a: 3"
#define LOG_VART(var) LOG_TRACE(#var << ": " << (var))
#define LOG_VARD(var) LOG_DEBUG(#var << ": " << (var))
#define LOG_VARV(var) LOG_VERBOSE(#var << ": " << (var))
#define LOG_VARI(var) LOG_INFO(#var << ": " << (var))
#define LOG_VARW(var) LOG_WARN(#var << ": " << (var))
#define LOG_VARE(var) LOG_ERROR(#var << ": " << (var))
#define LOG_VAR(var) LOG_VARI(var)

// The following macros will be defined by these includes. Using anything else could cause
// portability issues.
// LOG_LEVEL, LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_FATAL
#ifdef HOOT_HAVE_LIBLOG4CXX
# include "LogLog4Cxx.h"
#else
# include "LogGeneric.h"
#endif

#endif // LOG_H
