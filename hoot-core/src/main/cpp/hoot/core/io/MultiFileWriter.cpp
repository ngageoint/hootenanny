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
 * @copyright Copyright (C) 2022 Maxar (http://www.maxar.com/)
 */
#include "MultiFileWriter.h"

namespace hoot
{

const QString MultiFileWriter::POINTS = "Points";
const QString MultiFileWriter::LINES = "Lines";
const QString MultiFileWriter::POLYGONS = "Polygons";

MultiFileWriter::MultiFileWriter(MultiFileWriterType type)
  : _type(type),
    _currentDevice(nullptr),
    _header(&_fileHeader),
    _footer(&_fileFooter)
{
  _header.setCodec("UTF-8");
  _footer.setCodec("UTF-8");
}

MultiFileWriter::~MultiFileWriter()
{
  //  Close  the file(s)/buffer
  close();
}

void MultiFileWriter::setCurrentFileIndex(const QString& index)
{
  switch(_type)
  {
  case MultiFileWriterType::SingleBuffer:
  case MultiFileWriterType::SingleFile:
    //  Nothing needs to be done here, ignore the index
    break;
  default:
    //  Only do something if the index is different
    if (_currentIndex != index)
    {
      //  For multigeometry output check for `Points`, `Lines`, and `Polygons`
      if (_type == MultiFileWriterType::MultiGeom && index != POINTS && index != LINES && index != POLYGONS)
        throw HootException(QString("Illegal file index in multigeometry mode."));
      //  Reset the current device
      _currentDevice = nullptr;
      //  Set the index
      _currentIndex = index;
    }
    break;
  }
}

void MultiFileWriter::write(const QString& contents, MultiFileWriterSection section)
{
  //  Check the section
  if (section == MultiFileWriterSection::SectionBody)
  {
    //  Check the current device before writing
    if (!_currentDevice)
    {
      //  Check the type
      if (_type == MultiFileWriterType::SingleFile)
      {
        //  Create the single file object
        _files.push_back(std::make_shared<QFile>(_filePath));
        _currentDevice = _files[0].get();
        _deviceMap.emplace("SingleFile", _currentDevice);
        //  Open up the single file object
        if (!_currentDevice->open(QIODevice::WriteOnly | QIODevice::Text))
          throw HootException(QString("Error opening %1 for writing").arg(_filePath));
        //  Write the header to the new file before writing what was requested
        _currentDevice->write(_fileHeader.toUtf8());
      }
      else if (_type == MultiFileWriterType::SingleBuffer)
      {
        if (_stringBuffer.isOpen())
          throw HootException(QString("Cannot write to string-buffered object before calling open()."));
        //  Open the buffer and set it to be the current device
        _stringBuffer.open(QBuffer::WriteOnly);
        _currentDevice = &_stringBuffer;
        //  Write the header to the new file before writing what was requested
        _currentDevice->write(_fileHeader.toUtf8());
      }
      else
      {
        if (_currentIndex.isEmpty())
          throw HootException(QString("Cannot write to multifile output without calling setCurrentFileIndex() with non-empty value."));
        //  Check if the current index has an open file object
        if (_deviceMap.find(_currentIndex) != _deviceMap.end())
          _currentDevice = _deviceMap[_currentIndex];
        else
        {
          //  Open a new device and add it to the map
          QString filename = _filePath.arg(_currentIndex);
          //  Create the file object, push it to the back
          _files.push_back(std::make_shared<QFile>(filename));
          _currentDevice = _files.back().get();
          _deviceMap.emplace(_currentIndex, _currentDevice);
          //  Open up the single file object
          if (!_currentDevice->open(QIODevice::WriteOnly | QIODevice::Text))
            throw HootException(QString("Error opening %1 for writing").arg(filename));
          //  Write the header to the new file before writing what was requested
          _currentDevice->write(_fileHeader.toUtf8());
        }
      }
    }
    _currentDevice->write(contents.toUtf8());
  }
  else if (section == MultiFileWriterSection::SectionHeader)
    _header << contents;
  else if (section == MultiFileWriterSection::SectionFooter)
    _footer << contents;
}

QString MultiFileWriter::getBuffer() const
{
  if (_type != MultiFileWriterType::SingleBuffer)
    throw HootException(QString("Cannot call getBuffer() on non-string-buffered output objects."));
  //  Output the object content plus the footer
  return QString::fromUtf8(_stringBuffer.buffer()) + _fileFooter;
}

void MultiFileWriter::open()
{
  //  This version of open can only be called on SingleBuffer objects
  if (_type != MultiFileWriterType::SingleBuffer)
    throw HootException(QString("Cannot call open() without filename on non-string-buffered output objects."));
  //  Cannot re-open the file once it has been written to
  if (_stringBuffer.isOpen())
    throw HootException(QString("Cannot call open() on object that is already open."));
}

void MultiFileWriter::open(const QString& filename)
{
  //  This version of open() cannot be used with the SingleBuffer object
  if (_type == MultiFileWriterType::SingleBuffer)
    throw HootException(QString("Cannot call open(<filename>) on string-buffered output objects."));
  else if (_type == MultiFileWriterType::SingleFile)
    _filePath = filename;
  else
  {
    _filePath = filename;
    //  MultiFile writer will update the filename in QString::arg() format so files can be created
    QFileInfo fi(_filePath);
    QString extension = fi.suffix();
    //  Update the filename
    _filePath.insert(_filePath.length() - (extension.length() + 1), "-%1");
    //  Don't open anything until something is actually written out the the file
  }
}

void MultiFileWriter::close()
{
  switch(_type)
  {
  case MultiFileWriterType::SingleBuffer:
    //  Only need to close the string buffer
    _stringBuffer.close();
    break;
  default:
    //  Iterate all open devices
    for (auto device = _deviceMap.cbegin(); device != _deviceMap.cend(); ++device)
    {
      if (device->second->isOpen())
      {
        //  Any device that is open, has had something written to it, write the footer
        device->second->write(_fileFooter.toUtf8());
        //  Then close it
        device->second->close();
      }
    }
  }
}

}
