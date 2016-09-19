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

#ifndef LOGLOG4CXX_H
#define LOGLOG4CXX_H

#include <log4cxx/logger.h>

#include <iostream>

namespace hoot
{

log4cxx::LoggerPtr& getLogger();
log4cxx::LevelPtr toLog4CxxLevel(Log::WarningLevel l);

#define LOG_LEVEL(level, message) {\
  if ((level) >= hoot::Log::getInstance().getLevel()) \
  { \
    log4cxx::LevelPtr log4cxxLevel = toLog4CxxLevel(level); \
    std::stringstream ss_; \
    ss_ << message; \
    LOG4CXX_LOGLS(hoot::getLogger(), log4cxxLevel, ss_.str()); \
  }}

#define LOG_TRACE(str) { LOG_LEVEL(hoot::Log::Trace, str) }
#define LOG_DEBUG(str) { LOG_LEVEL(hoot::Log::Debug, str) }
#define LOG_VERBOSE(str) { LOG_LEVEL(hoot::Log::Verbose, str) }
#define LOG_INFO(str) { LOG_LEVEL(hoot::Log::Info, str) }
#define LOG_WARN(str) { LOG_LEVEL(hoot::Log::Warn, str) }
#define LOG_ERROR(str) { LOG_LEVEL(hoot::Log::Error, str) }
#define LOG_FATAL(str) { LOG_LEVEL(hoot::Log::Fatal, str) }

}

#endif // LOGLOG4CXX_H
