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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "ServicesJobStatus.h"

// Hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

using namespace std;

namespace hoot
{

QString ServicesJobStatus::toString() const
{
  switch (_type)
  {
    case ServicesJobStatus::Running:
      return "Running";
    case ServicesJobStatus::Complete:
      return "Complete";
    case ServicesJobStatus::Failed:
      return "Failed";
    case ServicesJobStatus::Cancelled:
      return "Cancelled";
    case ServicesJobStatus::Unknown:
      return "Unknown";
    default:
      return QString("Unknown (%1)").arg(_type);
  }
}

ServicesJobStatus::Type ServicesJobStatus::fromString(QString typeString)
{
  const QString typeStr = typeString.toLower();
  if (typeStr == "running")
  {
    return Running;
  }
  else if (typeStr == "complete")
  {
    return Complete;
  }
  else if (typeStr == "failed")
  {
    return Failed;
  }
  else if (typeStr == "cancelled")
  {
    return Cancelled;
  }
  else if (typeStr == "unknown")
  {
    return Unknown;
  }
  return Unknown;
}

}
