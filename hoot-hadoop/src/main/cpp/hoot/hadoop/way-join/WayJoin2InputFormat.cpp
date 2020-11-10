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

#include "WayJoin2InputFormat.h"

// Hoot
#include <hoot/core/io/OsmPbfReader.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/HootException.h>
#include <hoot/hadoop/pbf/PbfInputFormat.h>

// Pretty Pipes
#include <pp/Factory.h>
#include <pp/Hdfs.h>
#include <pp/io/FileInputFormat.h>

// Qt
#include <QStringList>

using namespace std;

namespace hoot
{
using namespace pp;

PP_FACTORY_REGISTER(pp::InputFormat, WayJoin2InputFormat)

WayJoin2InputFormat::WayJoin2InputFormat()
{
}

void WayJoin2InputFormat::_addCsqSplits(const QString& p)
{
  Hdfs fs;
  if (fs.getFileStatus(p.toStdString()).isDir())
  {
    vector<FileStatus> status = fs.listStatus(p.toStdString(), true);
    for (size_t i = 0; i < status.size(); i++)
    {
      QString ps = QString::fromStdString(status[i].getPath());
      if (ps.endsWith(".csq"))
      {
        _addCsqSplits(ps);
      }
    }
  }
  else
  {
    pp::FileInputFormat fif;
    fif.setPath(p.toStdString());
    for (int i = 0; i < fif.getSplitCount(); i++)
    {
      boost::shared_ptr<pp::InputSplit> split(fif.getSplit(i).copy());

      // create a parent split and add it to the list.
      WayJoin2InputSplit parentSplit;
      parentSplit.setSplit(split);
      _splits.push_back(parentSplit);
    }
  }
}

void WayJoin2InputFormat::_addPbfSplits(const QString& p)
{
  PbfInputFormat pif;
  pif.setPath(p.toStdString());
  for (int i = 0; i < pif.getSplitCount(); i++)
  {
    boost::shared_ptr<pp::InputSplit> split(pif.getSplit(i).copy());

    // create a parent split and add it to the list.
    WayJoin2InputSplit parentSplit;
    parentSplit.setSplit(split);
    _splits.push_back(parentSplit);
  }
}

void WayJoin2InputFormat::setPath(string path)
{
  QStringList paths = QString::fromStdString(path).split(",");

  for (int i = 0; i < paths.size(); i++)
  {
    if (paths[i].endsWith(".pbf"))
    {
      _addPbfSplits(paths[i]);
    }
    else if (paths[i].endsWith(".csq"))
    {
      _addCsqSplits(paths[i]);
    }
    else
    {
      LOG_WARN("Unexpected file type. (" << paths[i].toStdString() << ")");
    }
  }
}

void WayJoin2InputFormat::setConfiguration(const pp::Configuration& conf)
{
  setPath(conf.get("mapred.input.dir"));
}

}
