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
 * This is used to preserve types when merging multiple elements into one. e.g. merging multiple
 * POIs into a multi-use building. It keeps the most specific type when multiple types with the same
 * key are encountered. In the case where duplicated types have the same level of specificity, the
 * duplicated types are arbitrarily placed in an "alt_types" tag.
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
   */
  PreserveTypesTagMerger(const std::set<QString>& skipTagKeys = std::set<QString>());

  /**
   * @see TagMerger
   */
  virtual Tags mergeTags(const Tags& t1, const Tags& t2, ElementType et) const override;

  virtual QString getDescription() const
  { return "Keeps tags from both features and preserves overlapping type tags"; }

  void setOverwrite1(bool overwrite) { _overwrite1 = overwrite; }
  void setSkipTagKeys(const std::set<QString>& keys) { _skipTagKeys = keys; }

private:

  // if true the first set of tags passed into mergeTags are overwritten; otherwise the second set
  // is overwritten
  bool _overwrite1;
  // any type tag that would otherwise be preserved will be skipped if in this list
  std::set<QString> _skipTagKeys;

  Tags _preserveAltTypes(const Tags& source, const Tags& target) const;
  // can probably eventually get rid of this by correcting logic that's duplicating tags in
  // mergeTags
  void _removeRedundantAltTypeTags(Tags& tags) const;

  bool _isAncestor(const QString& childKey, const QString& childVal, const QString& parentKey,
                   const QString& parentVal) const;
};

}

#endif // PRESERVETYPESTAGMERGER_H
