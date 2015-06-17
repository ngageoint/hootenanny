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
 * @copyright Copyright (C) 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef CUSTOMPOIMERGER_H
#define CUSTOMPOIMERGER_H

// hoot
#include <hoot/core/conflate/Merger.h>

namespace hoot
{

class CustomPoiMerger : public Merger
{
public:
  CustomPoiMerger(const set< pair<ElementId, ElementId> >& pairs);

  virtual void apply(const OsmMapPtr& map, vector< pair<ElementId, ElementId> >& replaced)
    const;

  virtual set<ElementId> getImpactedElementIds() const;

  virtual bool isValid(const ConstOsmMapPtr& map) const;

  virtual void replace(ElementId oldEid, ElementId newEid);

  virtual QString toString() const { return QString("CustomPoiMerger"); }

private:
  /// @optimize - This could be replaced with a vector to speed "replace" calls.
  set< pair<ElementId, ElementId> > _pairs;
};

}

#endif // CUSTOMPOIMERGER_H
