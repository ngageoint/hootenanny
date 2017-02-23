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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef IDGENERATOR_H
#define IDGENERATOR_H

#include <tgs/SharedPtr.h>

// standard
#include <string>

namespace hoot
{
using namespace std;

class IdGenerator
{
public:
  static string className() { return "hoot::IdGenerator"; }

  IdGenerator() {}

  virtual ~IdGenerator() {}

  virtual boost::shared_ptr<IdGenerator> clone() const = 0;

  virtual long createNodeId() = 0;

  virtual long createRelationId() = 0;

  virtual long createWayId() = 0;

  virtual void ensureNodeBounds(long nid) = 0;

  virtual void ensureRelationBounds(long rid) = 0;

  virtual void ensureWayBounds(long wid) = 0;

  static shared_ptr<IdGenerator> getInstance();

  virtual void reset() = 0;

private:
  static shared_ptr<IdGenerator> _theInstance;
};

typedef boost::shared_ptr<IdGenerator> IdGeneratorPtr;

}

#endif // IDGENERATOR_H
