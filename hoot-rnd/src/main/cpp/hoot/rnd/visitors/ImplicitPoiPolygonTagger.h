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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 * @copyright Copyright (C) 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef IMPLICIT_POI_POLYGON_TAGGER_H
#define IMPLICIT_POI_POLYGON_TAGGER_H

// hoot
#include <hoot/core/visitors/ImplicitTaggerBase.h>
#include <hoot/core/conflate/poi-polygon/filters/PoiPolygonPoiCriterion.h>
#include <hoot/core/conflate/poi-polygon/filters/PoiPolygonPolyCriterion.h>

namespace hoot
{

/**
 * Adds tags implicitly derived from POI names to POIs and polygons
 */
class ImplicitPoiPolygonTagger : public ImplicitTaggerBase
{
public:

  static std::string className() { return "hoot::ImplicitPoiPolygonTagger"; }

  ImplicitPoiPolygonTagger();
  ImplicitPoiPolygonTagger(const QString databasePath);

  virtual QString getName() const { return "Implicit POI/Polygon Tagger"; }

  virtual QString getDescription() const
  { return "Adds tags to POIs and polygons implicitly derived from their names"; }

protected:

  virtual bool _visitElement(const ElementPtr& e);

private:

  PoiPolygonPoiCriterion _poiFilter;
  PoiPolygonPolyCriterion _polyFilter;
  bool _inABuildingOrPoiCategory;

  bool _elementIsATaggablePoi(const ElementPtr& e);
  bool _elementIsATaggablePolygon(const ElementPtr& e);

};

}

#endif // IMPLICIT_POI_POLYGON_TAGGER_H
