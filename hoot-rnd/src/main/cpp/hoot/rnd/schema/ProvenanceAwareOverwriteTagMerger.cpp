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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ProvenanceAwareOverwriteTagMerger.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MetadataTags.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(TagMerger, ProvenanceAwareOverwriteTagMerger)

ProvenanceAwareOverwriteTagMerger::ProvenanceAwareOverwriteTagMerger(bool swap) :
OverwriteTagMerger(swap)
{
}

Tags ProvenanceAwareOverwriteTagMerger::mergeTags(const Tags& t1, const Tags& t2,
                                                  ElementType et) const
{
  Tags tags = OverwriteTagMerger::mergeTags(t1, t2, et);
  QString sourceVal = "";
  if (t1.contains(MetadataTags::HootSource()) && t2.contains(MetadataTags::HootSource()))
  {
    sourceVal = t1.get(MetadataTags::HootSource()) + "," + t2.get(MetadataTags::HootSource());
  }
  else if (t1.contains(MetadataTags::HootSource()))
  {
    sourceVal = t1.get(MetadataTags::HootSource());
  }
  else if (t2.contains(MetadataTags::HootSource()))
  {
    sourceVal = t2.get(MetadataTags::HootSource());
  }
  tags.set(MetadataTags::HootSource(), sourceVal);
  return tags;
}

}
