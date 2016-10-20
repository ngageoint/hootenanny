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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef MARKFORREVIEWMERGER_H
#define MARKFORREVIEWMERGER_H

// hoot
#include <hoot/core/conflate/Merger.h>

namespace hoot
{

/**
 * Marks all the specified pairs with review tags using ReviewMarker.
 */
class MarkForReviewMerger : public Merger
{
public:

  /**
   * Constructed with a set of element matching pairs. The pairs are generally Unknown1 as first
   * and Unknown2 as second.
   */
  MarkForReviewMerger(const set< pair<ElementId, ElementId> >& pairs, QString note,
    QString reviewType, double score);

  /**
   * Review when a whole group of elements needs to be reviewed instead of specific pairs. This
   * will create a single review that contains all the specified elements.
   */
  MarkForReviewMerger(const set<ElementId>& eids, QString note,
    QString reviewType, double score);

  virtual void apply(const OsmMapPtr& map, vector< pair<ElementId, ElementId> >& replaced)
    const;

  virtual set<ElementId> getImpactedElementIds() const;

  virtual bool isValid(const ConstOsmMapPtr& map) const;

  virtual void replace(ElementId oldEid, ElementId newEid);

  virtual QString toString() const;

private:
  set<ElementId> _eids;
  set< pair<ElementId, ElementId> > _pairs;
  QString _note;
  QString _reviewType;
  double _score;

};

}

#endif // MARKFORREVIEWMERGER_H
