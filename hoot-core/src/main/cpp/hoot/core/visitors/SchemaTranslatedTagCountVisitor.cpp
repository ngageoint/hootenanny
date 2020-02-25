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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "SchemaTranslatedTagCountVisitor.h"

// hoot
#include <hoot/core/schema/ScriptToOgrSchemaTranslator.h>
#include <hoot/core/io/schema/Feature.h>
#include <hoot/core/io/schema/FeatureDefinition.h>
#include <hoot/core/io/schema/FieldDefinition.h>
#include <hoot/core/elements/ElementConverter.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/schema/ScriptSchemaTranslator.h>
#include <hoot/core/io/schema/Schema.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, SchemaTranslatedTagCountVisitor)

SchemaTranslatedTagCountVisitor::SchemaTranslatedTagCountVisitor(
  const std::shared_ptr<ScriptSchemaTranslator>& t) :
  _map(),
  _populatedCount(),
  _defaultCount(),
  _nullCount(),
  _taskStatusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval())
{
  _translator = std::dynamic_pointer_cast<ScriptToOgrSchemaTranslator>(t);
  if (!_translator)
  {
    throw HootException("Error allocating translator, the translation script must support "
                        "converting to OGR.");
  }

  _schema = _translator->getOgrOutputSchema();
}

void SchemaTranslatedTagCountVisitor::_countTags(std::shared_ptr<Feature>& f)
{
  const std::shared_ptr<const FeatureDefinition>& defn = f->getFeatureDefinition();

  for (size_t i = 0; i < defn->getFieldCount(); i++)
  {
    std::shared_ptr<const FieldDefinition> fd = defn->getFieldDefinition(i);

    const QVariantMap& vm = f->getValues();

    if (vm.contains(fd->getName()) == false)
    {
      _nullCount++;
    }
    else
    {
      QVariant v = vm[fd->getName()];

      if (v.isNull())
      {
        _nullCount++;
      }
      else if (fd->hasDefaultValue() && fd->getDefaultValue() == v)
      {
        _defaultCount++;
      }
      else
      {
        _populatedCount++;
      }
    }
  }
}

void SchemaTranslatedTagCountVisitor::visit(const ConstElementPtr& e)
{
  if (e->getTags().getInformationCount() > 0)
  {
    std::shared_ptr<Geometry> g =
      ElementConverter(_map->shared_from_this()).convertToGeometry(e, false);

    Tags t = e->getTags();
    t[MetadataTags::ErrorCircular()] = QString::number(e->getCircularError());
    t[MetadataTags::HootStatus()] = e->getStatusString();

    // remove all the empty tags.
    for (Tags::const_iterator it = e->getTags().begin(); it != e->getTags().end(); ++it)
    {
      if (t[it.key()] == "")
      {
        t.remove(it.key());
      }
    }

    vector<ScriptToOgrSchemaTranslator::TranslatedFeature> f = _translator->translateToOgr(t,
      e->getElementType(), g->getGeometryTypeId());

    // only write the feature if it wasn't filtered by the translation script.
    for (size_t i = 0; i < f.size(); i++)
    {
      _countTags(f[i].feature);
    }
    _numAffected++;
  }
  _numProcessed++;

  if (_numProcessed % _taskStatusUpdateInterval == 0)
  {
    PROGRESS_INFO(
      "Processed  " <<  StringUtils::formatLargeNumber(_numProcessed) << " features...");
  }
}

}
