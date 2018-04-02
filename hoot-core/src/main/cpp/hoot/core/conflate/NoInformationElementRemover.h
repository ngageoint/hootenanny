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
 * @copyright Copyright (C) 2015, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef NOINFORMATIONELEMENTREMOVER_H
#define NOINFORMATIONELEMENTREMOVER_H

// TGS
#include <tgs/SharedPtr.h>

// Hoot
#include <hoot/core/ops/OsmMapOperation.h>

namespace hoot
{
  class OsmMap;

/**
 * Recursively removes all elements that do not contain any information and are not in use by
   another element.
 */
class NoInformationElementRemover : public OsmMapOperation
{
  public:

    static std::string className() { return "hoot::NoInformationElementRemover"; }

    NoInformationElementRemover();

    /**
      @see OsmMapOperation
    */
    void apply(boost::shared_ptr<OsmMap>& map);

    virtual QString getDescription() const
    { return "Removes elements containing no information"; }

  protected:

    boost::shared_ptr<OsmMap> _map;
};

}

#endif // NOINFORMATIONELEMENTREMOVER_H
