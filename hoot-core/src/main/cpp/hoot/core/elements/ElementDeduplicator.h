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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef ELEMENT_DEDUPLICATOR_H
#define ELEMENT_DEDUPLICATOR_H

// Hoot
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

/**
 * TODO
 */
class ElementDeduplicator
{

public:

  ElementDeduplicator();

  /**
   * TODO
   *
   * @param map
   */
  void dedupe(OsmMapPtr map);

  /**
   * TODO
   *
   * @param map1
   * @param map2
   */
  void dedupe(OsmMapPtr map1, OsmMapPtr map2);

  void setDedupeIntraMap(bool dedupe) { _dedupeIntraMap = dedupe; }
  void setDedupeNodes(bool dedupe) { _dedupeNodes = dedupe; }
  void setDedupeWays(bool dedupe) { _dedupeWays = dedupe; }
  void setDedupeRelations(bool dedupe) { _dedupeRelations = dedupe; }
  void setFavorMoreConnectedWays(bool favor) { _favorMoreConnectedWays = favor; }

private:

  // TODO
  bool _dedupeIntraMap;

  // TODO
  bool _dedupeNodes;
  bool _dedupeWays;
  bool _dedupeRelations;

  // TODO
  bool _favorMoreConnectedWays;

  void _validateInputs();

  /*
   * TODO
   */
  void _calcElementHashes(
    OsmMapPtr map, QMap<QString, ElementId>& hashes,
    QSet<std::pair<ElementId, ElementId>>& duplicates);

  void _removeElements(const QSet<ElementId>& elementsToRemove, OsmMapPtr map);
};

}

#endif // ELEMENT_DEDUPLICATOR_H
