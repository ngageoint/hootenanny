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
#ifndef OBJECTOUTPUTSTREAM_H
#define OBJECTOUTPUTSTREAM_H

// Boost
#include <boost/any.hpp>

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/io/Serializable.h>
#include <hoot/core/util/NotImplementedException.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QDataStream>

// Standard
#include <memory>
#include <sstream>

namespace hoot
{
using namespace boost;
using namespace std;

class ObjectOutputStream
{
public:
  ObjectOutputStream(std::ostream& ss);

  ObjectOutputStream(QDataStream& os);

  ~ObjectOutputStream();

  void flush();

  template<class T>
  void writeObject(const T& obj)
  {
    const Serializable* s = dynamic_cast<const Serializable*>(&obj);
    if (s == 0)
    {
      throw NotImplementedException("The object must implement serializable.");
    }
    if (Factory::getInstance().hasClass(s->getClassName()) == false)
    {
      throw HootException("The class must be registered with the factory before it can be "
                          "serialized. (" + s->getClassName() + ")");
    }
    LOG_INFO("Writing object: " << s->getClassName());
    *_stream << QString::fromStdString(s->getClassName());
    s->writeObject(*_stream);
    flush();
  }

  void writeObject(any obj);

public:
  QDataStream* _stream;
  auto_ptr<QDataStream> _stream2Delete;
  auto_ptr<QByteArray> _array;
  ostream* _ostream;
};

}

#endif // OBJECTSTREAM_H
