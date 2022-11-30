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

MultiFileWriter::MultiFileWriter(MultiFileWriterType type)
  : _type(type),
    _currentSection(SectionHeader),
    _currentDevice(nullptr),
    _header(&_fileHeader),
    _footer(&_fileFooter),
    _isOpen(false)
{
  _header.setCodec("UTF-8");
  _footer.setCodec("UTF-8");
}

MultiFileWriter::~MultiFileWriter()
{
  //  Close  the file(s)/buffer
  close();
}

void MultiFileWriter::setWriterType(MultiFileWriterType type)
{
  //  Close the object if it is open
  if (_isOpen)
    close();
  //  Reset the header and footer
  _header.reset();
  _footer.reset();
  //  Set the type
  _type = type;
  //  Reset the internals
  _currentDevice = nullptr;
  _deviceMap.clear();
  _files.clear();
  _currentIndex = "";
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
      //  Reset the current device
      _currentDevice = nullptr;
      //  Set the index
      _currentIndex = index;
    }
    break;
  }
}

bool MultiFileWriter::isCurrentIndexWritten() const
{
  //  Check if the value even exists and return it
  if (_writtenMap.find(_currentIndex) != _writtenMap.end())
    return _writtenMap.at(_currentIndex);
  //  Nothing written if the value doesn't exist
  return false;
}

void MultiFileWriter::writeHeader(const QString& contents)
{
  MultiFileWriterSection section = _currentSection;
  //  Set the section to header
  _currentSection = MultiFileWriterSection::SectionHeader;
  //  Write to the header
  write(contents);
  //  Set the section to the previously selected section
  _currentSection = section;
}

void MultiFileWriter::writeFooter(const QString& contents)
{
  MultiFileWriterSection section = _currentSection;
  //  Set the section to footer
  _currentSection = MultiFileWriterSection::SectionFooter;
  //  Write to the footer
  write(contents);
  //  Set the section to the previously selected section
  _currentSection = section;
}

void MultiFileWriter::write(const QString& contents)
{
  //  Check the section
  if (_currentSection == MultiFileWriterSection::SectionBody)
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
    //  Record that something has been written to the file at the current
    _writtenMap[_currentIndex] = true;
    //  Do the actual writing
    _currentDevice->write(contents.toUtf8());
  }
  else if (_currentSection == MultiFileWriterSection::SectionHeader)
    _header << contents;
  else if (_currentSection == MultiFileWriterSection::SectionFooter)
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
  //  Set the open flag
  _isOpen = true;
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
  //  Set the open flag
  _isOpen = true;
}

void MultiFileWriter::close()
{
  if (_type == MultiFileWriterType::SingleBuffer) //  Only need to close the string buffer
    _stringBuffer.close();
  else
  {
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
  //  Set the open flag to closed
  _isOpen = false;
}

}
