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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef MERGERBASE_H
#define MERGERBASE_H

#include <hoot/core/conflate/Merger.h>

namespace hoot
{

class MergerBase : public Merger
{
public:
  static std::string className() { return "hoot::MergerBase"; }

  typedef std::set< std::pair<ElementId, ElementId> > PairsSet;

  MergerBase() {}

  virtual ~MergerBase() {}

  virtual std::set<ElementId> getImpactedElementIds() const;

  virtual std::set< std::pair<ElementId, ElementId> > getImpactedUnknown1ElementIds() const
  {  return std::set< std::pair<ElementId, ElementId> >(); }

  virtual bool isValid(const ConstOsmMapPtr& map) const;

  virtual void replace(ElementId oldEid, ElementId newEid);

  virtual QString toString() const { return QString("Unimplemented toString()"); }

protected:
  virtual PairsSet& getPairs() = 0;
  virtual const PairsSet& getPairs() const = 0;

};

}

#endif // MERGERBASE_H
