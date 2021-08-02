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
#ifndef OSM_API_READER_H
#define OSM_API_READER_H

// Hoot
#include <hoot/core/elements/Tags.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/ParallelBoundedApiReader.h>
#include <hoot/core/util/Units.h>

// Qt
#include <QHash>
#include <QString>

namespace hoot
{

class OsmMap;
class Element;

/**
 * Reads OSM XML output from an OSM API map call by bounding box.  Reading will fail without a bounding box
 * https://wiki.openstreetmap.org/wiki/API_v0.6#Retrieving_map_data_by_bounding_box:_GET_.2Fapi.2F0.6.2Fmap
 */
class OsmApiReader : public OsmXmlReader, private ParallelBoundedApiReader
{
public:

  static QString className() { return "hoot::OsmApiReader"; }
  /** Constructor */
  OsmApiReader();
  /** Destructor */
  ~OsmApiReader();
  /**
   * @brief close Close the reader
   */
  void close() override;
  /**
   * @brief isSupported
   * @param url URL of the OSM API to read
   * @return True if the URL is a supported format
   */
  bool isSupported(const QString& url) override;
  /**
   * @brief read Read the OSM from the API into the map
   * @param map Pointer to the map to read into
   */
  void read(const OsmMapPtr& map) override;
  /**
   * @brief setUseDataSourceIds
   * @param useDataSourceIds
   */
  void setUseDataSourceIds(bool useDataSourceIds) override;
  /**
   * @brief supportedFormats
   * @return the supported formats
   */
  QString supportedFormats() override;
  /**
   * @brief setConfiguration
   * @param conf Updated configuration
   */
  void setConfiguration(const Settings& conf) override;

private:

  /**
   * @brief _getBoundsEnvelope - Get either the `bounds` parameter value
   *  or the bounds of a file listed in `bounds.file` parameter
   * @return The bounds for the read
   */
  std::shared_ptr<geos::geom::Geometry> _getBoundsEnvelope() const;

  /** Value of the bounds envelope string */
  QString _boundsString;
  /** Value of the bounds filename string */
  QString _boundsFilename;
};

}

#endif // OSM_API_READER_H
