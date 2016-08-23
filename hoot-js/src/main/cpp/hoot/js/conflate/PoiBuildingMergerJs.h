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
#ifndef POIBUILDINGMERGERJS_H
#define POIBUILDINGMERGERJS_H

#include <hoot/js/SystemNodeJs.h>
#include <v8.h>

namespace hoot
{

/**
 * Hoot JS wrapper around PoiBuildingMerger
 */
class PoiBuildingMergerJs : public node::ObjectWrap
{

public:

 static void Init(v8::Handle<v8::Object> target);

private:

  PoiBuildingMergerJs();
  ~PoiBuildingMergerJs();

  /**
   * Merges a POI with a building
   *
   * @param args an OSM map containing a single node POI and a single poly building, which
   * can be a way or a relation (multipolygon)
   * @return a map with the building and POI merged
   */
  static v8::Handle<v8::Value> jsPoiBuildingMerge(const v8::Arguments& args);
};

}

#endif // POIBUILDINGMERGERJS_H
