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
 * @copyright Copyright (C) 2016, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef INVERTEDEDGEMATCHSET_H
#define INVERTEDEDGEMATCHSET_H

#include <hoot/core/conflate/network/EdgeMatchSet.h>

namespace hoot
{

/**
 * Presents the matches in a EdgeMatchSet so that the first string is the second and vice versa.
 */
class InvertedEdgeMatchSet : public EdgeMatchSet
{
public:
  InvertedEdgeMatchSet();

  virtual bool contains(const EdgeMatchPtr &em) const = 0;

  virtual QString toString() const override { assert(false); return ""; }
};

}

#endif // INVERTEDEDGEMATCHSET_H
