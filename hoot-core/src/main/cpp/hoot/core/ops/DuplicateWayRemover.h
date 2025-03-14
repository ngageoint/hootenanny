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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#ifndef DUPLICATEWAYREMOVER_H
#define DUPLICATEWAYREMOVER_H

// Hoot
#include <hoot/core/conflate/ConflateInfoCacheConsumer.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/OsmMapOperation.h>

namespace hoot
{
/**
 * Searches for multiple ways that share two or more consecutive nodes if they are found, then
 * it attempts to remove the duplicate bits of the longer way. If the lines partially overlap with
 * exactly the same geometry then only the partial overlap is removed from the more complex
 * geometry. There may be some scenarios that this is destructive, but in most cases it is probably
 * useful. This routine also doesn't handle the case where two ways have multiple overlapping
 * sections. In that case, only the longest duplicate section will be removed.
 *
 * Area ways are ignored.
 */
class DuplicateWayRemover : public OsmMapOperation, public ConflateInfoCacheConsumer
{
public:

  static QString className() { return "DuplicateWayRemover"; }

  DuplicateWayRemover();
  ~DuplicateWayRemover() override = default;

  void apply(OsmMapPtr& map) override;

  /**
   * Remove parts of ways that are duplicates.
   */
  static void removeDuplicates(OsmMapPtr map);

  QString getInitStatusMessage() const override { return "Removing duplicate ways..."; }
  QString getCompletedStatusMessage() const override
  { return "Removed " + QString::number(_numAffected) + " duplicate ways"; }

  /**
   * @see FilteredByGeometryTypeCriteria
   */
  QStringList getCriteria() const override;

  QString getDescription() const override { return "Removes duplicate ways from a map"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

  void setConflateInfoCache(const std::shared_ptr<ConflateInfoCache>& cache) override
  { _conflateInfoCache = cache; }

  /**
   * see duplicate.way.remover.strict.tag.matching
   */
  bool getStrictTagMatching() const { return _strictTagMatching; }

  /**
   * see duplicate.way.remover.strict.tag.matching
   */
  void setStrictTagMatching(bool strictTagMatching)
  {
    _strictTagMatching = strictTagMatching;
  }

private:

  OsmMapPtr _map;

  bool _strictTagMatching;

  // Existence of this cache tells us that elements must be individually checked to see that they
  // are conflatable given the current configuration before modifying them.
  std::shared_ptr<ConflateInfoCache> _conflateInfoCache;

  bool _isCandidateWay(const ConstWayPtr& w) const;
  void _splitDuplicateWays(WayPtr w1, WayPtr w2, bool rev1 = false, bool rev2 = false);
  std::vector<WayPtr> _splitWay(WayPtr w, int start, int length, bool newIds = false) const;
  WayPtr _getUpdatedWay(WayPtr way, const std::vector<long>& nodes, bool newIds) const;
  void _replaceMultiple(const ConstWayPtr& oldWay, const std::vector<WayPtr>& ways);
};

}

#endif // DUPLICATEWAYREMOVER_H
