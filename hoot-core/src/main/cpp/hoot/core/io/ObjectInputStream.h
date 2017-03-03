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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef OBJECTINPUTSTREAM_H
#define OBJECTINPUTSTREAM_H

// Boost
#include <boost/any.hpp>

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/io/Serializable.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QDataStream>

// Standard
#include <memory>
#include <sstream>

// Tgs
#include <tgs/Io/StdIoDevice.h>

namespace hoot
{
using namespace boost;
using namespace std;
using namespace Tgs;

class ObjectInputStream
{
public:
  /**
   * Takes ownership of the istream.
   */
  ObjectInputStream(istream& is);

  ObjectInputStream(QDataStream& is);

  /**
   * Reads an object with a "BaseClass" of T. The "BaseClass" is defined in the factory. See
   * the HOOT_FACTORY_REGISTER call for the class you're trying to read.
   */
  template <class T>
  T* readObject();

private:
  istream* _istream;
  auto_ptr<StdIoDevice> _stdIoDevice;
  auto_ptr<QDataStream> _streamDelete;
  QDataStream* _stream;
};

template <class T>
inline T* ObjectInputStream::readObject()
{
  QString className;
  (*_stream) >> className;
  LOG_INFO("Reading: " << className);
  T* result = Factory::getInstance().constructObject<T>(className.toStdString());
  LOG_INFO("type name: " << typeid(result).name());
  // we're assuming that if the class is readable, then it must be Serializable. If for some reason
  // the user serializes a class that isn't Serializable (how?) the results would be undefined.
  Serializable* s = dynamic_cast<Serializable*>(result);
  if (s == 0)
  {
    // I don't think we can ever actually get here, but checking doesn't hurt.
    throw HootException("Object must support serializable. (" + className + ")");
  }
  s->readObject(*_stream);
  return result;
}

}

#endif // OBJECTINPUTSTREAM_H
