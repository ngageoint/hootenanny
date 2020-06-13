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
 * @copyright Copyright (C) 2015, 2017, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

// Qt
class QDataStream;

// Standard
#include <string>

namespace hoot
{

/**
 * Note that this interface was primarily in place to support hoot use in Hadoop for serializing
 * objects between map/reduce conflation steps.  Even with Hadoop support gone, there's no harm in
 * keeping it around, for now, in case we need to serialize anything in a similar fashion in the
 * future.
 */
class Serializable
{
public:

  Serializable() = default;
  virtual ~Serializable() = default;

  /**
   * Returns the name of the class. Typically just returns the result of className().
   */
  virtual std::string getClassName() const = 0;

  /**
   * Reads the information for this object from the stream.
   */
  virtual void readObject(QDataStream& is) = 0;

  /**
   * Writes the data for this object to the stream. This should _not_ include uniquely identifying
   * class information. E.g. the class name.
   */
  virtual void writeObject(QDataStream& os) const = 0;
};

}

#endif // SERIALIZABLE_H
