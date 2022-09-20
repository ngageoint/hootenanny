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
 * @copyright Copyright (C) 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#ifndef CONFIGOPTIONSDISPLAYER_H
#define CONFIGOPTIONSDISPLAYER_H

namespace hoot
{

/**
 * Displays configuration options
 */
class ConfigOptionsDisplayer
{
public:

  /**
   * Displays all configuration options
   *
   * @param withDetails if true, displays details along with the option names; if false only
   * displays option names
   */
  static QString getAllOptionNames(const bool withDetails = false);

  /**
   * Display a single configuration option
   *
   * @param optionName the name of the option to display
   * @param withDetails if true, displays details along with the option name; if false only
   * displays the option name
   */
  static QString getOptionName(const QString& optionName, const bool withDetails = false);
  
private:

  static QString _getAllConfigOptionsDetails(const QString& optionName, const QString& configOptionsFile);
};

}

#endif // CONFIGOPTIONSDISPLAYER_H
