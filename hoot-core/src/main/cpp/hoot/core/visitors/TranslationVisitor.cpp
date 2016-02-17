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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include <vector>

#include <boost/shared_ptr.hpp>

#include <geos/geom/Geometry.h>

#include <hoot/core/elements/Element.h>
#include <hoot/core/elements/Tags.h>
#include <hoot/core/io/ScriptToOgrTranslator.h>
#include <hoot/core/io/ScriptTranslator.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>


#include "TranslationVisitor.h"

namespace hoot
{

TranslationVisitor::TranslationVisitor(ScriptTranslator& t, bool toOgr, OsmMap* map) : _t(t),
  _map(map), _toOgr(toOgr)
{
  _circularErrorKey = "error:circular";
  _accuracyKey = "accuracy";

  if (toOgr)
  {
    _togr = dynamic_cast<ScriptToOgrTranslator*>(&t);
    if (_togr == 0)
    {
      throw HootException("Translating to OGR requires a script that supports to OGR "
        "translations.");
    }
  }
}

void TranslationVisitor::visit(const ConstElementPtr& ce)
{
  // this is a hack to get around the visitor interface. The visitor interface should probably be
  // redesigned into ConstElementVisitor and ElementVisitor.
  ElementPtr e = _map->getElement(ce->getElementId());
  Tags& tags = e->getTags();

  if (tags.getNonDebugCount() > 0)
  {
    GeometryTypeId gtype = ElementConverter::getGeometryType(e, false);

    if (_toOgr)
    {

      vector<Tags> allTags = _togr->translateToOgrTags(tags, e->getElementType(), gtype);

      if (allTags.size() > 0)
      {
        if (allTags.size() > 1)
        {
          LOG_WARN("More than one feature was returned, only keeping the first feature.");
        }

        e->setTags(allTags[0]);
      }
    }
    else
    {
      QByteArray layerName;
      if (tags.contains(OsmSchema::layerNameKey()))
      {
        layerName = tags[OsmSchema::layerNameKey()].toUtf8();
      }

      QByteArray geomType;

      switch (gtype)
      {
      case GEOS_POINT:
      case GEOS_MULTIPOINT:
        geomType = "Point";
        break;
      case GEOS_LINESTRING:
      case GEOS_MULTILINESTRING:
        geomType = "Line";
        break;
      case GEOS_POLYGON:
      case GEOS_MULTIPOLYGON:
        geomType = "Area";
        break;
      case GEOS_GEOMETRYCOLLECTION:
        geomType = "Collection";
        break;
      default:
        throw InternalErrorException("Unexpected geometry type.");
      }

      _t.translateToOsm(tags, layerName.data(), geomType);

      if (tags.contains(_circularErrorKey))
      {
        e->setCircularError(tags.getDouble(_circularErrorKey));
        tags.remove(_circularErrorKey);
        tags.remove(_accuracyKey);
      }
      else if (tags.contains(_accuracyKey))
      {
        e->setCircularError(tags.getDouble(_accuracyKey));
        tags.remove(_circularErrorKey);
        tags.remove(_accuracyKey);
      }
    }
  }
}

}
