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
 * @copyright Copyright (C) 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef WAYJOINER_BASIC_H
#define WAYJOINER_BASIC_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/algorithms/WayJoiner.h>

namespace hoot
{

/**
 * Joins ways back together that were split during pre-conflation cleaning or during matching.
 */
class WayJoinerBasic : public WayJoiner
{
public:

  static QString className() { return "hoot::WayJoinerBasic"; }

  WayJoinerBasic() = default;
  virtual ~WayJoinerBasic() = default;

  /**
   * Static method to join all joinable ways using WayJoinerBasic
   */
  static void joinWays(const OsmMapPtr& map);

  virtual QString getDescription() const override
  { return "Rejoins ways split during pre-conflation cleaning or conflation matching."; }

  virtual QString getName() const override { return className(); }
};

}

#endif  //  WAYJOINER_BASIC_H
