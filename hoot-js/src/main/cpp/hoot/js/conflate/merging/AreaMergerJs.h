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
 * @copyright Copyright (C) 2016, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef AREAMERGERJS_H
#define AREAMERGERJS_H

// Hoot
#include <hoot/js/HootJsStable.h>
#include <hoot/js/SystemNodeJs.h>
#include <hoot/js/PluginContext.h>
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

/**
 * Merges areas
 */
class AreaMergerJs
{

public:

  /**
   * Merges an unlimited number of areas together using a generic conflation script
   *
   * The map passed may or may not contain constituent elements (way nodes, relation members).
   *
   * @param map a map containing the buildings to be merged
   * @param mergeTargetId the ID of the area which all other areas should be merged into
   * @param current the context this method should run under
   */
  static void mergeAreas(OsmMapPtr map, const ElementId& mergeTargetId, v8::Isolate* current);
};

}

#endif // AREAMERGERJS_H
