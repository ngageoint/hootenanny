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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "RemoveTagsVisitor.h"

// hoot
#include <hoot/core/criterion/NotCriterion.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, RemoveTagsVisitor)

RemoveTagsVisitor::RemoveTagsVisitor()
  : _numTagsRemoved(0)
{
  setConfiguration(conf());
}

RemoveTagsVisitor::RemoveTagsVisitor(const QStringList& keys)
  : _numTagsRemoved(0),
    _negateCriterion(false)
{
  _setKeys(keys);
}

void RemoveTagsVisitor::setConfiguration(const Settings& conf)
{
  ConfigOptions configOptions(conf);
  _setKeys(configOptions.getTagFilterKeys());
  _negateCriterion = configOptions.getElementCriteriaNegate();
  _setCriterion(configOptions.getTagFilterElementCriterion());
}

void RemoveTagsVisitor::_setKeys(const QStringList& keys)
{
  QSet<QString> keysParsed;
  for (const auto& key : qAsConst(keys))
  {
    if (!keysParsed.contains(key))
    {
      keysParsed.insert(key);
      _keyRegexs.append(QRegExp(key, Qt::CaseInsensitive, QRegExp::Wildcard));
    }
  }
}

void RemoveTagsVisitor::addCriterion(const ElementCriterionPtr& e)
{
  if (!_negateCriterion)
    _criterion = e;
  else
    _criterion = std::make_shared<NotCriterion>(e);
}

void RemoveTagsVisitor::_setCriterion(const QString& criterionName)
{
  if (!criterionName.trimmed().isEmpty())
  {
    LOG_VART(criterionName);
    addCriterion(Factory::getInstance().constructObject<ElementCriterion>(criterionName.trimmed()));
  }
}

void RemoveTagsVisitor::visit(const std::shared_ptr<Element>& e)
{
  // see if the element passes the filter (if there is one)
  if (_criterion.get() && !_criterion->isSatisfied(e))
    return;
  _numAffected++;
  _numTagsRemoved += e->getTags().removeKeys(_keyRegexs);
}

}
