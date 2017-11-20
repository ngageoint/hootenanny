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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef IMPLICIT_TAG_ELIGIBLE_POI_CRITERION_H
#define IMPLICIT_TAG_ELIGIBLE_POI_CRITERION_H

// hoot
#include <hoot/core/filters/ElementCriterion.h>
#include <hoot/core/elements/Tags.h>

// Qt
#include <QStringList>


namespace hoot
{

/**
 * A criterion that is only satisified with POIs.
 */
class ImplicitTagEligiblePoiCriterion : public ElementCriterion
{
public:

  static std::string className() { return "hoot::ImplicitTagEligiblePoiCriterion"; }

  ImplicitTagEligiblePoiCriterion() {}

  virtual bool isSatisfied(const boost::shared_ptr<const Element>& e) const;

  virtual boost::shared_ptr<ElementCriterion> clone()
  { return  boost::shared_ptr<ElementCriterion>(new ImplicitTagEligiblePoiCriterion()); }

  static QStringList getPoiKvps(const Tags& tags);
  static bool hasPoiKvp(const Tags& tags);

};

}
#endif
