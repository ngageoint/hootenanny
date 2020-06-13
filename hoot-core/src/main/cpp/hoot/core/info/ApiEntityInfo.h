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
#ifndef API_ENTITY_INFO_H
#define API_ENTITY_INFO_H

namespace hoot
{

/**
 * Interface to describe the functionality of various Hoot API entities. e.g. OsmMapOperation,
 * ElementVisitor, ValueAggregator, etc.
 */
class ApiEntityInfo
{
public:

  ApiEntityInfo() = default;
  virtual ~ApiEntityInfo() = default;

  /**
   * Returns a one sentence description for the entity.
   *
   * Keep this as short as possible, capitalize the first letter, and check to see that it stays
   * on one line when displayed with the 'info --operators' command.
   *
   * To prevent a description from being displayed by the 'info' command, this may be implemented
   * as returning an empty string.
   */
  virtual QString getDescription() const = 0;
};

}

#endif // API_ENTITY_INFO_H
