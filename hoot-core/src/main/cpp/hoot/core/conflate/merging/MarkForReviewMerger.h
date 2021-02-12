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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef MARKFORREVIEWMERGER_H
#define MARKFORREVIEWMERGER_H

// hoot
#include <hoot/core/conflate/merging/Merger.h>
#include <hoot/core/conflate/review/ReviewMarker.h>

namespace hoot
{

/**
 * Marks all the specified pairs with review tags using ReviewMarker.
 *
 * @todo Should this inherit MergerBase instead?
 */
class MarkForReviewMerger : public Merger
{
public:

  static QString className() { return "hoot::MarkForReviewMerger"; }

  MarkForReviewMerger() = default;
  virtual ~MarkForReviewMerger() = default;

  /**
   * Constructed with a set of element matching pairs. The pairs are generally Unknown1 as first
   * and Unknown2 as second.
   */
  MarkForReviewMerger(const std::set<std::pair<ElementId, ElementId>>& pairs, QString note,
                      QString reviewType, double score);

  /**
   * Review when a whole group of elements needs to be reviewed instead of specific pairs. This
   * will create a single review that contains all the specified elements.
   */
  MarkForReviewMerger(const std::set<ElementId>& eids, QString note,
                      QString reviewType, double score);

  virtual void apply(const OsmMapPtr& map,
                     std::vector<std::pair<ElementId, ElementId>>& replaced) override;

  virtual std::set<ElementId> getImpactedElementIds() const override;

  virtual bool isValid(const ConstOsmMapPtr& map) const;

  virtual void replace(ElementId oldEid, ElementId newEid);

  virtual QString toString() const override;

  virtual QString getDescription() const
  { return "Marks elements as needing review before merging"; }

  virtual QString getName() const { return className(); }

  virtual QString getClassName() const override { return className(); }

private:

  std::set<ElementId> _eids;
  std::set<std::pair<ElementId, ElementId>> _pairs;
  QString _note;
  QString _reviewType;
  double _score;
  ReviewMarker _reviewMarker;
};

}

#endif // MARKFORREVIEWMERGER_H
