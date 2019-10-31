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
#ifndef OSM_CHANGESET_FILE_WRITER_FACTORY_H
#define OSM_CHANGESET_FILE_WRITER_FACTORY_H

// Qt
#include <QString>

namespace hoot
{
class OsmChangesetFileWriter;

/**
 * A factory for constructing changeset file writers based on the URL (Singleton).
 */
class OsmChangesetFileWriterFactory
{
public:

  /**
   * TODO
   *
   * @param url
   * @param osmApiDbUrl
   * @return
   */
  std::shared_ptr<OsmChangesetFileWriter> createWriter(const QString& url,
                                                       const QString& osmApiDbUrl = "");

  /**
   * TODO
   *
   * @return
   */
  static OsmChangesetFileWriterFactory& getInstance();

private:

  OsmChangesetFileWriterFactory();

  static std::shared_ptr<OsmChangesetFileWriterFactory> _theInstance;
};

}

#endif // OSM_CHANGESET_FILE_WRITER_FACTORY_H
