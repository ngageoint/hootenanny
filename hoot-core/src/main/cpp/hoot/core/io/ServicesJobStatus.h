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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef SERVICES_JOB_STATUS_H
#define SERVICES_JOB_STATUS_H

// Qt
#include <QString>

namespace hoot
{

class ServicesJobStatus
{
public:

  // matches the same enum in hoot-services JobStatus.java
  typedef enum TypeEnum
  {
    Running = 0,
    Complete,
    Failed,
    Cancelled,
    Unknown
  } TypeEnum;

  typedef int Type;

  ServicesJobStatus() { _type = Unknown; }
  ServicesJobStatus(Type type) { _type = type; }

  bool operator==(ServicesJobStatus t) const { return t._type == _type; }
  bool operator!=(ServicesJobStatus t) const { return t._type != _type; }

  Type getEnum() const { return _type; }

  QString toString() const;

  /**
   * @brief fromString Parses type from either a human readable string or the numeric string.
   * @param typeString The string to parse.
   * @return The type parsed, or throws an exception if it is an invalid string.
   */
  static Type fromString(QString typeString);

private:

  ServicesJobStatus::Type _type;
};

}

#endif // SERVICES_JOB_STATUS_H
