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
 * @copyright Copyright (C) 2015, 2017, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "ConfigUtils.h"

// Hoot
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/visitors/ApiTagTruncateVisitor.h>
#include <hoot/core/ops/DuplicateNodeRemover.h>

namespace hoot
{

bool ConfigUtils::boundsOptionEnabled()
{
  return
    !conf().get(ConfigOptions::getConvertBoundingBoxKey()).toString().trimmed().isEmpty() ||
    !conf().get(ConfigOptions::getConvertBoundingBoxHootApiDatabaseKey()).toString().trimmed().isEmpty() ||
    !conf().get(ConfigOptions::getConvertBoundingBoxOsmApiDatabaseKey()).toString().trimmed().isEmpty();
}

void ConfigUtils::checkForTagValueTruncationOverride()
{
  // Disable tag truncation if the override option is in place.
  if (ConfigOptions().getConflateTagDisableValueTruncation())
  {
    QStringList conflatePreOps = ConfigOptions().getConflatePreOps();
    int numFound =
      conflatePreOps.removeAll(QString::fromStdString(ApiTagTruncateVisitor::className()));
    if (numFound > 0)
    {
      conf().set("conflate.pre.ops", conflatePreOps);
    }
    QStringList conflatePostOps = ConfigOptions().getConflatePostOps();
    numFound =
      conflatePostOps.removeAll(QString::fromStdString(ApiTagTruncateVisitor::className()));
    if (numFound > 0)
    {
      conf().set("conflate.post.ops", conflatePostOps);
    }
  }
}

void ConfigUtils::checkForDuplicateElementCorrectionMismatch(const QStringList& ops)
{
  const QString dupeNodeRemoverClassName =
    QString::fromStdString(DuplicateNodeRemover::className());
  if (ops.contains(dupeNodeRemoverClassName))
  {
    conf().set(ConfigOptions::getMapMergeIgnoreDuplicateIdsKey(), true);
  }
}

}
