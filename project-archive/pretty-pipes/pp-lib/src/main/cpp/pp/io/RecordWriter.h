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
 * @copyright Copyright (C) 2015, 2017 Maxar (http://www.maxar.com/)
 */

#ifndef RECORDWRITER_H
#define RECORDWRITER_H

// Boost
#include <boost/shared_ptr.hpp>

// Hadoop Pipes
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <hadoop/Pipes.hh>
#pragma GCC diagnostic warning "-Wunused-parameter"

namespace pp
{

class RecordWriter : public HadoopPipes::RecordWriter
{
public:

  static std::string className() { return "pp::RecordWriter"; }

  virtual ~RecordWriter() {}

  virtual void emit(const std::string& key, const std::string& value)
  {
    emitRecord(key.data(), key.size(), value.data(), value.size());
  }

  template<typename KeyClass, typename ValueClass>
  void emitRecord(const KeyClass& key, const ValueClass& value)
  {
    key.
    emitRecord((const char*)&key, sizeof(key), (const char*)&value, sizeof(value));
  }

  template<class KeyClass, class ValueClass>
  void emitRaw(const KeyClass& key, const ValueClass& value)
  {
    emitRecord((const char*)&key, sizeof(key), (const char*)&value, sizeof(value));
  }

  virtual void emitRecord(const char* keyData, size_t keySize, const char* valueData,
    size_t valueSize) = 0;

};

}

#endif // RECORDWRITER_H
