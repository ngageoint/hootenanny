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

#include "Hdfs.h"

// Qt
#include <QSet>

// Standard
#include <fstream>

//Boost
#include <boost/shared_ptr.hpp>

#include "HdfsConnection.h"
#include "HdfsDevice.hpp"

using namespace std;

namespace pp
{

Hdfs::Hdfs(const string& host, int port)
  : _host(host),
    _port(port),
    _fs(NULL)
{
}

void Hdfs::copyFromLocal(string src, string dst)
{
  boost::shared_ptr<ostream> dstStream(create(dst));
  fstream srcStream;
  srcStream.open(src.data(), fstream::in);
  if (srcStream.good() == false)
  {
    throw ios_base::failure("Error opening src.");
  }

  // just using typical block size on disk (4096), there is probably a better value.
  char buf[4096];
  while (srcStream.eof() == false)
  {
    srcStream.read(buf, 4096);
    dstStream->write(buf, srcStream.gcount());
  }

  dstStream->flush();
}

ostream* Hdfs::create(string path)
{
  HdfsDevice dev(_host, _port, path, true);
  return new boost::iostreams::stream<HdfsDevice>(dev);
}

bool Hdfs::deletePath(string path, bool recursive)
{
  if (recursive == false)
  {
    throw ios_base::failure("Unsupported.");
  }
  if (hdfsDelete(_getFs(), path.data()) == -1)
  {
    throw ios_base::failure("Error deleting specified path.");
  }
  return true;
}

bool Hdfs::exists(string path)
{
  // there doesn't appear to be an error code for this function. Simply 0 if it exists and -1 if
  // it doesn't
  return hdfsExists(_getFs(), path.data()) == 0;
}

string Hdfs::getAbsolutePath(const std::string& path)
{
  QString qPath = QString::fromStdString(path);
  // if it is already absolute.
  if (path[0] == '/' || qPath.startsWith("hdfs://"))
  {
    return path;
  }

  char tmp[1024];
  char* buf = hdfsGetWorkingDirectory(_getFs(), tmp, 1024);
  if (buf == NULL)
  {
    throw ios_base::failure("Error retrieving working directory.");
  }

  QString cwd(tmp);
  if (cwd.endsWith("/") == false)
  {
    cwd = cwd + "/";
  }
  return cwd.toStdString() + path;
}

hdfsFS Hdfs::_getFs()
{
  if (_fs == NULL)
  {
    _fs = HdfsConnection::getInstance().getConnection(_host, _port);
  }
  return _fs;
}

FileStatus Hdfs::getFileStatus(string path)
{
  hdfsFileInfo* fi = hdfsGetPathInfo(_getFs(), path.data());

  if (fi == NULL)
  {
    throw ios_base::failure("Error retrieving file status. (" + path + ")");
  }

  FileStatus result;
  result._setAccessTime(fi->mLastAccess);
  result._setBlockSize(fi->mBlockSize);
  result._setModificationTime(fi->mLastMod);
  result._setLen(fi->mSize);
  result._setKind(fi->mKind);
  result._setPath(fi->mName);

  hdfsFreeFileInfo(fi, 1);

  return result;
}

QStringList Hdfs::getLocations(string path, long start, long length, const bool sort)
{
  char*** h = hdfsGetHosts(_getFs(), path.data(), start, length);
  if (h == NULL)
  {
    throw ios_base::failure("Error getting block locations from HDFS.");
  }

  QSet<QString> hosts;


  int i = 0;
  while (h[i] != NULL)
  {
    int j = 0;
    while (h[i][j] != NULL)
    {
      char* host = h[i][j];
      hosts.insert(host);
      j++;
    }
    i++;
  }

  QStringList hostsList(hosts.toList());
  if (sort)
  {
    hostsList.sort();
  }
  return hostsList;
}

bool Hdfs::_fileStatusPathCompare(const FileStatus& fs1, const FileStatus& fs2)
{
  return fs1.getPath() > fs2.getPath();
}

std::vector<FileStatus> Hdfs::listStatus(string path, const bool sortByPath)
{
  if (exists(path) == false)
  {
    throw ios_base::failure("Error retrieving status on non-existant path (" + path + ")");
  }
  std::vector<FileStatus> result;

  int numEntries;
  hdfsFileInfo* fis = hdfsListDirectory(_getFs(), path.data(), &numEntries);

  if (fis == NULL)
  {
    throw ios_base::failure("Error listing directory contents. (" + path + ")");
  }

  result.resize(numEntries);
  for (int i = 0; i < numEntries; i++)
  {
    hdfsFileInfo* fi = fis + i;
    FileStatus& r = result[i];
    r._setAccessTime(fi->mLastAccess);
    r._setModificationTime(fi->mLastMod);
    r._setLen(fi->mSize);
    r._setKind(fi->mKind);
    r._setPath(fi->mName);
  }

  hdfsFreeFileInfo(fis, numEntries);

  if (sortByPath)
  {
    sort(result.begin(), result.end(), _fileStatusPathCompare);
  }

  return result;
}

void Hdfs::mkdirs(string path)
{
  if (hdfsCreateDirectory(_getFs(), path.data()) == -1)
  {
    throw ios_base::failure("Error creating directory");
  }
}

istream* Hdfs::open(std::string path)
{
  if (exists(path) == false)
  {
    throw ios_base::failure("File not found.");
  }
  HdfsDevice dev(_host, _port, path);
  return new boost::iostreams::stream<HdfsDevice>(dev);
}

void Hdfs::remove(string path)
{
  if (hdfsDelete(_getFs(), path.data()) == -1)
  {
    throw ios_base::failure(QString("Error deleting file (%1)").
      arg(QString::fromStdString(path)).toStdString());
  }
}

void Hdfs::rename(const string& from, const string& to)
{
  if (hdfsRename(_getFs(), from.data(), to.data()) == -1)
  {
    throw ios_base::failure(QString("Error renaming file (%1 to %2)").
      arg(QString::fromStdString(from)).arg(QString::fromStdString(to)).toStdString());
  }
}

}
