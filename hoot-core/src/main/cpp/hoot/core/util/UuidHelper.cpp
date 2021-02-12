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
 * @copyright Copyright (C) 2015, 2019 Maxar (http://www.maxar.com/)
 */
#include "UuidHelper.h"

// hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/util/ConfigOptions.h>

// Qt
#include <QCryptographicHash>

namespace hoot
{

int UuidHelper::_repeatableKey;

UuidHelper::UuidHelper()
{
}

QUuid UuidHelper::createUuid()
{
  if (ConfigOptions().getUuidHelperRepeatable())
  {
    return createUuid5(QString::number(_repeatableKey++));
  }
  else
  {
    return QUuid::createUuid();
  }
}

QUuid UuidHelper::createUuid5(QString string, QUuid ns)
{
  // concat the namespace + the UTF-8 encoding of string.
  QByteArray concat = toByteArray(ns) + string.toUtf8();

  // calculate SHA-1 hash
  QByteArray hash = QCryptographicHash::hash(concat, QCryptographicHash::Sha1);

  // Grab just the first 16bytes of the SHA-1 hash
  QByteArray mid = hash.mid(0, 16);

  // Modify the bytes to contain the UUID version info
  // The four bits of M are the version number.
  // The two MSB in N are 0b10.
  // xxxxxxxx-xxxx-Mxxx-Nxxx-xxxxxxxxxxxx
  mid[6] = (mid[6] & 0x0F) | 0x50;
  mid[8] = (mid[8] & 0x3F) | 0x80;

  // Populate the QUuid with the bytes
  return toUuid(mid);
}

QByteArray UuidHelper::toByteArray(const QUuid& uuid)
{
  return QByteArray::fromHex(uuid.toString().toLatin1().replace("-", "").replace("{", "").
                             replace("}", ""));
}

QUuid UuidHelper::toUuid(const QByteArray& bytes)
{
  QByteArray result = bytes.toHex();
  result.insert(20, "-");
  result.insert(16, "-");
  result.insert(12, "-");
  result.insert(8, "-");
  return QUuid(QString(result));
}

}
