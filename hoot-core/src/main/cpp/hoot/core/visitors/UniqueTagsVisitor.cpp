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
#include "UniqueTagsVisitor.h"

// hoot
#include <hoot/core/schema/OsmSchema.h>

namespace hoot
{

// This isn't being factory registered, since there's no standard way to retrieve a set of strings
// from a visitor.

void UniqueTagsVisitor::visit(const ConstElementPtr& e)
{
  for (Tags::const_iterator tagItr = e->getTags().begin(); tagItr != e->getTags().end(); ++tagItr)
  {
    _uniqueKvps.insert(OsmSchema::toKvp(tagItr.key(), tagItr.value()));
  }
}

}