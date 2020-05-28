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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
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

namespace hoot
{

/**
 * This class is here to abstract out the logging interface(Singleton).
 */
class Log
{

public:

  static QString LOG_WARN_LIMIT_REACHED_MESSAGE;

  enum WarningLevel
  {
    None = 0,
    /**
     * Used for logging variable states and other info for tracing code, often when inside loops.
     * You want to know the value of 'i'? Use trace.
     */
    Trace = 500,
    /**
     * Used for program state information that may be useful when debugging parts of the code not
     * inside a loop. E.g. noting the projection or bounds of a map.
     */
    Debug = 1000,
    /**
     * Status information that would be useful to a user while the application is running. E.g.
     * the number of features being processed.
     */
    Info = 2000,
    /**
     * Information specific to a command job's status; This generally is only used with a Progress
     * object and is a coarser log level than INFO.
     */
    Status = 2500,
    /**
     * Warn about anomalous situations, including input data encountered, that may cause logic to
     * produce incorrect results. Be judicious about what is included here and use a warning
     * message limit when possible.
     */
    Warn = 3000,
    /**
     * Describes anomalous situations, including data encountered, that definitely will cause logic
     * to produce incorrect results but is not fatal to the application.
     */
    Error = 4000,
    /**
     * Describes an unrecoverable situation
     */
    Fatal = 5000
  };

  static Log& getInstance();

  WarningLevel getLevel() const { return _level; }

  static WarningLevel levelFromString(QString l);

  static QString levelToString(WarningLevel l);

  QString getLevelAsString() const;

  /**
   * May get called multiple times (e.g. before and after config settings are finalized).
   */
  void init();

  bool isDebugEnabled() const { return _level <= Debug; }

  bool isInfoEnabled() const { return _level <= Info; }

  void log(WarningLevel level, const std::string& str);

  void log(WarningLevel level, const std::string& str, const std::string& filename,
    const std::string& prettyFunction, int lineNumber);

  void log(WarningLevel level, const QString& str, const QString& filename,
    const QString& prettyFunction, int lineNumber);

  void progress(WarningLevel level, const std::string& str, const std::string& filename,
    const std::string& functionName, int lineNumber);

  void setLevel(WarningLevel l);

  static int getWarnMessageLimit();

  static std::string ellipsisStr(const std::string& str, uint count = 33);

private:

  bool notFiltered(const std::string& prettyFunction);

  WarningLevel _level;
  static int _warnMessageLimit;
  bool _classFilterInitialized;
  QStringList _classFilter;

  Log();
  /** Default destructor */
  ~Log() = default;
  /** Delete copy constructor and assignment operator */
  Log(const Log&) = delete;
  Log& operator=(const Log&) = delete;
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
    Log::getInstance().log(Log::Trace, "Disabling logging.");
    Log::getInstance().setLevel(tmpLevel);
  }

  ~DisableLog()
  {
    reset();
  }

  void reset()
  {
    Log::getInstance().setLevel(_oldLevel);
    Log::getInstance().log(Log::Trace, "Enabled logging.");
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

#define PROGRESS_STATUS(str) { PROGRESS_LEVEL(hoot::Log::Status, str) }
#define PROGRESS_INFO(str) { PROGRESS_LEVEL(hoot::Log::Info, str) }
#define PROGRESS_DEBUG(str) { PROGRESS_LEVEL(hoot::Log::Debug, str) }
#define PROGRESS_TRACE(str) { PROGRESS_LEVEL(hoot::Log::Trace, str) }

/// print out a variable along w/ it's value. E.g. int a = 3; LOG_VAR(a); => logs "a: 3"
#define LOG_VART(var) LOG_TRACE(#var << ": " << (var))
#define LOG_VARD(var) LOG_DEBUG(#var << ": " << (var))
#define LOG_VARI(var) LOG_INFO(#var << ": " << (var))
#define LOG_VARW(var) LOG_WARN(#var << ": " << (var))
#define LOG_VARE(var) LOG_ERROR(#var << ": " << (var))
#define LOG_VAR(var) LOG_VARI(var)

# include "LogGeneric.h"

#endif // LOG_H
