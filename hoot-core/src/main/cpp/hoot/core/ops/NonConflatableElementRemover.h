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

#ifndef NONCONFLATABLEELEMENTREMOVER_H
#define NONCONFLATABLEELEMENTREMOVER_H

// Hoot
#include <hoot/core/ops/OsmMapOperation.h>

namespace hoot
{

/**
 * Recursively removes all elements that are deemed non-conflatable
 */
class NonConflatableElementRemover : public OsmMapOperation
{
public:

  static QString className() { return "NonConflatableElementRemover"; }

  NonConflatableElementRemover() = default;
  ~NonConflatableElementRemover() override = default;

  /**
    @see OsmMapOperation
  */
  void apply(std::shared_ptr<OsmMap>& map) override;

  QString getInitStatusMessage() const override
  { return "Removing unconfaltable elements..."; }
  QString getCompletedStatusMessage() const override
  { return "Removed " + QString::number(_numAffected) + " unconflatable elements"; }

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override { return "Removes elements that are not conflatable"; }

private:

  std::shared_ptr<OsmMap> _map;
};

}

#endif // NONCONFLATABLEELEMENTREMOVER_H
