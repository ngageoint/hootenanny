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
 * @copyright Copyright (C) 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "SchemaUtils.h"

// Hoot
#include <hoot/core/visitors/ElementCountVisitor.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/criterion/HasTypeCriterion.h>
#include <hoot/core/io/IoUtils.h>

namespace hoot
{

bool SchemaUtils::anyElementsHaveType(const ConstOsmMapPtr& map)
{
  return
    (int)FilteredVisitor::getStat(
      std::make_shared<HasTypeCriterion>(), std::make_shared<ElementCountVisitor>(), map) > 0;
}

void SchemaUtils::validateTranslationUrl(const QString& url)
{
  QFileInfo fileInfo(url);
  if (!fileInfo.exists())
  {
    throw IllegalArgumentException("Translation file does not exist: " + url);
  }
  else if (!url.endsWith(".js") && !url.endsWith(".py"))
  {
    throw IllegalArgumentException("Invalid translation file format: " + url);
  }
}

QString SchemaUtils::outputFormatToTranslationDirection(const QString& output)
{
  if (IoUtils::isSupportedOgrFormat(output, true))
  {
    LOG_INFO("No translation direction specified. Assuming 'toogr' based on output format...");
    return "toogr";
  }
  else if (IoUtils::isSupportedOsmFormat(output))
  {
    LOG_INFO("No translation direction specified. Assuming 'toosm' based on output format...");
    return "toosm";
  }
  else
  {
    LOG_INFO("No translation direction specified. Using 'toosm'...");
    return "toosm";
  }
}

}
