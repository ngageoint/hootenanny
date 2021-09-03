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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef MULTIARY_POI_HASH_VISITOR_H
#define MULTIARY_POI_HASH_VISITOR_H

// hoot
#include <hoot/core/visitors/ElementHashVisitor.h>

namespace hoot
{

/**
 * This contains the original node hash implementation created for Multiary POI Conflation, which
 * is dormant. The default implementation this inherits from has been simplified.
 */
class MultiaryPoiHashVisitor : public ElementHashVisitor
{
public:

  static QString className() { return "MultiaryPoiHashVisitor"; }

  MultiaryPoiHashVisitor() = default;
  ~MultiaryPoiHashVisitor() = default;

  QString getName() const override { return className(); }

  QString getClassName() const override { return className(); }

  QString getDescription() const override
  { return "Calculates unique hash values for POIs conflated with Multiary POI Conflation"; }

protected:

  QString _toJson(const ConstNodePtr& node) const override;
};

}

#endif // MULTIARY_POI_HASH_VISITOR_H
