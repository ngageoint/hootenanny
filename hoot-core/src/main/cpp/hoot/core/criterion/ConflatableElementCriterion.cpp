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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ConflatableElementCriterion.h"

// hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

QMap<QString, ElementCriterionPtr> ConflatableElementCriterion::_conflatableCriteria;

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
  const QMap<QString, ElementCriterionPtr> conflatableCriteria = getConflatableCriteria();

  QMap<QString, ElementCriterionPtr> conflatableCriteriaForGeometryType;
  for (QMap<QString, ElementCriterionPtr>::const_iterator itr = conflatableCriteria.begin();
       itr != conflatableCriteria.end(); ++itr)
  {
    ElementCriterionPtr crit = itr.value();
    std::shared_ptr<GeometryTypeCriterion> geometryCrit =
      std::dynamic_pointer_cast<GeometryTypeCriterion>(crit);
    if (geometryCrit && geometryCrit->getGeometryType() == geometryType)
    {
      conflatableCriteriaForGeometryType[itr.key()] = crit;
    }
  }
  return conflatableCriteriaForGeometryType;
}

void ConflatableElementCriterion::_createConflatableCriteria()
{
  const std::vector<std::string> criterionClassNames =
    Factory::getInstance().getObjectNamesByBase(ElementCriterion::className());
  LOG_VART(criterionClassNames);
  for (std::vector<std::string>::const_iterator itr = criterionClassNames.begin();
       itr != criterionClassNames.end(); ++itr)
  {
    ElementCriterionPtr crit(Factory::getInstance().constructObject<ElementCriterion>(*itr));
    if (std::dynamic_pointer_cast<ConflatableElementCriterion>(crit) != 0)
    {
      _conflatableCriteria[QString::fromStdString(*itr)] = crit;
    }
  }
  LOG_VART(_conflatableCriteria.size());
}

QStringList ConflatableElementCriterion::getConflatableCriteriaForElement(const ConstElementPtr& e)
{
  const QMap<QString, ElementCriterionPtr> conflatableCriteria = getConflatableCriteria();

  QStringList conflatableCriteriaForElement;
  for (QMap<QString, ElementCriterionPtr>::const_iterator itr = conflatableCriteria.begin();
       itr != conflatableCriteria.end(); ++itr)
  {
    LOG_VART(itr.key());
    if (itr.value()->isSatisfied(e))
    {
      // It is something we can conflate?
      conflatableCriteriaForElement.append(itr.key());
    }
  }
  LOG_VART(conflatableCriteriaForElement);
  return conflatableCriteriaForElement;
}

QStringList ConflatableElementCriterion::getCriterionClassNamesByGeometryType(const GeometryType& type)
{
  QStringList classNamesByType;
  std::vector<std::string> classNames =
    Factory::getInstance().getObjectNamesByBase("hoot::ElementCriterion");
  LOG_VARD(classNamesByType);
  for (size_t i = 0; i < classNames.size(); i++)
  {
    const std::string className = classNames[i];
    LOG_VARD(className);

    ElementCriterionPtr crit(
      Factory::getInstance().constructObject<ElementCriterion>(className));
    std::shared_ptr<ConflatableElementCriterion> conflatableCrit =
      std::dynamic_pointer_cast<ConflatableElementCriterion>(crit);
    if (conflatableCrit)
    {
      LOG_DEBUG("is conflatable: " << className);
      std::shared_ptr<GeometryTypeCriterion> geometryTypeCrit =
        std::dynamic_pointer_cast<GeometryTypeCriterion>(crit);
      if (geometryTypeCrit && geometryTypeCrit->getGeometryType() == type)
      {
        LOG_DEBUG("is same geometry: " << className);
        classNamesByType.append(QString::fromStdString(className));
      }
    }
  }
  return classNamesByType;
}

}
