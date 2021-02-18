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

#ifndef FILEINPUTFORMAT_H
#define FILEINPUTFORMAT_H

// Boost
#include <boost/shared_ptr.hpp>

// Pretty Pipes
#include <pp/InputFormat.h>

#include "FileInputSplit.h"

namespace pp
{

class FileInputFormat : public InputFormat
{
public:
  static std::string className() { return "pp::FileInputFormat"; }

  FileInputFormat();

  virtual ~FileInputFormat() {}

  virtual const pp::InputSplit& getSplit(int i) { return *(_splits[i]); }

  virtual int getSplitCount() { return _splits.size(); }

  virtual void setConfiguration(const Configuration& conf);

  void setPath(const std::string& path) { _path = path; _init(); }

private:
  std::string _path;
  std::vector<boost::shared_ptr<FileInputSplit> > _splits;

  void _addFile(const std::string& path);

  void _init();
};

}

#endif // FILEINPUTFORMAT_H
