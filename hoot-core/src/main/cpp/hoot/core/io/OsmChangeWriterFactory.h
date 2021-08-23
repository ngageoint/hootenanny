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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef OSMCHANGEWRITERFACTORY_H
#define OSMCHANGEWRITERFACTORY_H

namespace hoot
{
class OsmChangeWriter;

/**
 * A factory for constructing changeset writers based on the URL (Singleton).
 */
class OsmChangeWriterFactory
{
public:

  /**
   * Creates a writer capable of streaming OSM changes
   *
   * @param url the path the writer writes to
   * @param elementPayloadFormat determines the format the elements are stored in; options are
   * 'json' or 'xml'
   * @return an OSM change writer
   */
  std::shared_ptr<OsmChangeWriter> createWriter(const QString& url,
                                                const QString& elementPayloadFormat = "json") const;

  /**
   * Determines if the output location is supported by any OsmChangeWriter
   *
   * @param output the output path to validate
   * @return true if the output path is supported by any writer; false otherwise
   */
  bool isSupported(const QString& output) const;

  /**
   * Returns a Singleton instance of OsmChangeWriterFactory
   *
   * @return
   */
  static OsmChangeWriterFactory& getInstance();

private:

  /** Default constructor/destructor */
  OsmChangeWriterFactory() = default;
  ~OsmChangeWriterFactory() = default;
  /** Delete copy constructor and assignment operator */
  OsmChangeWriterFactory(const OsmChangeWriterFactory&) = delete;
  OsmChangeWriterFactory& operator=(const OsmChangeWriterFactory&) = delete;
};

}

#endif // OSMCHANGEWRITERFACTORY_H
