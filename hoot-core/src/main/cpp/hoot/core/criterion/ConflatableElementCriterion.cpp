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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "ConflatableElementCriterion.h"

// hoot
#include <hoot/core/elements/ConstOsmMapConsumer.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

QMap<QString, ElementCriterionPtr> ConflatableElementCriterion::_conflatableCriteria;
QMap<QString, QMap<QString, ElementCriterionPtr>> ConflatableElementCriterion::_conflatableCriteriaByGeometryType;

QMap<QString, ElementCriterionPtr> ConflatableElementCriterion::getConflatableCriteria()
{
  LOG_VART(_conflatableCriteria.isEmpty());
  if (_conflatableCriteria.isEmpty())
  {
    _createConflatableCriteria();
  }
  LOG_VART(_conflatableCriteria.size());
  return _conflatableCriteria;
}

QMap<QString, ElementCriterionPtr> ConflatableElementCriterion::getConflatableCriteria(
  const GeometryType& geometryType)
{
  const QString geometryTypeStr = GeometryTypeCriterion::typeToString(geometryType);
  if (!_conflatableCriteriaByGeometryType[geometryTypeStr].isEmpty())
  {
    return _conflatableCriteriaByGeometryType[geometryTypeStr];
  }
  else
  {
    const QMap<QString, ElementCriterionPtr> conflatableCriteria = getConflatableCriteria();
    for (QMap<QString, ElementCriterionPtr>::const_iterator itr = conflatableCriteria.begin();
         itr != conflatableCriteria.end(); ++itr)
    {
      ElementCriterionPtr crit = itr.value();
      std::shared_ptr<GeometryTypeCriterion> geometryCrit =
        std::dynamic_pointer_cast<GeometryTypeCriterion>(crit);
      if (geometryCrit && geometryCrit->getGeometryType() == geometryType)
      {
        _conflatableCriteriaByGeometryType[geometryTypeStr][itr.key()] = itr.value();
      }
    }
    return _conflatableCriteriaByGeometryType[geometryTypeStr];
  }
}

void ConflatableElementCriterion::_createConflatableCriteria()
{
  const std::vector<QString> criterionClassNames =
    Factory::getInstance().getObjectNamesByBase(ElementCriterion::className());
  LOG_VART(criterionClassNames);
  for (std::vector<QString>::const_iterator itr = criterionClassNames.begin();
       itr != criterionClassNames.end(); ++itr)
  {
    ElementCriterionPtr crit(Factory::getInstance().constructObject<ElementCriterion>(*itr));
    if (std::dynamic_pointer_cast<ConflatableElementCriterion>(crit) != 0)
    {
      _conflatableCriteria[*itr] = crit;
    }
  }
  LOG_VART(_conflatableCriteria.size());
}

QStringList ConflatableElementCriterion::getConflatableCriteriaForElement(
  const ConstElementPtr& e, ConstOsmMapPtr map)
{
  const QMap<QString, ElementCriterionPtr> conflatableCriteria = getConflatableCriteria();

  QStringList conflatableCriteriaForElement;
  QMap<QString, std::shared_ptr<ConflatableElementCriterion>> genericConflatableCriteria;

  // First, go through and mark only with those crits that support specific conflation.

  for (QMap<QString, ElementCriterionPtr>::const_iterator itr = conflatableCriteria.begin();
       itr != conflatableCriteria.end(); ++itr)
  {
    LOG_VART(itr.key());
    ElementCriterionPtr crit = itr.value();

    ConstOsmMapConsumer* mapConsumer = dynamic_cast<ConstOsmMapConsumer*>(crit.get());
    if (mapConsumer != 0)
    {
      mapConsumer->setOsmMap(map.get());
    }

    std::shared_ptr<ConflatableElementCriterion> conflatableCrit =
      std::dynamic_pointer_cast<ConflatableElementCriterion>(crit);
    assert(conflatableCrit);
    if (!conflatableCrit->supportsSpecificConflation())
    {
      genericConflatableCriteria[itr.key()] = conflatableCrit;
      continue;
    }

    if (crit->isSatisfied(e))
    {
      conflatableCriteriaForElement.append(itr.key());
    }
  }
  LOG_VART(conflatableCriteriaForElement);

  // If no specific conflate crit was available, then try to add a generic one.

  if (conflatableCriteriaForElement.empty())
  {
    for (QMap<QString, std::shared_ptr<ConflatableElementCriterion>>::const_iterator itr =
           genericConflatableCriteria.begin();
         itr != genericConflatableCriteria.end(); ++itr)
    {
      LOG_VART(itr.key());
      std::shared_ptr<ConflatableElementCriterion> crit = itr.value();

      if (crit->isSatisfied(e))
      {
        // It is something we can conflate?
        conflatableCriteriaForElement.append(itr.key());
      }
    }
    LOG_VART(conflatableCriteriaForElement);
  }

  return conflatableCriteriaForElement;
}

QStringList ConflatableElementCriterion::getCriterionClassNamesByGeometryType(
  const GeometryType& type)
{
  QStringList classNamesByType;
  std::vector<QString> classNames =
    Factory::getInstance().getObjectNamesByBase(ElementCriterion::className());
  LOG_VART(classNamesByType);
  for (size_t i = 0; i < classNames.size(); i++)
  {
    const QString className = classNames[i];
    LOG_VART(className);

    ElementCriterionPtr crit(
      Factory::getInstance().constructObject<ElementCriterion>(className));
    std::shared_ptr<ConflatableElementCriterion> conflatableCrit =
      std::dynamic_pointer_cast<ConflatableElementCriterion>(crit);
    if (conflatableCrit)
    {
      LOG_TRACE("is conflatable: " << className);
      std::shared_ptr<GeometryTypeCriterion> geometryTypeCrit =
        std::dynamic_pointer_cast<GeometryTypeCriterion>(crit);
      if (geometryTypeCrit && geometryTypeCrit->getGeometryType() == type)
      {
        LOG_TRACE("is same geometry: " << className);
        classNamesByType.append(className);
      }
    }
  }
  return classNamesByType;
}

bool ConflatableElementCriterion::supportsSpecificConflation(const QString& criterionClassName)
{
  ElementCriterionPtr crit = getConflatableCriteria()[criterionClassName];
  if (crit)
  {
    std::shared_ptr<ConflatableElementCriterion> conflatableCrit =
      std::dynamic_pointer_cast<ConflatableElementCriterion>(crit);
    if (conflatableCrit)
    {
      return conflatableCrit->supportsSpecificConflation();
    }
  }
  return false;
}

}
