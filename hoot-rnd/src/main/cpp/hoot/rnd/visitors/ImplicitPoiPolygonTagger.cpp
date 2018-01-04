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
 */
#include "ImplicitPoiPolygonTagger.h"

#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, ImplicitPoiPolygonTagger)

ImplicitPoiPolygonTagger::ImplicitPoiPolygonTagger() :
ImplicitTaggerBase()
{
  _ruleReader.reset(new ImplicitTagRulesSqliteReader());
  _ruleReader->open(ConfigOptions().getImplicitTaggerPoiRulesDatabase());
}

ImplicitPoiPolygonTagger::ImplicitPoiPolygonTagger(const QString databasePath) :
ImplicitTaggerBase(databasePath)
{
  _ruleReader.reset(new ImplicitTagRulesSqliteReader());
  _ruleReader->open(databasePath);
}

bool ImplicitPoiPolygonTagger::_visitElement(const ElementPtr& e)
{
  const bool elementIsANode = e->getElementType() == ElementType::Node;
  LOG_VART(elementIsANode);
  const bool inABuildingOrPoiCategory =
    OsmSchema::getInstance().getCategories(e->getTags()).intersects(
      OsmSchemaCategory::building() | OsmSchemaCategory::poi());
  _elementIsASpecificPoi =
    OsmSchema::getInstance().hasCategory(e->getTags(), "poi") &&
    !e->getTags().contains("poi") && e->getTags().get("building") != QLatin1String("yes");
  LOG_VART(_elementIsASpecificPoi);
  const bool elementIsAGenericPoi = !_elementIsASpecificPoi;

  if (elementIsAGenericPoi)
  {
    return true;
  }
  else if (_elementIsASpecificPoi && _allowTaggingSpecificPois)
  {
    return true;
  }
  else if (elementIsANode && e->getTags().getNames().size() > 0)
  {
    return true;
  }

  const bool elementIsAWay = e->getElementType() == ElementType::Way;
  LOG_VART(elementIsAWay);
  const bool elementIsAPoly = _polyFilter.isSatisfied(e);
  LOG_VART(elementIsAPoly);
  const bool elementIsASpecificPoly =
    inABuildingOrPoiCategory && e->getTags().get("building") != QLatin1String("yes");
  //TODO: hack
  _elementIsASpecificPoi = elementIsASpecificPoly;
  LOG_VART(_elementIsASpecificPoi);
  const bool elementIsAGenericPoly = !elementIsASpecificPoly;

  if (elementIsAGenericPoly)
  {
    return true;
  }
  else if (elementIsASpecificPoly && _allowTaggingSpecificPois)
  {
    return true;
  }
  else if (elementIsAWay && e->getTags().getNames().size() > 0)
  {
    return true;
  }

  return false;
}

}
