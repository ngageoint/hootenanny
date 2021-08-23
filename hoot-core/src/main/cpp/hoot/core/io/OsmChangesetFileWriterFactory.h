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
#ifndef OSM_CHANGESET_FILE_WRITER_FACTORY_H
#define OSM_CHANGESET_FILE_WRITER_FACTORY_H

// Qt


namespace hoot
{
class OsmChangesetFileWriter;

/**
 * A factory for constructing changeset file writers based on the URL (Singleton).
 */
class OsmChangesetFileWriterFactory
{
public:

  static QString className() { return "hoot::OsmChangesetFileWriterFactory"; }

  /**
   * Creates a writer capable of writing OSM changesets to a file
   *
   * @param url the path the writer writes to
   * @param osmApiDbUrl URL to an OSM API database used to calculate element IDs; required only if
   * the output changeset is of type .osc.sql
   * @return an OsmChangesetFileWriter
   */
  std::shared_ptr<OsmChangesetFileWriter> createWriter(const QString& url,
                                                       const QString& osmApiDbUrl = "") const;

  /**
   * Returns a Singleton instance of the factory
   *
   * @return an OsmChangesetFileWriterFactory Singleton instance
   */
  static OsmChangesetFileWriterFactory& getInstance();

private:

  /** Default constructor/destructor */
  OsmChangesetFileWriterFactory() = default;
  ~OsmChangesetFileWriterFactory() = default;
  /** Delete copy constructor and assignment operator */
  OsmChangesetFileWriterFactory(const OsmChangesetFileWriterFactory&) = delete;
  OsmChangesetFileWriterFactory& operator=(const OsmChangesetFileWriterFactory&) = delete;
};

}

#endif // OSM_CHANGESET_FILE_WRITER_FACTORY_H
