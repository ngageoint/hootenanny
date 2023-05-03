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
 * @copyright Copyright (C) 2023 Maxar (http://www.maxar.com/)
 */

#ifndef STRING_MEMORY_INTERFACE_H
#define STRING_MEMORY_INTERFACE_H

//  Qt
#include <QString>

namespace hoot
{

/**
 * @brief The StringMemoryInterface contains a memory saving technique based on QStrings
 */
class StringMemoryInterface
{
public:
  StringMemoryInterface() = default;
  virtual ~StringMemoryInterface() = default;
  /**
   * @brief getStringLocation Get a string with a consistant internal memory address
   * @param s String to save or get the memory location of
   * @return QString object that points to the string located in the string table
   */
  const QString& getStringLocation(const QString& s);

protected:

  QHash<QString, QString> _strings;
};

}

#endif  //  STRING_MEMORY_INTERFACE_H
