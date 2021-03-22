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
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
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

  static QString className() { return "hoot::ElementIdToVersionMapper"; }

  ElementIdToVersionMapper() = default;
  ~ElementIdToVersionMapper() = default;

  void apply(const OsmMapPtr& map) override;

  QMap<ElementId, long> getMappings() const { return _idToVersionMappings; }

  QString getInitStatusMessage() const override
  { return "Mapping element IDs to changeset versions..."; }

  QString getCompletedStatusMessage() const override
  { return "Mapped " +  StringUtils::formatLargeNumber(_numAffected) + " element IDs to versions."; }

  QString getDescription() const override
  { return "Maintains a mapping of element IDs to changeset versions"; }

  QString getName() const override { return className(); }

  QString getClassName() const override { return className(); }

private:

  QMap<ElementId, long> _idToVersionMappings;
};

}

#endif // ELEMENT_ID_TO_VERSION_MAPPER_H
