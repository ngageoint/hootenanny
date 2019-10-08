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
 * @copyright Copyright (C) 2015, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef UUIDHELPER_H
#define UUIDHELPER_H

// Qt
#include <QUuid>

namespace hoot
{

/**
 * The UuidHelper provides a number of convenience functions when working with UUID. Many of these
 * functions become irrelevant with Qt 5.0+, but we're targetting Qt 4.7 now.
 *
 * @todo We're at Qt 5 now, so let's clean this up.
 */
class UuidHelper
{
public:

  UuidHelper();

  /**
   * Create a uuid using the global settings for defining the technique.
   */
  static QUuid createUuid();

  /**
   * Create a version 5 UUID by hashing the provided string. The provided namespace is concatenated
   * with the string before the hash is calculated. By default we use a consistent, but arbitrary
   * namespace.
   *
   * @param string User provided string to use in the hash.
   * @param ns Namespace. This will be hashed with the user provided string. Default is Hoot
   *           specific.
   * @return Returns a UUID based on a hash. This is consistent for a given ns/string combination.
   */
  static QUuid createUuid5(QString string,
                           QUuid ns = QUuid("{1c745d1e-39e5-4926-a2d3-8f87af39e037}"));

  static void resetRepeatableKey() { _repeatableKey = 0; }

  static QByteArray toByteArray(const QUuid& uuid);

  static QUuid toUuid(const QByteArray& bytes);

  static int _repeatableKey;
};

}

#endif // UUIDHELPER_H
