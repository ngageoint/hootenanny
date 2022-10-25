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
 *
 */
class MultiFileWriter
{
public:

  enum MultiFileWriterType
  {
    SingleFile,
    SingleBuffer,
    MultiGeom,
    MultiFCode
  };

  enum MultiFileWriterSection
  {
    SectionHeader,
    SectionBody,
    SectionFooter
  };

  const static QString POINTS;
  const static QString LINES;
  const static QString POLYGONS;

  static QString className() { return "MultiFileWriter"; }

  MultiFileWriter(MultiFileWriterType type = MultiFileWriterType::SingleFile);
  ~MultiFileWriter();

  void setCurrentFileIndex(const QString& index);
  void setPointsIndex() { setCurrentFileIndex(POINTS); }
  void setLinesIndex() { setCurrentFileIndex(LINES); }
  void setPolygonsIndex() { setCurrentFileIndex(POLYGONS); }
  QString getCurrentFileIndex() const { return _currentIndex; }

  void write(const QString& contents, MultiFileWriterSection section = MultiFileWriterSection::SectionBody);
  void writeHeader(const QString& contents) { write(contents, MultiFileWriterSection::SectionHeader); }
  void writeFooter(const QString& contents) { write(contents, MultiFileWriterSection::SectionFooter); }

  QString getBuffer() const;

  void open();
  void open(const QString& filename);
  void close();

private:

  MultiFileWriterType _type;
  QString _filePath;

  std::map<QString, QIODevice*> _deviceMap;
  std::map<QString, bool> _devicesWritten;
  std::vector<std::shared_ptr<QFile>> _files;
  QIODevice* _currentDevice;
  QString _currentIndex;
  QBuffer _stringBuffer;

  QString _fileHeader;
  QString _fileFooter;
  QTextStream _header;
  QTextStream _footer;
};

}

#endif // OSMJSONWRITER_H
