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
 * @copyright Copyright (C) 2015, 2018, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef STRINGDISTANCECONSUMER_H
#define STRINGDISTANCECONSUMER_H

#include <hoot/core/algorithms/string/StringDistance.h>

namespace hoot
{

/**
 * @brief The StringDistanceConsumer class is an interface for consuming a StringDistance.
 */
class StringDistanceConsumer
{
public:

  StringDistanceConsumer() = default;
  virtual ~StringDistanceConsumer() = default;

  virtual void setStringDistance(const StringDistancePtr& sd) = 0;
};

}

#endif // STRINGDISTANCECONSUMER_H
