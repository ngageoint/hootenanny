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
 * @copyright Copyright (C) 2022, 2023 Maxar (http://www.maxar.com/)
 */
#include "TranslationInterface.h"

// geos
#include <geos/geom/GeometryFactory.h>

// hoot
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/schema/ScriptSchemaTranslator.h>
#include <hoot/core/schema/ScriptToOgrSchemaTranslator.h>
#include <hoot/core/schema/ScriptSchemaTranslatorFactory.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

int TranslationInterface::logWarnCount = 0;

TranslationInterface::TranslationInterface()
  : _strictChecking(StrictOn)
{
}

void TranslationInterface::initTranslator()
{
  if (_scriptPath.isEmpty())
    throw HootException("A script path must be set before the output data source is opened.");
  if (_translator == nullptr)
  {
    // Great bit of code taken from TranslatedTagDifferencer.cpp
    std::shared_ptr<ScriptSchemaTranslator> st = ScriptSchemaTranslatorFactory::getInstance().createTranslator(_scriptPath);
    st->setErrorTreatment(_strictChecking);
    _translator = std::dynamic_pointer_cast<ScriptToOgrSchemaTranslator>(st);
  }
  if (!_translator)
    throw HootException("Error allocating translator, the translation script must support converting to OGR.");
  _schema = _translator->getOgrOutputSchema();
}

void TranslationInterface::translateToFeatures(const ElementProviderPtr& provider, const ConstElementPtr& e,
                                               std::shared_ptr<Geometry> &g, // output
                                               std::vector<ScriptToOgrSchemaTranslator::TranslatedFeature> &tf) const
{
  if (!_translator)
    throw HootException("You must call open before attempting to write.");

  if (e->getTags().getInformationCount() > 0)
  {
    g = convertGeometry(provider, e);
    //  Validate the geometry before using it
    if (!g)
    {
      LOG_DEBUG("Geometry not created for: " << e->getElementId());
      return;
    }
    LOG_TRACE("After conversion to geometry, element is now a " << g->getGeometryType());
    //  Copy the tags and remove any empty values from the tags
    Tags t = e->getTags();
    QStringList keys = t.keys();
    for (const auto& key : qAsConst(keys))
    {
      if (t[key] == "")
        t.remove(key);
    }
    tf = _translator->translateToOgr(t, e->getElementType(), g->getGeometryTypeId());
  }
}

std::shared_ptr<geos::geom::Geometry> TranslationInterface::convertGeometry(const ElementProviderPtr& provider, const ConstElementPtr& e) const
{
  if (e->getTags().getInformationCount() > 0)
  {
    // There is probably a cleaner way of doing this. convertToGeometry calls getGeometryType which
    // will throw an exception if it gets a relation that it doesn't know about. E.g. "route",
    // "superroute", "turnlanes:turns", etc.
    try
    {
      return ElementToGeometryConverter(provider).convertToGeometry(e, false);
    }
    catch (const IllegalArgumentException& err)
    {
      if (logWarnCount < Log::getWarnMessageLimit())
      {
        LOG_WARN("Error converting geometry: " << err.getWhat() << " (" << e->toString() << ")");
      }
      else if (logWarnCount == Log::getWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
    }
  }
  return GeometryFactory::getDefaultInstance()->createEmptyGeometry();
}

}
