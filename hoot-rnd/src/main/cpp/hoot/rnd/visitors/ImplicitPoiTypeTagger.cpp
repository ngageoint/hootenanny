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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "ImplicitPoiTypeTagger.h"

#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, ImplicitPoiTypeTagger)

ImplicitPoiTypeTagger::ImplicitPoiTypeTagger(const QString& databasePath) :
ImplicitTypeTaggerBase(databasePath)
{
}

bool ImplicitPoiTypeTagger::_visitElement(const ElementPtr& e)
{
  LOG_VART(e);
  const bool elementIsANode = e->getElementType() == ElementType::Node;
  LOG_VART(elementIsANode);
  _elementIsASpecificFeature =
    OsmSchema::getInstance().hasCategory(e->getTags(), "poi") && !e->getTags().contains("poi") &&
     e->getTags().get("building") != QLatin1String("yes") &&
     e->getTags().get("office") != QLatin1String("yes");
  LOG_VART(_elementIsASpecificFeature);
  const bool elementIsAGenericPoi = !_elementIsASpecificFeature;
  LOG_VART(elementIsAGenericPoi);
  LOG_VART(_getNames(e->getTags()).size());

  //always allow generic elements
  if (elementIsAGenericPoi)
  {
    return true;
  }
  //allowing specific elements is configurable
  else if (_elementIsASpecificFeature && _allowTaggingSpecificFeatures)
  {
    return true;
  }
  else if (elementIsANode && _getNames(e->getTags()).size() > 0)
  {
    return true;
  }

  return false;
}

}
