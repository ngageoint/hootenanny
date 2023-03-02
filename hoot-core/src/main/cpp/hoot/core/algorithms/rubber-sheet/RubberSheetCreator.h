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
#ifndef RUBBERSHEETCREATOR_H
#define RUBBERSHEETCREATOR_H

namespace hoot
{

/**
 * @brief The RubberSheetCreator class creates a rubber sheet transforms from manual tie-points.
 */
class RubberSheetCreator
{
public:

  RubberSheetCreator() = default;

  /**
   * @brief derive derives rubber sheet transforms for rubber sheeting either direction between two
   * inputs.
   * @param tiepoints tie point filename
   * @param input filename for the input map used for projection
   * @param transform2To1 output transformation for rubber sheeting from input 2 to input 1
   * @param transform1To2 output transformation for rubber sheeting from input 1 to input 2
   * @param ref if true, input 1 is treated as the reference input and held constant
   */
  void create(const QString& tiepoints, const QString& input, const QString& input2,  const QString& transform2To1,
              const QString& transform1To2 = "", const bool ref = false) const;
};

}

#endif // RUBBERSHEETCREATOR_H
