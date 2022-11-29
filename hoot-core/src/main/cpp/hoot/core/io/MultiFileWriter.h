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
#ifndef MULTI_FILE_WRITER_H
#define MULTI_FILE_WRITER_H

// Qt
#include <QBuffer>
#include <QFile>
#include <QString>
#include <QTextStream>

// Std
#include <map>
#include <memory>
#include <vector>

namespace hoot
{

/**
 *  An encasulated object for writing to 1 or more files or to a memory buffer
 */
class MultiFileWriter
{
public:
  /** Enumeration controlling the type of  */
  enum MultiFileWriterType
  {
    SingleFile,
    SingleBuffer,
    MultiThematic
  };
  /** Enumeration controlling the location of the write */
  enum MultiFileWriterSection
  {
    SectionHeader,
    SectionBody,
    SectionFooter
  };

  static QString className() { return "MultiFileWriter"; }

  /** Constructor */
  MultiFileWriter(MultiFileWriterType type = MultiFileWriterType::SingleFile);
  ~MultiFileWriter();

  /** Setters for configuration of writer object */
  void setWriterType(MultiFileWriterType type);
  void setCurrentFileIndex(const QString& index);
  void setCurrentSection(MultiFileWriterSection section) { _currentSection = section; }
  void setHeaderSection() { _currentSection = SectionHeader; }
  void setFooterSection() { _currentSection = SectionFooter; }
  void setBodySection() { _currentSection = SectionBody; }
  /** Get the current file index */
  QString getCurrentFileIndex() const { return _currentIndex; }
  bool isCurrentIndexWritten() const;
  /** Writer functions */
  void write(const QString& contents);
  void writeHeader(const QString& contents);
  void writeFooter(const QString& contents);
  /** Get the value of the output written to a memory buffer, must be MultiFileWriterType::SingleBuffer */
  QString getBuffer() const;
  /** Open the object for a memory buffer */
  void open();
  /** Open the object for 1 or more file output */
  void open(const QString& filename);
  /** Close all file/memory handles */
  void close();
  /** Check if the object is open */
  bool isOpen() const { return _isOpen; }

private:
  /** Type of output, memory buffer, single, or multiple files */
  MultiFileWriterType _type;
  /** Current section to write to, header, body or footer */
  MultiFileWriterSection _currentSection;
  /** Base file path for output, for multi-file objects, the index string will be inserted into the filename
   *  i.e. TestFile.json will become TestFile-.json, TestFile-Lines.json, and TestFile-Polygons.json
   *  for MultiFileWriterType::MultiThematic objects.
   */
  QString _filePath;
  /** Map of indices with device pointers used for changing between files for each element */
  std::map<QString, QIODevice*> _deviceMap;
  /** Map of indices with flag if the file has been written to yet */
  std::map<QString, bool> _writtenMap;
  /** Pointers to file objects, one for each index */
  std::vector<std::shared_ptr<QFile>> _files;
  /** Pointer to current device */
  QIODevice* _currentDevice;
  /** Current index */
  QString _currentIndex;
  /** Buffer for memory buffer output */
  QBuffer _stringBuffer;
  /** String buffers for file header and footer */
  QString _fileHeader;
  QString _fileFooter;
  /** Text streams for writing to the header and footer string buffers */
  QTextStream _header;
  QTextStream _footer;
  /** Open flag */
  bool _isOpen;
};

}

#endif // OSMJSONWRITER_H
