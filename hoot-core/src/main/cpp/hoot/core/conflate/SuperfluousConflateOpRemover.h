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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef SUPERFLUOUS_CONFLATE_OP_REMOVER_H
#define SUPERFLUOUS_CONFLATE_OP_REMOVER_H

// Qt
#include <QStringList>

namespace hoot
{

/**
 * This is used to weed down the list of conflate pre/post ops to only those needed given the
 * matchers being run for the conflate job, which helps with both runtime performance and conflated
 * output quality.
 *
 * So for instance, you don't try to split road intersections if you're only conflating buildings.
 * Only GeometryTypeCriterion are checked via FilteredByGeometryTypeCriteria. Not sure if this can
 * be extended to check supported matchers for given other types of criteria.
 */
class SuperfluousConflateOpRemover
{
public:

  /**
   * Changes the global state of conflate pre/post ops by removing any that are unnecessary for the
   * current conflate configuration
   */
  static void removeSuperfluousOps();

  /**
   * Determines GeometryTypeCriterion compatible with conflate matches in the current configuration
   *
   * @return a list of GeometryTypeCriterion class names
   */
  static QSet<QString> getMatchCreatorGeometryTypeCrits();

private:

  // cache supported GeometryTypeCriterion class names to avoid instantiating them unnecessarily
  // when checking multiple ops
  static QSet<QString> _geometryTypeClassNameCache;

  /*
   * Given a list of supported geometry type criteria, remove all conflate ops (map ops and
   * visitors) that don't support the associated geometry
   *
   * @param geometryTypeCrits a list of all ElementCriterion class names associated with the current
   * matcher config
   * @param ops a list of operator class names to be filtered
   * @param removedOps a list of the operators removed
   * @return the input operator list with unnecessary ops filtered out
   */
  static QStringList _filterOutUnneededOps(
    const QSet<QString>& geometryTypeCrits, const QStringList& ops, QSet<QString>& removedOps);

  static bool _isGeometryTypeCrit(const QString& className);
};

}

#endif // SUPERFLUOUS_CONFLATE_OP_REMOVER_H

