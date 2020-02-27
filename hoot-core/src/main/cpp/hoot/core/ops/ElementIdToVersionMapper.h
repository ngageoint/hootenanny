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
#ifndef ELEMENT_ID_TO_VERSION_MAPPER_H
#define ELEMENT_ID_TO_VERSION_MAPPER_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/ConstOsmMapOperation.h>
#include <hoot/core/util/StringUtils.h>

namespace hoot
{

class ElementIdToVersionMapper : public ConstOsmMapOperation
{
public:

  static std::string className() { return "hoot::ElementIdToVersionMapper"; }

  ElementIdToVersionMapper();

  virtual void apply(const OsmMapPtr& map);

  QMap<ElementId, long> getMappings() const { return _idToVersionMappings; }

  virtual QString getInitStatusMessage() const
  { return "Mapping element IDs to changeset versions..."; }

  virtual QString getCompletedStatusMessage() const
  { return "Mapped " +  StringUtils::formatLargeNumber(_numAffected) + " element IDs to versions."; }

  virtual QString getDescription() const
  { return "Maintains a mapping of element IDs to changeset versions"; }

  virtual std::string getClassName() const { return className(); }

private:

  QMap<ElementId, long> _idToVersionMappings;
};

}

#endif // ELEMENT_ID_TO_VERSION_MAPPER_H
