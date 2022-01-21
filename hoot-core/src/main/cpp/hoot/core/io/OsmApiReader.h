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

namespace hoot
{

/**
 * Reads OSM XML output from an OSM API map call by bounding box or bounding polygon.  Reading will fail without
 * bounds.  When using a bounding polygon, the bounding box for the polygon will be used in API calls and then
 * subsequently filter the resulting map on the polygon afterwards
 * https://wiki.openstreetmap.org/wiki/API_v0.6#Retrieving_map_data_by_bounding_box:_GET_.2Fapi.2F0.6.2Fmap
 */
class OsmApiReader : public OsmXmlReader, private ParallelBoundedApiReader
{
public:

  static QString className() { return "OsmApiReader"; }
  /** Constructor */
  OsmApiReader();
  /** Destructor */
  ~OsmApiReader() override;
  /**
   * @brief close Close the reader
   */
  void close() override;
  /**
   * @brief isSupported
   * @param url URL of the OSM API to read
   * @return True if the URL is a supported format
   */
  bool isSupported(const QString& url) const override;
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
  QString supportedFormats() const override;
  /**
   * @brief setConfiguration
   * @param conf Updated configuration
   */
  void setConfiguration(const Settings& conf) override;
  /**
   * See `Boundable` class
   */
  void setBounds(std::shared_ptr<geos::geom::Geometry> bounds) override;
  void setBounds(const geos::geom::Envelope& bounds) override;

private:
  /**
   * @brief _loadBounds - Get either the `bounds` parameter value
   *  or the bounds of a file listed in `bounds.file` parameter
   * @return true if the bounds were parsed correctly
   */
  bool _loadBounds();

  /** Value of the bounds envelope string */
  QString _boundsString;
  /** Value of the bounds filename string */
  QString _boundsFilename;
  /** Is the bounds a polygon */
  bool _isPolygon;
  /** Value of the bounding box or polygon */
  std::shared_ptr<geos::geom::Geometry> _boundingPoly;
  /** For testing */
  friend class OsmApiReaderTest;
};

}

#endif // OSM_API_READER_H
