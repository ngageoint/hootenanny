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
 * @copyright Copyright (C) 2015, 2016, 2018, 2019 Maxar (http://www.maxar.com/)
 */
#ifndef LOGGENERIC_H
#define LOGGENERIC_H

// Standard
#include <sstream>

#include <hoot/core/util/Log.h>

#define LOG_LEVEL(level, message) {\
  if ((level) >= hoot::Log::getInstance().getLevel()) \
  { \
    std::stringstream ss_; \
    ss_ << message; \
    hoot::Log::getInstance().log((level), ss_.str(), __FILE__, __PRETTY_FUNCTION__, __LINE__); \
  }}

#define LOG_TRACE(str) { LOG_LEVEL(hoot::Log::Trace, str) }
#define LOG_DEBUG(str) { LOG_LEVEL(hoot::Log::Debug, str) }
#define LOG_INFO(str) { LOG_LEVEL(hoot::Log::Info, str) }
#define LOG_STATUS(str) { LOG_LEVEL(hoot::Log::Status, str) }
#define LOG_WARN(str) { LOG_LEVEL(hoot::Log::Warn, str) }
#define LOG_ERROR(str) { LOG_LEVEL(hoot::Log::Error, str) }
#define LOG_FATAL(str) { LOG_LEVEL(hoot::Log::Fatal, str) }

#endif // LOGGENERIC_H
