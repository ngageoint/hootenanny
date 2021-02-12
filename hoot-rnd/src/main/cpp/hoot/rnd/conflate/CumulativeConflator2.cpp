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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */

#include "CumulativeConflator2.h"

// Hoot
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

void CumulativeConflator2::conflate(const QStringList& /*inputs*/, const QString& /*output*/)
{
  //assert(inputs.size() >= 3);

  // TODO: drop features in reviews?
  // TODO: option for diff conflate and merge workflow
  // TODO: pre-attribute conflate against osm option
  // TODO: avg conflate?
  // TODO: modifications to SmallHighwayMerger to make pre-cleaning step better?
  // TODO: score outputs with logic from compare command
  // TODO: ensemble approach


}

}
