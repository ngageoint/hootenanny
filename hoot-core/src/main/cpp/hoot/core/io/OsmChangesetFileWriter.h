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
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef OSM_CHANGESET_FILE_WRITER_H
#define OSM_CHANGESET_FILE_WRITER_H

// Hoot
#include <hoot/core/util/Configurable.h>
#include <hoot/core/algorithms/changeset/ChangesetProvider.h>

namespace hoot
{

/**
 * Interface for classes which write changesets to a file
 *
 * There may be some redundancy with this and OsmChangeWriter.
 */
class OsmChangesetFileWriter : public Configurable
{
public:

  static std::string className() { return "hoot::OsmChangesetFileWriter"; }

  virtual ~OsmChangesetFileWriter() {}

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
  virtual void write(const QString& path, const QList<ChangesetProviderPtr>& changesetProviders) = 0;

  /**
   * Returns a changeset statistics as a table string
   *
   * @return a changeset statistics string
   */
  virtual QString getStatsTable() const = 0;

  /**
   * Determines if the output location is supported by this writer
   *
   * @param output the output path to validate
   * @return true if the output path is supported by this writer; false otherwise
   */
  virtual bool isSupported(const QString& output) const = 0;

  /**
   * @see Configurable
   */
  virtual void setConfiguration(const Settings& conf) = 0;
};

}

#endif // OSM_CHANGESET_FILE_WRITER_H
