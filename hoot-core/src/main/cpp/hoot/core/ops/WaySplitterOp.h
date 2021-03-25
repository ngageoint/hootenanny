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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef __WAY_SPLITTER_OP_H__
#define __WAY_SPLITTER_OP_H__

// hoot
#include <hoot/core/util/Units.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/Configurable.h>

// Qt
#include <QString>

namespace hoot
{

/**
 * Reprojects the input map to planar if necessary, then splits any way longer than splits any
 * way longer than max length. If the input map is reprojected to planar it will not be reprojected
 * back to geographic when the operation is done. This is done for efficiency reasons.
 *
 * If way.splitter.max.length is not specified then it defaults to 5km.
 */
class WaySplitterOp : public OsmMapOperation, public Configurable
{
public:

  static QString className() { return "hoot::WaySplitterOp"; }

  static QString maxLengthKey() { return "way.splitter.max.length"; }

  WaySplitterOp();
  ~WaySplitterOp() = default;

  void apply(std::shared_ptr<OsmMap>& map) override;

  QString getName() const override { return className(); }

  QString getClassName() const override { return className(); }

  void setConfiguration(const Settings& conf) override;

  void setMaxLength(Meters l) { _maxLength = l; }

  QString getDescription() const override { return "Splits ways"; }

  QString getInitStatusMessage() const override { return "Splitting ways..."; }

  QString getCompletedStatusMessage() const override
  { return "Split " + QString::number(_numAffected) + " ways"; }

private:

  Meters _maxLength;
};

}

#endif // __WAY_SPLITTER_OP_H__
