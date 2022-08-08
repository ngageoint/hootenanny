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
 * @copyright Copyright (C) 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef LOG_COLOR_H
#define LOG_COLOR_H

// Qt
#include <QRegularExpression>

namespace hoot
{

class LogColor
{
public:
  /** Default coloring */
  static inline const char* None()   { return "\033[0m"; }
  /** Dim Purple Text */
  static inline const char* Trace()  { return "\033[2;35m"; }
  /** Regular Blue Text */
  static inline const char* Debug()  { return "\033[0;34m"; }
  /** Bright Green Text */
  static inline const char* Info()   {  return "\033[1;37m"; }
  /** Bright White Text */
  static inline const char* Status() {  return "\033[1;32m"; }
  /** Regular Red Text */
  static inline const char* Warn()   {  return "\033[0;33m"; }
  /** Bright Red Text */
  static inline const char* Error()  {  return "\033[1;31m"; }
  /** Red Background Text */
  static inline const char* Fatal()  {  return "\033[7;91m"; }
  /** Regular expression for stripping color from logs */
  static QRegularExpression ColorRegex;
};

}

#endif // UUIDHELPER_H
