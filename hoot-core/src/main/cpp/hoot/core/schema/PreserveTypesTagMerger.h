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
 * @copyright Copyright (C) 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef PRESERVETYPESTAGMERGER_H
#define PRESERVETYPESTAGMERGER_H

#include <hoot/core/schema/TagMerger.h>
#include <hoot/core/schema/OsmSchema.h>

namespace hoot
{

/**
 * This is used to preserve types when merging multiple elements into one.
 *
 * e.g. merging multiple POIs into a multi-use building
 */
class PreserveTypesTagMerger : public TagMerger
{
public:

  static QString ALT_TYPES_TAG_KEY;

  static std::string className() { return "hoot::PreserveTypesTagMerger"; }

  /**
   * Constructor
   *
   * @param skipTagKeys optional; Any additional type tags found during merging with a key in this
   * list will be not be preserved.
   * @param categoryFilter optional; Any additional type tags found during merging that do fall
   * within the specified category will be skipped
   */
  PreserveTypesTagMerger(const std::set<QString>& skipTagKeys = std::set<QString>(),
                         const OsmSchemaCategory& categoryFilter = OsmSchemaCategory::Empty);

  virtual Tags mergeTags(const Tags& t1, const Tags& t2, ElementType et) const override;

  virtual QString getDescription() const
  { return "Keeps tags from both features and preserves overlapping type tags"; }

private:

  // see descriptions for these in constructor
  std::set<QString> _skipTagKeys;
  OsmSchemaCategory _categoryFilter;

  Tags _preserveAltTypes(const Tags& source, const Tags& target) const;
  // can probably eventually get rid of this by correcting logic that's duplicating tags in
  // mergeTags
  void _removeRedundantAltTypeTags(Tags& tags) const;
};

}

#endif // PRESERVETYPESTAGMERGER_H
