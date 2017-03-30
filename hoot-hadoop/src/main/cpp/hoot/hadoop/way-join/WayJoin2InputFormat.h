/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef WAYJOIN2INPUTFORMAT_H
#define WAYJOIN2INPUTFORMAT_H

// Hoot
#include <hoot/hadoop/pbf/PbfInputSplit.h>

// Pretty Pipes
#include <pp/conf/Configuration.h>
#include <pp/InputFormat.h>

// Standard
#include <vector>

#include "WayJoin2InputSplit.h"

namespace hoot
{

/**
 * Due to the complexities of hadoop I'm not testing this class directly. There are indirect
 * tests in place that excersize the class. Writing a unit test specific to this class that actually
 * excersizes all the complexities in Hadoop is hard to jusitify.
 */
class WayJoin2InputFormat : public pp::InputFormat
{
public:

  static std::string className() { return "hoot::WayJoin2InputFormat"; }

  WayJoin2InputFormat();

  const hoot::WayJoin2InputSplit& getSplit(int i) { return _splits[i]; }

  int getSplitCount() { return _splits.size(); }

  virtual void setPath(string path);

  virtual void setConfiguration(const pp::Configuration& conf);

private:

  std::vector<WayJoin2InputSplit> _splits;

  void _addPbfSplits(const QString& p);

  void _addCsqSplits(const QString& p);

};

}

#endif // WAYJOIN2INPUTFORMAT_HB
