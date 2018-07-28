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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef CONFIGOPTIONSDISPLAYER_H
#define CONFIGOPTIONSDISPLAYER_H

// Qt
#include <QString>

namespace hoot
{

/**
 *
 */
class ConfigOptionsDisplayer
{
public:

  /**
   *
   *
   * @param withDetails
   */
  static void displayAllOptionNames(const bool withDetails = false);

  /**
   *
   *
   * @param optionName
   * @param withDetails
   */
  static void displayOptionName(const QString optionName, const bool withDetails = false);
  
private:

  static void _printAllConfigOptionsDetails(const QString optionName,
                                            const QString configOptionsFile);
};

}

#endif // CONFIGOPTIONSDISPLAYER_H
