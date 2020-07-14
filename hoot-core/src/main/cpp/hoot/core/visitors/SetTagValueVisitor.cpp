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
#include "SetTagValueVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/schema/MetadataTags.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, SetTagValueVisitor)

SetTagValueVisitor::SetTagValueVisitor() :
_appendToExistingValue(false),
_overwriteExistingTag(false)
{
  setConfiguration(conf());
}

SetTagValueVisitor::SetTagValueVisitor(
  const QStringList& keys, const QStringList& values, bool appendToExistingValue,
  const QStringList& criteriaClassNames, const bool overwriteExistingTag,
  const bool negateCriteria) :
_keys(keys),
_vals(values),
_appendToExistingValue(appendToExistingValue),
_overwriteExistingTag(overwriteExistingTag),
_circularErrorTagKeys(ConfigOptions().getCircularErrorTagKeys())
{
  if (_keys.size() != _vals.size())
  {
    throw IllegalArgumentException(
      "set.tag.value.visitor keys and values must be the same length. Keys size: " +
      QString::number(_keys.size()) + ", values size: " + QString::number(_vals.size()));
  }

  _negateCriteria = negateCriteria;
  _chainCriteria = false;
  _addCriteria(criteriaClassNames);
}

SetTagValueVisitor::SetTagValueVisitor(
  const QString& key, const QString& value, bool appendToExistingValue,
  const QStringList& criteriaClassNames, const bool overwriteExistingTag,
  const bool negateCriteria) :
_appendToExistingValue(appendToExistingValue),
_overwriteExistingTag(overwriteExistingTag),
_circularErrorTagKeys(ConfigOptions().getCircularErrorTagKeys())
{
  _keys.append(key);
  _vals.append(value);

  _negateCriteria = negateCriteria;
  _chainCriteria = false;
  _addCriteria(criteriaClassNames);
}

void SetTagValueVisitor::setConfiguration(const Settings& conf)
{
  ConfigOptions configOptions(conf);

  _keys = configOptions.getSetTagValueVisitorKeys();
  _vals = configOptions.getSetTagValueVisitorValues();
  if (_keys.size() != _vals.size())
  {
    throw IllegalArgumentException(
      "set.tag.value.visitor keys and values must be the same length. Keys size: " +
      QString::number(_keys.size()) + ", values size: " + QString::number(_vals.size()));
  }
  _appendToExistingValue = configOptions.getSetTagValueVisitorAppendToExistingValue();
  _overwriteExistingTag = configOptions.getSetTagValueVisitorOverwrite();

  _negateCriteria = configOptions.getElementCriterionNegate();
  _chainCriteria = configOptions.getSetTagValueVisitorChainElementCriteria();
  const QStringList critNames = configOptions.getSetTagValueVisitorElementCriteria();
  LOG_VART(critNames);
  _addCriteria(critNames);
  if (_configureChildren)
  {
    for (std::vector<ElementCriterionPtr>::const_iterator it = _criteria.begin();
         it != _criteria.end(); ++it)
    {
      ElementCriterionPtr crit = *it;
      Configurable* c = dynamic_cast<Configurable*>(crit.get());
      if (c != 0)
      {
        c->setConfiguration(conf);
      }
    }
  }
  _circularErrorTagKeys = configOptions.getCircularErrorTagKeys();
}

void SetTagValueVisitor::_setTag(const ElementPtr& e, const QString& k, const QString& v)
{
  if (_keys.isEmpty())
  {
    throw IllegalArgumentException("You must set the key in the SetTagValueVisitor class.");
  }
  if (_vals.isEmpty())
  {
    throw IllegalArgumentException("You must set the value in the SetTagValueVisitor class.");
  }

  LOG_VART(e->getElementId());

  if (_criteria.size() > 0 && !_criteriaSatisfied(e))
  {
    return;
  }

  LOG_VART(e->getTags().contains(k));
  if (!_overwriteExistingTag && e->getTags().contains(k))
  {
    return;
  }

  if (_circularErrorTagKeys.contains(k))
  {
    bool ok;
    double vDouble = v.toDouble(&ok);
    if (!ok)
    {
      throw IllegalArgumentException(MetadataTags::ErrorCircular() + " expects a double value.");
    }
    e->setCircularError(vDouble);
  }
  else
  {
    if (_appendToExistingValue)
    {
      e->getTags().appendValue(k, v);
    }
    else
    {
      e->getTags()[k] = v;
    }
    LOG_VART(k);
    LOG_VART(v);
    LOG_VART(e->getTags()[k]);
  }
  _numAffected++;
}

void SetTagValueVisitor::visit(const std::shared_ptr<Element>& e)
{
  for (int i = 0; i < _keys.size(); i++)
  {
    _setTag(e, _keys[i], _vals[i]);
  }
}

}
