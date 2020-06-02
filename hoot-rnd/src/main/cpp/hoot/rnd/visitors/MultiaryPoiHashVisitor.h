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

  static std::string className() { return "hoot::MultiaryPoiHashVisitor"; }

  MultiaryPoiHashVisitor();

  virtual std::string getClassName() const { return className(); }

  virtual QString getDescription() const
  { return "Calculates unique hash values for POIs conflated with Multiary POI Conflation"; }

protected:

  virtual QString _toJson(const ConstNodePtr& node) const;
};

}

#endif // MULTIARY_POI_HASH_VISITOR_H
