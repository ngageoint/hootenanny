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

#include "PbfInputFormat.h"

// Hoot
#include <hoot/core/io/OsmPbfReader.h>
#include <hoot/core/util/Log.h>

// Pretty Pipes
#include <pp/Factory.h>
#include <pp/Hdfs.h>

using namespace std;

namespace hoot
{
using namespace pp;

PP_FACTORY_REGISTER(pp::InputFormat, PbfInputFormat)

PbfInputFormat::PbfInputFormat()
{
}

PbfInputFormat::~PbfInputFormat()
{
}

void PbfInputFormat::_addSplit(const string& path)
{
  Hdfs hdfs("default", 0);
  auto_ptr<istream> strm(hdfs.open(path));

  FileStatus status = hdfs.getFileStatus(path);
  long blockSize = status.getBlockSize();

  OsmPbfReader reader(true);
  reader.setUseFileStatus(true);
  const vector<OsmPbfReader::BlobLocation>& result = reader.loadOsmDataBlobOffsets(*strm);

  long nextBlock = blockSize;

  PbfInputSplit split;
  split.setPath(path);

  for (size_t i = 0; i < result.size(); i++)
  {
    split.addHeader(result[i].headerOffset, result[i].getSize());
    if (result[i].headerOffset + result[i].getSize() >= nextBlock - 1)
    {
      QStringList hosts = hdfs.getLocations(path, split.getStart(), split.getLength(), true);
      split.setLocations(hosts.join(";").toStdString());
      _splits.push_back(split);
      split.reset();
      split.setPath(path);
      nextBlock += blockSize;
    }
  }

  if (split.isValid())
  {
    QStringList hosts = hdfs.getLocations(path, split.getStart(), split.getLength(), true);
    split.setLocations(hosts.join(";").toStdString());
    _splits.push_back(split);
  }
}

void PbfInputFormat::_init()
{
  Hdfs fs;

  FileStatus s = fs.getFileStatus(_path);
  if (s.isDir())
  {
    vector<FileStatus> status = fs.listStatus(_path, true);
    for (size_t i = 0; i < status.size(); i++)
    {
      QString str = QString::fromStdString(status[i].getPath());
      LOG_INFO("Processing split: " << str);
      if (str.endsWith(".pbf") && status[i].isFile())
      {
        _addSplit(str.toStdString());
      }
    }
  }
  else
  {
    _addSplit(_path);
  }
}

void PbfInputFormat::setConfiguration(const pp::Configuration& conf)
{
  setPath(conf.get("mapred.input.dir"));
}

}
