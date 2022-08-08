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
 * @copyright Copyright (C) 2015, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#ifndef NUMERICLIMITS_H
#define NUMERICLIMITS_H

//Std Includes
#include <limits>

template<typename T>
inline T numericLimitMax()
{
  return std::numeric_limits<T>::max();
}

template<typename T>
inline T numericLimitMin()
{
  return std::numeric_limits<T>::min();
}

template <>
inline double numericLimitMin<double>()
{
  return -std::numeric_limits<double>::max();
}

template <>
inline float numericLimitMin<float>()
{
  return -std::numeric_limits<float>::max();
}

#endif // NUMERICLIMITS_H
