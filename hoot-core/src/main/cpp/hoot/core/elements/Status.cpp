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
 * @copyright Copyright (C) 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

#include "Status.h"

// Hoot


#include <hoot/core/schema/MetadataTags.h>

using namespace std;

namespace hoot
{

Status Status::fromInput(int i)
{
  Status result;

  if (i < 0)
  {
    throw HootException("Invalid input offset.");
  }
  if (i <= 1)
  {
    result._type = i + 1;
  }
  else
  {
    result._type = i - 1 + EnumEnd;
  }

  return result;
}

int Status::getInput() const
{
  int result;
  if (_type > EnumEnd)
  {
    result = _type - EnumEnd + 1;
  }
  else
  {
    switch (_type)
    {
    case Status::Unknown1:
      result = 0;
      break;
    case Status::Unknown2:
      result = 1;
      break;
    default:
      throw HootException("This is not an input status type.");
    }
  }

  return result;
}

QString Status::toString() const
{
  if (_type > EnumEnd)
  {
    int inputNum = _type - EnumEnd + 2;
    return QString("Input%1").arg(inputNum, 3, 10, QChar('0'));
  }
  else
  {
    switch (_type)
    {
    case Status::Invalid:
      return "Invalid";
    case Status::Unknown1:
      return MetadataTags::Unknown1();
    case Status::Unknown2:
      return MetadataTags::Unknown2();
    case Status::Conflated:
      return "Conflated";
    case Status::TagChange:
      return "TagChange";
    default:
      return QString("Unknown (%1)").arg(_type);
    }
  }
}

QString Status::toTextStatus() const
{
  if (_type > EnumEnd)
  {
    return toString();
  }
  else
  {
    switch (_type)
    {
    case Status::Invalid:
      return "Invalid";
    case Status::Unknown1:
      return "Input1";
    case Status::Unknown2:
      return "Input2";
    case Status::Conflated:
      return "Conflated";
    case Status::TagChange:
      return "TagChange";
    default:
      return QString("Unknown (%1)").arg(_type);
    }
  }
}

Status::Type Status::fromString(QString statusString)
{
  statusString = statusString.toLower();

  bool rawOk;
  int rawNum = statusString.toInt(&rawOk);

  if (rawOk)
  {
    return rawNum;
  }
  else if (statusString == "invalid" || statusString == QString::number(Status::Invalid))
  {
    return Invalid;
  }
  else if (statusString == "unknown1" || statusString == "input1" ||
           statusString == QString::number(Status::Unknown1))
  {
    return Unknown1;
  }
  else if (statusString == "unknown2" || statusString == "input2" ||
           statusString == QString::number(Status::Unknown2))
  {
    return Unknown2;
  }
  else if (statusString == "conflated" || statusString == QString::number(Status::Conflated))
  {
    return Conflated;
  }
  // This is used by the DiffConflator.
  else if (statusString == "TagChange" || statusString == QString::number(Status::TagChange))
  {
    return TagChange;
  }
  else if (statusString.startsWith("input"))
  {
      bool ok;
      int num = statusString.replace("input", "").toInt(&ok);
      if (!ok)
      {
        throw HootException("Invalid element type string: " + statusString);
      }
      else
      {
        // Input 1 and 2 are enumeration 1 and 2. After that the inputs start at EnumEnd + 1 and
        // go up.
        if (num > 2)
        {
          num = (num - 2) + EnumEnd;
        }
        return num;
      }
  }
  else
  {
    throw IllegalArgumentException("Invalid element status string: " + statusString);
  }
}


}
