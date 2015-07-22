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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
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
using namespace std;

#warning Use HadoopPipesUtils instead of a thief.
/**
 * Hadoop pipes doesn't provide any way to get a list of all values. This thief gets around this.
 * Ugh.
 */
class JobConfThief : public HadoopPipes::JobConf {
private:
  map<string, string> values;
public:
  void set(const string&, const string& value) {
    throw Exception("Not Implemented");
  }

  virtual bool hasKey(const string& key) const {
    throw Exception("Not Implemented");
  }

  virtual const string& get(const string& key) const {
    throw Exception("Not Implemented");
  }

  virtual int getInt(const string& key) const {
    throw Exception("Not Implemented");
  }

  virtual float getFloat(const string& key) const {
    throw Exception("Not Implemented");
  }

  virtual bool getBoolean(const string&key) const {
    throw Exception("Not Implemented");
  }

  const map<string, string>& getMap() const { return values; }
};

}

#pragma GCC diagnostic warning "-Wunused-parameter"

#endif // JOBCONFTHIEF_H
