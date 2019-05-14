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

#ifndef HDFSSTREAM_H
#define HDFSSTREAM_H

// boost
#include <boost/iostreams/operations.hpp>
#include <boost/iostreams/concepts.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/array.hpp>

// Hadoop
#include <hdfs.h>

#include "HdfsConnection.h"

namespace pp
{
using namespace boost::iostreams;

class HdfsDevice
{
public: // boost iostream concepts
  typedef char                                          char_type;
  struct category :
    //public boost::iostreams::bidirectional_device_tag,
    public boost::iostreams::optimally_buffered_tag,
    public boost::iostreams::multichar_tag,
    public boost::iostreams::closable_tag,
    public boost::iostreams::flushable_tag,
    public boost::iostreams::seekable_device_tag { };

  HdfsDevice() :
    _bufferSize(DEFAULT_BUFFER_SIZE), _fp(NULL), _fs(NULL), _info(NULL), _initialized(false),
    _writeOnly(false) { }

  /**
   * Creates a new device for reading and writing.
   * @param fs - Filesystem to use, HdfsDevice takes ownership and will close the FS when it
   *             closes the file.
   * @param filename - File to open.
   * @param write - If true, then writing is enabled. Only writing or reading can be enabled at
   *             one time.
   */
  HdfsDevice(const std::string& host, int port, const std::string& path, const bool write = false,
             int bufferSize = DEFAULT_BUFFER_SIZE) :
    _bufferSize(bufferSize),
    _fp(NULL), _fs(NULL), _host(host), _info(NULL), _initialized(false),
    _path(path), _port(port), _writeOnly(write)
  {
  }

  HdfsDevice(const HdfsDevice& other) :
    _bufferSize(other._bufferSize),
    _fp(NULL), _fs(NULL), _host(other._host), _info(NULL), _initialized(false),
    _path(other._path), _port(other._port), _writeOnly(other._writeOnly)
  {
  }

  virtual ~HdfsDevice()
  {
  }

  /**
   * Close the file and disconnect from the filesystem.
   */
  void close()
  {
    if(_fp != NULL)
    {
      if(_fp->type == OUTPUT)
      {
        if (hdfsFlush(_fs, _fp) == -1)
        {
          throw std::ios_base::failure("Error flushing HDFS stream.");
        }
      }
      if (hdfsCloseFile(_fs, _fp) == -1)
      {
        throw std::ios_base::failure("Error closing HDFS stream.");
      }
      _fp = NULL;
      _fs = NULL;
    }
  }

  bool flush()
  {
    if (_writeOnly && _fp != NULL && hdfsFlush(_fs, _fp) == -1)
    {
      throw std::ios_base::failure("Error flushing HDFS stream.");
    }
    return true;
  }

  hdfsFileInfo* _getInfo()
  {
    if (_info == NULL)
    {
      _info = hdfsGetPathInfo(_fs, _path.data());
      if (_info == NULL)
      {
        throw std::ios_base::failure("Error getting path info.");
      }
    }
    return _info;
  }

  void _init() const
  {
    if (_initialized == false)
    {
      _fs = HdfsConnection::getInstance().getConnection(_host, _port);
      if (_fs == NULL)
      {
        throw std::ios_base::failure("Error connecting to HDFS.");
      }

      // open the file
      const int flags = _writeOnly ? O_WRONLY : O_RDONLY;
      const short replication = 0; // use default
      const tSize block_size = 0; // use default;
      _fp = hdfsOpenFile(_fs, _path.c_str(), flags, _bufferSize,
                          replication, block_size);
      if (_fp == NULL)
      {
        throw std::ios_base::failure(QString("Error opening file (%1)").
          arg(QString::fromStdString(_path)).toStdString());
      }
      _initialized = true;
    }
  }

  inline std::streamsize optimal_buffer_size() const { return _bufferSize; }

  /**
   * Reads from HDFS and puts the value in strm_ptr. No more than n bytes will be read. On failure
   * a -1 is returned.
   */
  std::streamsize read(char* strm_ptr, std::streamsize n)
  {
    _init();
    std::streamsize result = hdfsRead(_fs, _fp, strm_ptr, n);
    // if we're at the end of the stream
    if (result == 0)
    {
      // boost expectes -1 at EOF
      result = -1;
    }
    return result;
  }

  virtual std::streampos seek(std::streampos off, std::ios_base::seekdir way)
  {
    _init();
    tOffset pos;
    if (way == std::ios_base::end)
    {
      pos = _getInfo()->mSize + off;
    }
    else if (way == std::ios_base::beg)
    {
      pos = off;
    }
    else if (way == std::ios_base::cur)
    {
      pos = hdfsTell(_fs, _fp);
      if (pos == -1)
      {
        throw std::ios_base::failure("Error querying HDFS file position.");
      }
      pos += off;
    }
    else
    {
      throw std::ios_base::failure("Unexpected state.");
    }
    if (hdfsSeek(_fs, _fp, pos) == -1)
    {
      throw std::ios_base::failure("Error seeking on HDFS file.");
    }
    return pos;
  }

  std::streamsize write(const char* strm_ptr, std::streamsize n) {
    _init();
     return hdfsWrite(_fs, _fp, strm_ptr, n);
  }

  bool good() const
  {
    _init(); return _fp != NULL;
  }

private:
  static const int DEFAULT_BUFFER_SIZE = 1 << 10;

  int _bufferSize;
  mutable hdfsFile _fp;
  mutable hdfsFS _fs;
  std::string _host;
  hdfsFileInfo* _info;
  mutable bool _initialized;
  std::string _path;
  int _port;
  bool _writeOnly;

  HdfsDevice& operator=(const HdfsDevice& other);
};

}

#endif // HDFSSTREAM_H
