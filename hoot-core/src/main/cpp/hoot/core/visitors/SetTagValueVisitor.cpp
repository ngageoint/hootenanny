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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "SetTagValueVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/criterion/NotCriterion.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ConstElementVisitor, SetTagValueVisitor)

SetTagValueVisitor::SetTagValueVisitor()
{
  setConfiguration(conf());
}

SetTagValueVisitor::SetTagValueVisitor(QString key, QString value, bool appendToExistingValue,
                                       const QString criterionName, bool overwriteExistingTag,
                                       bool negateCriterion) :
_appendToExistingValue(appendToExistingValue),
_overwriteExistingTag(overwriteExistingTag),
_negateCriterion(negateCriterion)
{
  _k.append(key);
  _v.append(value);
  _setCriterion(criterionName);

  LOG_VART(_k);
  LOG_VART(_v);
  LOG_VART(_appendToExistingValue);
  LOG_VART(_overwriteExistingTag);
  LOG_VART(_negateCriterion);
}

void SetTagValueVisitor::setConfiguration(const Settings& conf)
{
  ConfigOptions configOptions(conf);
  _k = configOptions.getSetTagValueVisitorKey();
  _v = configOptions.getSetTagValueVisitorValue();
  if (_k.size() != _v.size())
  {
    throw IllegalArgumentException("set.tag.value.visitor key and value must be the same length.");
  }
  _appendToExistingValue = configOptions.getSetTagValueVisitorAppendToExistingValue();
  _overwriteExistingTag = configOptions.getSetTagValueVisitorOverwrite();
  _negateCriterion = configOptions.getElementCriterionNegate();
  _setCriterion(configOptions.getSetTagValueVisitorElementCriterion());

  LOG_VART(_k);
  LOG_VART(_v);
  LOG_VART(_appendToExistingValue);
  LOG_VART(_overwriteExistingTag);
  LOG_VART(_negateCriterion);
}

void SetTagValueVisitor::addCriterion(const ElementCriterionPtr& e)
{
  if (!_negateCriterion)
  {
    _criterion = e;
  }
  else
  {
    _criterion.reset(new NotCriterion(e));
  }
}

void SetTagValueVisitor::_setCriterion(const QString criterionName)
{
  if (!criterionName.trimmed().isEmpty())
  {
    LOG_VART(criterionName);
    addCriterion(
      boost::shared_ptr<ElementCriterion>(
        Factory::getInstance().constructObject<ElementCriterion>(criterionName.trimmed())));
  }
}

void SetTagValueVisitor::_setTag(const ElementPtr& e, QString k, QString v)
{
  if (k.isEmpty())
  {
    throw IllegalArgumentException("You must set the key in the SetTagValueVisitor class.");
  }
  if (v.isEmpty())
  {
    throw IllegalArgumentException("You must set the value in the SetTagValueVisitor class.");
  }

  LOG_VART(e->getElementId());

  if (_criterion.get() && !_criterion->isSatisfied(*e))
  {
    return;
  }

  LOG_VART(e->getTags().contains(k));
  if (!_overwriteExistingTag && e->getTags().contains(k))
  {
    return;
  }

  if (k == MetadataTags::ErrorCircular())
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
}

void SetTagValueVisitor::visit(const boost::shared_ptr<Element>& e)
{
  for (int i = 0; i < _k.size(); i++)
  {
    _setTag(e, _k[i], _v[i]);
  }
}

}
