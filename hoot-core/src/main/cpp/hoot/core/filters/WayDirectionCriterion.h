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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef WAYDIRECTIONCRITERION_H
#define WAYDIRECTIONCRITERION_H

// GEOS
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/Units.h>
#include <hoot/core/elements/Element.h>
#include <hoot/core/filters/ElementCriterion.h>

#include "WayFilter.h"

namespace hoot
{
  using namespace geos::geom;
  class Way;

class WayDirectionCriterion : public ElementCriterion
{
public:
  WayDirectionCriterion(const ConstOsmMapPtr& map,
                        shared_ptr<const Way> baseWay,
                        bool similarDirection = true);

  virtual bool isSatisfied(const shared_ptr<const Element> &e) const;

  WayDirectionCriterion* clone() { return new WayDirectionCriterion(_map, _baseWay, _similarDirection); }

private:
  ConstOsmMapPtr _map;
  shared_ptr<const Way> _baseWay;
  bool _similarDirection;
};

}

#endif // WAYDIRECTIONCRITERION_H
