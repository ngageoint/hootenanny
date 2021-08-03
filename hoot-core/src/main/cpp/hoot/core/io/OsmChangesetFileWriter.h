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
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef OSM_CHANGESET_FILE_WRITER_H
#define OSM_CHANGESET_FILE_WRITER_H

// Hoot
#include <hoot/core/util/Configurable.h>
#include <hoot/core/algorithms/changeset/ChangesetProvider.h>
#include <hoot/core/io/ChangesetStatsFormat.h>
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

/**
 * Interface for classes which write changesets to a file
 *
 * @todo There may be some redundancy with this and OsmChangeWriter.
 */
class OsmChangesetFileWriter : public Configurable
{

public:

  static QString className() { return "hoot::OsmChangesetFileWriter"; }

  OsmChangesetFileWriter();
  virtual ~OsmChangesetFileWriter() = default;

  /**
   * Write the changeset out to the specified file
   *
   * @param path Pathname for the output file(s)
   * @param changesetProvider changeset provider to stream the changes from
   */
  virtual void write(const QString& path, const ChangesetProviderPtr& changesetProvider) = 0;
  /**
   * Write the changeset out to the specified file
   *
   * @param path Pathname for the output file(s)
   * @param changesetProviders changeset providers to stream the changes from
   */
  virtual void write(const QString& path,
                     const QList<ChangesetProviderPtr>& changesetProviders) = 0;

  /**
   * Returns a changeset statistics as a table string
   *
   * @param format statistics format
   * @return a changeset statistics string
   */
  virtual QString getStatsTable(
    const ChangesetStatsFormat& format = ChangesetStatsFormat::Text) const = 0;

  /**
   * Determines if the output location is supported by this writer
   *
   * @param output the output path to validate
   * @return true if the output path is supported by this writer; false otherwise
   */
  virtual bool isSupported(const QString& output) const = 0;

  /**
   * Sets all maps corresponding to the former state of the datasets
   *
   * @todo These map setters feel a little kludgy...maybe use a new map list interface, similar to
   * OsmMapConsumer?
   */
  virtual void setMap1List(const QList<ConstOsmMapPtr>& /*map*/) {}
  /**
   * Sets all maps corresponding to the changed state of the datasets
   */
  virtual void setMap2List(const QList<ConstOsmMapPtr>& /*map*/) {}

protected:

  // used for bounds checking
  QList<ConstOsmMapPtr> _map1List;
  QList<ConstOsmMapPtr> _map2List;

  bool _includeDebugTags;
  bool _includeCircularErrorTags;

  // list of metadata tag keys allowed to be written to the changeset
  QStringList _metadataAllowKeys;

  // overrides bounds checking
  bool _changesetIgnoreBounds;

  /*
   * Determines if an change element satisfies the configured bounds requirement
   *
   * @param element the element to check
   * @param map1 before changes map
   * @param map2 after changes map
   * @return true if the element passes the configured bounds requirement; false otherwise
   */
  bool _failsBoundsCheck(
    const ConstElementPtr& element, const ConstOsmMapPtr& map1, const ConstOsmMapPtr& map2) const;
};

}

#endif // OSM_CHANGESET_FILE_WRITER_H
