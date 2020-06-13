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
 * @copyright Copyright (C) 2015, 2017, 2018, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef TAGSIMILARITYSCORER_H
#define TAGSIMILARITYSCORER_H

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Element.h>

namespace hoot
{

class Tags;

class TagDifferencer
{
public:
  static std::string className() { return "hoot::TagDifferencer"; }

  TagDifferencer() = default;
  virtual ~TagDifferencer() = default;

  /**
   * Returns a value from 0 to 1. 0 means they're identical as far as this differencer is concerned
   * and 1 means they're different. Each differencer may (will?) have a different definition of
   * what difference means. Possibly just the difference in name tags, or only looks at highway
   * related tags, etc.
   *
   * The score returned should not be considered linear. If a = 1 and b = 0.01, then all you can
   * say is that a is more different than b, not that a is 100 times more different, etc.
   */
  virtual double diff(const ConstOsmMapPtr& map, const ConstElementPtr& e1,
    const ConstElementPtr& e2) const = 0;
};

}

#endif // TAGSIMILARITYSCORER_H
