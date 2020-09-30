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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef CHANGESET_REPLACEMENT_CREATOR_4_H
#define CHANGESET_REPLACEMENT_CREATOR_4_H

// Hoot
#include <hoot/core/algorithms/changeset/ChangesetReplacementCreator1.h>

namespace hoot
{

/**
 * This initially loads in the features with relations and all relations members and then removes
 * just the references to the relations members which don't match the geometry type for the data
 * being processed.
 */
class ChangesetReplacementCreator4 : public ChangesetReplacementCreator1
{

public:

  static std::string className() { return "hoot::ChangesetReplacementCreator4"; }

  ChangesetReplacementCreator4();

  virtual QString toString() const override
    { return QString::fromStdString(className()).remove("hoot::"); }

protected:

  /*
   * Returns the default geometry filters (point, line, poly) to use when no other geometry filters
   * are specified
   */
  virtual QMap<GeometryTypeCriterion::GeometryType, ElementCriterionPtr>
    _getDefaultGeometryFilters() const override;
};

}

#endif // CHANGESET_REPLACEMENT_CREATOR_4_H
