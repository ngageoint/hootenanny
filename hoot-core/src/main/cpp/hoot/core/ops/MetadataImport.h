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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef METADATAIMPORT_H
#define METADATAIMPORT_H

// Hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/ops/MetadataOp.h>

// geos
#include <geos/geom/Geometry.h>

namespace hoot
{

class MetadataImport : public MetadataOp
{
public:

  static std::string className() { return "hoot::MetadataImport"; }

  // OsmMapOperation
  virtual QString getDescription() const override { return "Imports metadata"; }

private:

  // MetadataOp
  virtual void _apply();


  bool areMetadataTagsEqual(ElementPtr p1, ElementPtr p2);
  bool applyToElement(ElementPtr pElement, WayPtr pTagSource, std::shared_ptr<geos::geom::Geometry>& geom );
};

}

#endif // METADATAIMPORT_H
