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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef TAGFILTER_H
#define TAGFILTER_H

// Hoot
#include <hoot/core/elements/Way.h>

#include "WayFilter.h"

namespace hoot
{

class TagFilter : public WayFilter
{
public:

  static string className() { return "hoot::TagFilter"; }

  /**
   * Constructs a tag filter and uses the key value pair specified in the configuration file. If
   * not kvp is specified then an error is thrown.
   */
  TagFilter();

  /**
   * If FilterType is KeepMatches then keep all ways that have the specified key value pair.
   */
  TagFilter(FilterType type, QString k, QString v) { _type = type; _k = k; _v = v; }

  virtual bool isFiltered(const Way &w) const { return isFiltered((const Element&)w); }

protected:
  virtual bool isFiltered(const Element& e) const;

private:
  QString _k, _v;
  FilterType _type;
};

}

#endif // TAGFILTER_H
