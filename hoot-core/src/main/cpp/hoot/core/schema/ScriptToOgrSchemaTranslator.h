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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef SCRIPT_TO_OGR_SCHEMA_TRANSLATOR_H
#define SCRIPT_TO_OGR_SCHEMA_TRANSLATOR_H

// geos
#include <geos/geom/Geometry.h>

// hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/io/schema/StrictChecking.h>
#include <hoot/core/io/schema/Feature.h>

namespace hoot
{

class Schema;
class Tags;

class ScriptToOgrSchemaTranslator
{
public:

  struct TranslatedFeature
  {
  public:

    std::shared_ptr<Feature> feature;
    QString tableName;

    QString toString() const
    {
      return tableName + ": " + feature->toString();
    }
  };

  ScriptToOgrSchemaTranslator() = default;
  virtual ~ScriptToOgrSchemaTranslator() = default;

  virtual std::shared_ptr<const Schema> getOgrOutputSchema() = 0;

  virtual std::vector<TranslatedFeature> translateToOgr(
    Tags& tags, ElementType elementType, geos::geom::GeometryTypeId geometryType) = 0;

  virtual std::vector<Tags> translateToOgrTags(
    Tags& tags, ElementType elementType, geos::geom::GeometryTypeId geometryType) = 0;
};

using ScriptToOgrSchemaTranslatorPtr = std::shared_ptr<ScriptToOgrSchemaTranslator>;
using ConstScriptToOgrSchemaTranslatorPtr = std::shared_ptr<const ScriptToOgrSchemaTranslator>;
using TranslatedFeatureVector = std::vector<ScriptToOgrSchemaTranslator::TranslatedFeature>;

}

#endif // SCRIPT_TO_OGR_SCHEMA_TRANSLATOR_H
