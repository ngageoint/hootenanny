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

#include "FileInputFormat.h"

#include <pp/Factory.h>
#include <pp/Hdfs.h>

using namespace std;

namespace pp
{

PP_FACTORY_REGISTER(InputFormat, FileInputFormat)

FileInputFormat::FileInputFormat()
{
}

void FileInputFormat::_addFile(const string& path)
{
  Hdfs hdfs;
  FileStatus status = hdfs.getFileStatus(path);

  long blockSize = status.getBlockSize();
  long len = status.getLen();

  for (long pos = 0; pos < len; pos += blockSize)
  {
    boost::shared_ptr<FileInputSplit> fis(new FileInputSplit());
    fis->setStart(pos);
    fis->setLength(std::min(blockSize, len - pos));
    fis->setPath(path);

    QStringList hosts = hdfs.getLocations(path, fis->getStart(), fis->getLength(), true);
    fis->setLocations(hosts.join(";").toStdString());
    _splits.push_back(fis);
  }
}

void FileInputFormat::_init()
{
  Hdfs hdfs;
  FileStatus status = hdfs.getFileStatus(_path);

  if (status.isFile())
  {
    _addFile(_path);
  }
  else
  {
    vector<FileStatus> children = hdfs.listStatus(_path, true);

    for (size_t i = 0; i < children.size(); i++)
    {
      if (children[i].isFile())
      {
        _addFile(children[i].getPath());
      }
    }
  }

}

void FileInputFormat::setConfiguration(const pp::Configuration& conf)
{
  setPath(conf.get("mapred.input.dir"));
}

}
