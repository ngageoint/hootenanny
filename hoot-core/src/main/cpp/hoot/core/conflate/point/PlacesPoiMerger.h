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
#ifndef PLACESPOIMERGER_H
#define PLACESPOIMERGER_H

// hoot
#include <hoot/core/conflate/MergerBase.h>
#include <hoot/core/schema/TagMerger.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

class PlacesPoiMerger : public MergerBase, public Configurable
{
public:

  static QString placesTagMerger() { return "places.tag.merger"; }

  PlacesPoiMerger(const set< pair<ElementId, ElementId> >& pairs);

  virtual void apply(const OsmMapPtr& map, vector< pair<ElementId, ElementId> >& replaced)
    const;

  virtual void setConfiguration(const Settings& conf);

protected:
  virtual PairsSet& getPairs() { return _pairs; }
  virtual const PairsSet& getPairs() const { return _pairs; }

private:
  /// @optimize - This could be replaced with a vector to speed "replace" calls.
  set< pair<ElementId, ElementId> > _pairs;
  const TagMerger* _merger;
};

}

#endif // PLACESPOIMERGER_H
