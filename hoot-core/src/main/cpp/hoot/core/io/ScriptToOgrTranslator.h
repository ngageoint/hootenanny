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
#ifndef SCRIPTTOOGRTRANSLATOR_H
#define SCRIPTTOOGRTRANSLATOR_H

// geos
#include <geos/geom/Geometry.h>

// hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/io/schema/StrictChecking.h>
#include <hoot/core/io/schema/Feature.h>

// Qt
#include <QString>

// Tgs
#include <tgs/SharedPtr.h>

namespace hoot
{
using namespace boost;
using namespace geos::geom;
class Feature;
class Schema;
class Tags;

class ScriptToOgrTranslator
{
public:
  typedef struct TranslatedFeature
  {
  public:
    boost::shared_ptr<Feature> feature;
    QString tableName;

    QString toString() const
    {
      return tableName + ": " + feature->toString();
    }
  } TranslatedFeature;

  virtual boost::shared_ptr<const Schema> getOgrOutputSchema() = 0;

  virtual vector<TranslatedFeature> translateToOgr(Tags& tags, ElementType elementType,
    GeometryTypeId geometryType) = 0;

  virtual vector<Tags> translateToOgrTags(Tags& tags, ElementType elementType,
    GeometryTypeId geometryType) = 0;
};

}

#endif // SCRIPTTOOGRTRANSLATOR_H
