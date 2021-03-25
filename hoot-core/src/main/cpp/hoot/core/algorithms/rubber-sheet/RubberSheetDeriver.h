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
 * @copyright Copyright (C) 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef RUBBERSHEETDERIVER_H
#define RUBBERSHEETDERIVER_H

// Qt
#include <QString>

/**
 * Derives a rubber sheet transforms
 */
namespace hoot
{

class RubberSheetDeriver
{
public:

  RubberSheetDeriver() = default;

  /**
   * Derives rubber sheet transforms for rubber sheeting either direction between two inputs
   *
   * @param input1 input 1
   * @param input2 input 2
   * @param transform2To1 output transformation for rubber sheeting from input 2 to input 1
   * @param transform1To2 output transformation for rubber sheeting from input 1 to input 2
   * @param ref if true, input 1 is treated as the reference input and held constant
   */
  void derive(const QString& input1, const QString& input2, const QString& transform2To1,
              const QString& transform1To2 = "", const bool ref = false);
};

}

#endif // RUBBERSHEETDERIVER_H
