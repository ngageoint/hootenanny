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

#ifndef RUBBERSHEETAPPLIER_H
#define RUBBERSHEETAPPLIER_H

// Qt


namespace hoot
{

/**
 * @brief The RubberSheetApplier class applies a rubber sheet transform.
 */
class RubberSheetApplier
{
public:

  RubberSheetApplier() = default;

  /**
   * @brief apply applies a rubber sheet transform to input and writes to output.
   * @param transform transform to apply
   * @param input input to apply the transform to
   * @param output path to write the transformed output
   */
  void apply(const QString& transform, const QString& input, const QString& output) const;
};

}

#endif // RUBBERSHEETAPPLIER_H
