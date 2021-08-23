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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef OSMPGCSVWRITER_H
#define OSMPGCSVWRITER_H

// hoot
#include <hoot/core/util/Configurable.h>
#include <hoot/core/io/PartialOsmMapWriter.h>

// Qt

#include <QTextStream>

// std


namespace hoot
{

class OsmPgCsvWriter : public PartialOsmMapWriter, public Configurable
{
public:
  static QString className() { return "hoot::OsmCsvWriter"; }

  OsmPgCsvWriter();
  ~OsmPgCsvWriter() = default;

  /**
   * @brief isSupported returns true if the URL is likely supported
   * @param url Filename ending in ".pgcsv"
   * @return
   */
  bool isSupported(const QString& url) override { return url.toLower().endsWith(".pgcsv"); }
  /**
   * @brief supportedFormats
   * @return
   */
  QString supportedFormats() override { return ".pgcsv"; }
  /**
   * @brief open
   * @param url
   */
  void open(const QString& url) override;
  /**
   * @brief close
   */
  void close() override;
  /**
   * @brief toString Write map to one large string containing all PGCSV files,
   * @param map Pointer to map object to write PGCSV data to a string
   * @return
   */
  static QString toString(const ConstOsmMapPtr& map);
  /**
   * @brief write Write map to set of PGCSV files
   * @param map Pointer to map object to write to PGCSV
   */
  void write(const ConstOsmMapPtr& map) override;
  /**
   * @brief writePartial Write a single node/way/relation to the correct stream
   * @param n/w/r - Pointer to the node/way/relation to write to PGCSV
   */
  void writePartial(const ConstNodePtr& n) override;
  void writePartial(const ConstWayPtr& w) override;
  void writePartial(const ConstRelationPtr& r) override;
  /**
   * @brief finalizePartial Finalize the map write
   */
  void finalizePartial() override;
  /**
   * @brief setConfiguration allows configuration settings to override the defaults
   * @param conf Configuration settings object
   */
  void setConfiguration(const Settings& conf) override;

private:
  /**
   * @brief _initFiles Initialize the file headers
   */
  void _initFiles();
  /**
   * @brief _getTags Convert the tags to `hstore` formatted text
   * @param e Pointer to the element
   * @return `hstore` formatted string of tags
   */
  QString _getTags(const ConstElementPtr& e) const;

  /** Decimal precision for output from configuration */
  int _precision;
  /** Configurable field separator, defaults to comma */
  QString _separator;
  /** Configurable end of line, defaults to '\n' */
  QString _endl;
  /**
   * @brief The FileType enum - File type indices for stream and file arrays below (note type int)
   */
  enum FileType : int
  {
    Nodes = 0,
    Ways,
    WayNodes,
    Relations,
    RelationMembers,
    MaxFileType
  };
  /** Array of streams, one for each output file type */
  std::array<QTextStream, FileType::MaxFileType> _streams;
  /** Array of files, one for each output file type */
  std::array<std::shared_ptr<QFile>, FileType::MaxFileType> _files;
};

}

#endif  //  OSMPGCSVWRITER_H
