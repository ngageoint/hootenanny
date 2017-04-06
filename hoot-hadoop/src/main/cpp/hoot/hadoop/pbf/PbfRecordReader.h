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

#ifndef PBFRECORDREADER_H
#define PBFRECORDREADER_H

// Boost
#include <boost/shared_ptr.hpp>

// Pretty Pipes
#include <pp/RecordReader.h>

// Std
#include <string>
#include <vector>

namespace hoot
{
class OsmMap;

using namespace boost;
using namespace std;

class PbfRecordReader : public pp::RecordReader
{
public:
  static string className() { return "hoot::PbfRecordReader"; }

  PbfRecordReader() {}

  virtual ~PbfRecordReader() {}

 OsmMapPtr getMap();

  virtual bool next(std::string& key, std::string& value);

  /**
   * The progress of the record reader through the split as a value between
   * 0.0 and 1.0.
   */
  virtual float getProgress();

  virtual void initialize(pp::InputSplit* split, HadoopPipes::MapContext& context);

private:
  string _path;
  /**
   * The start position of all the headers in this split.
   */
  vector<long> _headers;
  int _currentHeader;
  long _start;
};


}

#endif // PBFRECORDREADER_H
