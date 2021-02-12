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
 * @copyright Copyright (C) 2015 Maxar (http://www.maxar.com/)
 */
#ifndef CSLUTILITIES_H
#define CSLUTILITIES_H

//Std Includes
#include <string>
#include <vector>

class CslUtilities
{
public:

  /**
   * @brief readCostMatrix read a file containing the cost matrix in row major form (space separate column entries and CR for each row)
   * @param filename the name of the file
   * @return the cost matrix
   */
  static std::vector<float> readCostMatrix(std::string filename);
};

#endif // CSLUTILITIES_H
