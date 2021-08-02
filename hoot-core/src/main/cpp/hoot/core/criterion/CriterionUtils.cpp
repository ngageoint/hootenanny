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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "CriterionUtils.h"

// hoot
#include <hoot/core/criterion/NotCriterion.h>
#include <hoot/core/criterion/OrCriterion.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

ElementCriterionPtr CriterionUtils::constructCriterion(
  QStringList& criteriaClassNames, const bool chainCriteria, const bool negate)
{
  bool isStreamable = false; // This gets ignored.
  return constructCriterion(criteriaClassNames, chainCriteria, negate, isStreamable);
}

ElementCriterionPtr CriterionUtils::constructCriterion(
  QStringList& criteriaClassNames, const bool chainCriteria, const bool negate,
  bool& isStreamable)
{
  if (criteriaClassNames.isEmpty())
  {
    isStreamable = true;
    return ElementCriterionPtr();
  }

  for (int i = 0; i < criteriaClassNames.size(); i++)
  {
    if (!criteriaClassNames.at(i).startsWith(MetadataTags::HootNamespacePrefix()))
    {
      QString className = criteriaClassNames[i];
      className.prepend(MetadataTags::HootNamespacePrefix());
      criteriaClassNames[i] = className;
    }
  }

  ChainCriterionPtr crit;

  if (criteriaClassNames.size() > 1)
  {
    if (chainCriteria)
    {
      crit = std::make_shared<ChainCriterion>();
    }
    else
    {
      crit = std::make_shared<OrCriterion>();
    }
  }

  ElementCriterionPtr subCrit;
  isStreamable = true;
  for (int i = 0; i < criteriaClassNames.size(); i++)
  {
    const QString criterionClassName = criteriaClassNames.at(i);
    LOG_VART(criterionClassName);
    try
    {
      subCrit = Factory::getInstance().constructObject<ElementCriterion>(criterionClassName);
    }
    catch (const boost::bad_any_cast&)
    {
      throw IllegalArgumentException("Invalid criterion: " + criterionClassName);
    }

    const OsmMapConsumer* omc = dynamic_cast<OsmMapConsumer*>(subCrit.get());
    LOG_VART(omc == 0);
    if (omc)
    {
      isStreamable = false;
    }
    LOG_VART(isStreamable);

    if (negate)
    {
      subCrit = std::make_shared<NotCriterion>(subCrit);
    }
    LOG_VART(subCrit.get());

    std::shared_ptr<Configurable> critConfig;
    if (subCrit.get())
    {
      critConfig = std::dynamic_pointer_cast<Configurable>(subCrit);
    }
    LOG_VART(critConfig.get());
    if (critConfig.get())
    {
      critConfig->setConfiguration(conf());
    }

    if (crit)
    {
      crit->addCriterion(subCrit);
    }
  }

  if (criteriaClassNames.size() == 1)
  {
    return subCrit;
  }
  else
  {
    return crit;
  }
}

ElementCriterionPtr CriterionUtils::combineCriterion(
  const QList<ElementCriterionPtr>& criteria, const bool chain, const bool negate)
{
  ElementCriterionPtr combinedCrit;

  for (QList<ElementCriterionPtr>::const_iterator itr = criteria.begin(); itr != criteria.end();
       ++itr)
  {
    if (!combinedCrit)
    {
      combinedCrit = (*itr)->clone();
    }
    else
    {
      if (chain)
      {
        combinedCrit = std::make_shared<ChainCriterion>(combinedCrit->clone(), *itr);
      }
      else
      {
        combinedCrit = std::make_shared<OrCriterion>(combinedCrit->clone(), *itr);
      }
    }
  }

  if (negate)
  {
    combinedCrit = std::make_shared<NotCriterion>(combinedCrit->clone());
  }

  return combinedCrit;
}

}
