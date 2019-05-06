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
#ifndef BUILDING_PART_TAG_MERGER_H
#define BUILDING_PART_TAG_MERGER_H

#include <hoot/core/schema/TagMerger.h>

namespace hoot
{

/**
 * TODO
 */
class BuildingPartTagMerger : public TagMerger
{
public:

  static std::string className() { return "hoot::BuildingPartTagMerger"; }

  BuildingPartTagMerger();
  BuildingPartTagMerger(const std::set<QString>& buildingPartTagNames);

  virtual Tags mergeTags(const Tags& buildingTags, const Tags& buildingPartTags,
                         ElementType et) const override;

  virtual QString getDescription() const
  { return "TODO"; }

  void setBuildingPartTagNames(std::set<QString> tagNames) { _buildingPartTagNames = tagNames; }

private:

  std::set<QString> _buildingPartTagNames;
};

}

#endif // BUILDING_PART_TAG_MERGER_H
