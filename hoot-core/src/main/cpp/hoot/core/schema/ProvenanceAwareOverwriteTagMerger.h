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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef PROVENANCEAWAREOVERWRITETAGMERGER_H
#define PROVENANCEAWAREOVERWRITETAGMERGER_H

// hoot
#include <hoot/core/schema/OverwriteTagMerger.h>

namespace hoot
{

/**
 * Same as OverwriteTag2Merger except the provenance tag, hoot:source, which will be merged using
 * values from both features.
 */
class ProvenanceAwareOverwriteTagMerger : public OverwriteTagMerger
{

public:

  static QString className() { return "ProvenanceAwareOverwriteTagMerger"; }

  /**
   * @brief Constructor
   * @param swap swaps the direction the tags are merged
   */
  ProvenanceAwareOverwriteTagMerger(bool swap = false);

  /**
   * @see TagMerger
   */
  Tags mergeTags(const Tags& t1, const Tags& t2, ElementType et) const override;

  QString getDescription() const override
  { return "Overwrites secondary feature tags with reference tags while maintaining provenance (experimental)"; }
};

}

#endif // PROVENANCEAWAREOVERWRITETAGMERGER_H
