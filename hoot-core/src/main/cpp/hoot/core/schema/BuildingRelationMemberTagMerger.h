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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef BUILDING_RELATION_MEMBER_TAG_MERGER_H
#define BUILDING_RELATION_MEMBER_TAG_MERGER_H

#include <hoot/core/schema/TagMerger.h>

namespace hoot
{

/**
 * A tag merger for relation building members
 */
class BuildingRelationMemberTagMerger : public TagMerger
{
public:

  static QString className() { return "BuildingRelationMemberTagMerger"; }

  BuildingRelationMemberTagMerger() = default;
  BuildingRelationMemberTagMerger(const QSet<QString>& ignoreTagKeys);
  ~BuildingRelationMemberTagMerger() = default;

  /**
   * Merges tags between a building and a single building part
   *
   * @param relationTags the final output tags for the building
   * @param constituentBuildingTags the tags of the building part
   * @param elementType the type of element whose tags are being merged (ignored)
   * @return a merged set of tags
   */
  Tags mergeTags(
    const Tags& relationTags, const Tags& constituentBuildingTags,
    ElementType elementType) const override;

  static QSet<QString> getBuildingPartTagNames();

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override
  { return "Merges building and building part tags together"; }

private:

  // tag keys that identify building parts
  static QSet<QString> _buildingPartTagNames;
  // a set of tag keys, which will be ignored during tag merging
  QSet<QString> _ignoreTagKeys;
};

}

#endif // BUILDING_RELATION_MEMBER_TAG_MERGER_H
