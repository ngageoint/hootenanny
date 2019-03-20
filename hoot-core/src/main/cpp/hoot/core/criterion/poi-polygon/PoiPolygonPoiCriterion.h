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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef POI_POLYGON_POI_CRITERION_H
#define POI_POLYGON_POI_CRITERION_H

// hoot
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/conflate/address/AddressParser.h>

// Qt
#include <QStringList>

namespace hoot
{

/**
 * Identifies POIs for use with POI/Polygon conflation
 */
class PoiPolygonPoiCriterion : public ElementCriterion
{
public:

  static std::string className() { return "hoot::PoiPolygonPoiCriterion"; }

  PoiPolygonPoiCriterion();

  virtual bool isSatisfied(const ConstElementPtr& e) const override;

  virtual ElementCriterionPtr clone() { return ElementCriterionPtr(new PoiPolygonPoiCriterion()); }

  virtual QString getDescription() const { return ""; }

private:

  QStringList _tagIgnoreList;
  AddressParser _addressParser;
};

}

#endif // POI_POLYGON_POI_CRITERION_H
