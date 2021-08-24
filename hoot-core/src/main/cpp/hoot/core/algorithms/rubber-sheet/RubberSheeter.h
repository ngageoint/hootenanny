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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef RUBBERSHEETER_H
#define RUBBERSHEETER_H

namespace hoot
{

/**
 * @brief The RubberSheeter class rubber sheets two inputs together.
 */
class RubberSheeter
{
public:

  RubberSheeter() = default;

  /**
   * @brief rubberSheet rubber sheets two inputs together.
   * @param input1 the first input to rubber sheet
   * @param input2 the second input to rubber sheet
   * @param output the rubber sheeted combined output
   */
  void rubberSheet(const QString& input1, const QString& input2, const QString& output) const;
};

}

#endif // RUBBERSHEETER_H
