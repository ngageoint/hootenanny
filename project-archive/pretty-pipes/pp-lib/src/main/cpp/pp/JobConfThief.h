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

#ifndef JOBCONFTHIEF_H
#define JOBCONFTHIEF_H

// Hadoop Pipes
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <hadoop/Pipes.hh>

// Standard
#include <map>

#include "Exception.h"

namespace pp
{

#warning Use HadoopPipesUtils instead of a thief.
/**
 * Hadoop pipes doesn't provide any way to get a list of all values. This thief gets around this.
 * Ugh.
 */
class JobConfThief : public HadoopPipes::JobConf {
private:
  std::map<std::string, std::string> values;
public:
  void set(const std::string&, const std::string& value) {
    throw Exception("Not Implemented");
  }

  virtual bool hasKey(const std::string& key) const {
    throw Exception("Not Implemented");
  }

  virtual const std::string& get(const std::string& key) const {
    throw Exception("Not Implemented");
  }

  virtual int getInt(const std::string& key) const {
    throw Exception("Not Implemented");
  }

  virtual float getFloat(const std::string& key) const {
    throw Exception("Not Implemented");
  }

  virtual bool getBoolean(const string&key) const {
    throw Exception("Not Implemented");
  }

  const std::map<std::string, std::string>& getMap() const { return values; }
};

}

#pragma GCC diagnostic warning "-Wunused-parameter"

#endif // JOBCONFTHIEF_H
