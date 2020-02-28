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
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef ELEMENT_REPLACER_H
#define ELEMENT_REPLACER_H

// Hoot
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

/**
 * Replaces all elements in a map with features from another map with corresponding element IDs.
 * Elements in the from map with no corresponding IDs in the target map are skipped.
 */
class ElementReplacer : public OsmMapOperation
{
public:

  static std::string className() { return "hoot::ElementReplacer"; }

  ElementReplacer();
  ElementReplacer(OsmMapPtr mapToReplaceFrom);

  /**
   * @see OsmMapOperation
   */
  virtual void apply(std::shared_ptr<OsmMap>& map) override;

  virtual QString getDescription() const override
  { return "Replace features in a map with those from another map"; }

  virtual QString getInitStatusMessage() const
  { return "Replacing elements..."; }

  virtual QString getCompletedStatusMessage() const
  { return "Replaced " + QString::number(_numAffected) + " elements"; }

  virtual std::string getClassName() const { return className(); }

private:

  OsmMapPtr _mapToReplaceFrom;
};

}

#endif // ELEMENT_REPLACER_H
