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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef STRINGDISTANCE_H
#define STRINGDISTANCE_H

// Hoot
#include <hoot/core/info/ApiEntityInfo.h>

namespace hoot
{

/**
 * @brief The StringDistance class is an interface to determining the similarity of two strings.
 */
class StringDistance : public ApiEntityInfo
{
public:

  static QString className() { return "StringDistance"; }

  StringDistance() = default;
  virtual ~StringDistance() = default;

  /**
   * @brief compare returns a value from 1 (very similar) to 0 (very dissimilar) describing the
   * distance between two strings.
   */
  virtual double compare(const QString& s1, const QString& s2) const = 0;
};

using StringDistancePtr = std::shared_ptr<StringDistance>;

}

#endif // STRINGDISTANCE_H
