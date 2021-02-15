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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "SchemaTranslationVisitor.h"

#include <vector>

#include <geos/geom/Geometry.h>

#include <hoot/core/elements/Element.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/elements/Tags.h>
#include <hoot/core/schema/ScriptToOgrSchemaTranslator.h>
#include <hoot/core/schema/ScriptSchemaTranslatorFactory.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, SchemaTranslationVisitor)

SchemaTranslationVisitor::SchemaTranslationVisitor() :
_toOgr(false)
{
}

void SchemaTranslationVisitor::setConfiguration(const Settings& conf)
{
  ConfigOptions c(conf);

  LOG_VARD(conf.hasKey(c.getSchemaTranslationScriptKey()));
  LOG_VARD(c.getSchemaTranslationScript());
  if (conf.hasKey(c.getSchemaTranslationScriptKey()) && c.getSchemaTranslationScript() != "")
  {
    setTranslationDirection(c.getSchemaTranslationDirection());
    setTranslationScript(c.getSchemaTranslationScript());
  }
  LOG_VARD(_toOgr);
  _circularErrorTagKeys = c.getCircularErrorTagKeys();
}

void SchemaTranslationVisitor::setTranslationDirection(QString direction)
{
  LOG_VARD(direction);
  if (direction.toLower() == "toogr")
  {
    _toOgr = true;
  }
  else if (direction.toLower() == "toosm")
  {
    _toOgr = false;
  }
  else
  {
    throw HootException("Expected a schema.translation.direction of 'toogr' or 'toosm'.");
  }
}

void SchemaTranslationVisitor::setTranslationScript(QString path)
{
  LOG_VARD(path);
  _translator.reset(ScriptSchemaTranslatorFactory::getInstance().createTranslator(path));
  if (_toOgr)
  {
    _ogrTranslator = dynamic_cast<ScriptToOgrSchemaTranslator*>(_translator.get());
    if (_ogrTranslator == 0)
    {
      throw IllegalArgumentException(
        "Translating to OGR requires a script that supports to OGR translations.");
    }
  }
}

void SchemaTranslationVisitor::visit(const ElementPtr& e)
{
  if (e.get() && e->getTags().getNonDebugCount() > 0)
  {
    Tags& tags = e->getTags();

    GeometryTypeId gtype = ElementToGeometryConverter::getGeometryType(e, false);

    // If we don't know what it is, no point in translating it.
    if (gtype == ElementToGeometryConverter::UNKNOWN_GEOMETRY)
    {
      return;
    }

    if (_toOgr)
    {
      vector<Tags> allTags = _ogrTranslator->translateToOgrTags(tags, e->getElementType(), gtype);

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
      LOG_TRACE("Tags before: " << tags);
      LOG_VART(tags.size());

      QByteArray layerName;
      if (tags.contains(MetadataTags::HootLayername()))
      {
        layerName = tags[MetadataTags::HootLayername()].toUtf8();
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

      _translator->translateToOsm(tags, layerName.data(), geomType);

      // Arbitrarily pick the first error tag found. If the element has both, the last one parsed
      // will be used. We're not expecting elements to have more than one CE tag.
      const QString ceKey = tags.getFirstKey(_circularErrorTagKeys);
      if (!ceKey.isEmpty())
      {
        e->setCircularError(tags.getDouble(ceKey));

        // Preserving original behavior of the class here. Not completely sure why we need to remove
        // them. Removing 'error:circular' seems ok, since its a hoot specific key. Not as sure
        // about 'accuracy', though, as that may be OSM specific.
        tags.remove(MetadataTags::ErrorCircular());
        tags.remove(MetadataTags::Accuracy());
      }

      LOG_TRACE("Tags after: " << tags);
    }

    _numAffected++;
  }
}

}
