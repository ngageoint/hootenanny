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
#include "ImplicitPoiTagger.h"

#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, ImplicitPoiTagger)

ImplicitPoiTagger::ImplicitPoiTagger() :
ImplicitTaggerBase()
{
  _ruleReader.reset(new ImplicitTagRulesSqliteReader());
  _ruleReader->open(ConfigOptions().getImplicitTaggerRulesDatabase());
}

ImplicitPoiTagger::ImplicitPoiTagger(const QString databasePath) :
ImplicitTaggerBase(databasePath)
{
  _ruleReader.reset(new ImplicitTagRulesSqliteReader());
  _ruleReader->open(databasePath);
}

bool ImplicitPoiTagger::_visitElement(const ElementPtr& e)
{
  const bool elementIsANode = e->getElementType() == ElementType::Node;
  _elementIsASpecificFeature =
    OsmSchema::getInstance().hasCategory(e->getTags(), "poi") && !e->getTags().contains("poi") &&
     e->getTags().get("building") != QLatin1String("yes") &&
     e->getTags().get("office") != QLatin1String("yes");
  const bool elementIsAGenericPoi = !_elementIsASpecificFeature;

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
  else if (elementIsANode && e->getTags().getNames().size() > 0)
  {
    return true;
  }

  return false;
}

}
