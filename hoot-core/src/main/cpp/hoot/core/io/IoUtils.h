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

#ifndef IOUTILS_H
#define IOUTILS_H

// GEOS
#include <geos/geom/Geometry.h>

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/io/ElementVisitorInputStream.h>

// Qt
#include <QString>

namespace hoot
{

/**
 * General utilities related to I/O
 */
class IoUtils
{

public:

  static QString className() { return "hoot::IoUtils"; }

  /**
   * Determines if the a URL is a supported input format
   *
   * @param url the URL to examine
   * @return true if the URL points to a valid input format; false otherwise
   */
  static bool isSupportedInputFormat(const QString& url);

  /**
   * Determines if the a URL is a supported output format
   *
   * @param url the URL to examine
   * @return true if the URL points to a valid output format; false otherwise
   */
  static bool isSupportedOutputFormat(const QString& url);

  /**
   * Returns true if the input format is a Hootenanny supported OSM format
   *
   * @param input input path
   * @return true if the input is OSM and supported; false otherwise
   */
  static bool isSupportedOsmFormat(const QString& input);

  /**
   * Returns true if the input format is a Hootenanny supported OGR format
   *
   * @param input input path
   * @param allowDir if true; then a directory can be passed as an input
   * @return true if the input is supported by OGR; false otherwise
   */
  static bool isSupportedOgrFormat(const QString& input, const bool allowDir = false);

  /**
   * Determines if a set of inputs paths are all OGR supported formats
   *
   * @param inputs input paths
   * @param allowDir if true; then a directory can be passed as an input
   * @return true if all inputs passed in are supported by OGR; false otherwise
   */
  static bool areSupportedOgrFormats(const QStringList& inputs, const bool allowDir = false);

  /**
   * Determines if any in a set of inputs paths are OGR supported formats
   *
   * @param inputs input paths
   * @param allowDir if true; then a directory can be passed as an input
   * @return true if any of the inputs passed in are supported by OGR; false otherwise
   */
  static bool anyAreSupportedOgrFormats(const QStringList& inputs, const bool allowDir = false);

  /**
   * Returns all file paths under a directory point to a supported input format
   *
   * @param topLevelPaths one or more directory paths
   * @param nameFilters wildcard filters for path exclusion; e.g. *myFile*, *.json
   * @return a list of file paths
   */
  static QStringList getSupportedInputsRecursively(
    const QStringList& topLevelPaths, const QStringList& nameFilters = QStringList());

  /**
   * Determines whether both input and output are streamable data sources (associated
   * readers/writers must implemented the partial map interfaces)
   *
   * @param input data source
   * @param output data destination
   * @return true if both formats are streamable; false otherwise
   */
  static bool isStreamableIo(const QString& input, const QString& output);

  /**
   * Determines whether both inputs and output are streamable data sources (associated
   * readers/writers must implemented the partial map interfaces)
   *
   * @param inputs data sources
   * @param output data destination
   * @return true if all formats are streamable; false otherwise
   */
  static bool areStreamableIo(const QStringList& inputs, const QString& output);

  /**
   * Determines whether inputs and are streamable data sources (associated
   * readers/writers must implemented the partial map interfaces)
   *
   * @param inputs data sources
   * @param logUnstreamable if true, a message is logged to indicate the input being parsed is not
   * a streamable input
   * @return true if all inputs are streamable; false otherwise
   */
  static bool areStreamableInputs(const QStringList& inputs, const bool logUnstreamable = false);

  /**
   * Return true if all the specified operations are valid streaming operations.
   *
   * There are some ops that require the whole map be available in RAM (e.g. remove duplicate
   * nodes). These operations are not applicable for streaming.
   *
   * @param ops
   * @return
   */
  static bool areValidStreamingOps(const QStringList& ops);

  /**
   * Determines if a URL points to a valid streamable input
   *
   * @param url the URL to examine
   * @return true if the URL points to a valid streamable input; false otherwise
   */
  static bool isStreamableInput(const QString& url);

  /**
   * Determines if a URL points to a valid streamable output
   *
   * @param url the URL to examine
   * @return true if the URL points to a valid streamable output; false otherwise
   */
  static bool isStreamableOutput(const QString& url);

  /**
   * Get an input stream set up to be filtered by operations
   *
   * @param streamToFilter the stream to be filtered
   * @param ops a list of Hoot operation class names to use for inline filtering on the input stream
   * @return an input stream
   */
  static ElementInputStreamPtr getFilteredInputStream(
    ElementInputStreamPtr streamToFilter, const QStringList& ops);

  /**
    Loads an OSM map into an OsmMap object

    @param map the object to load the map into
    @param path the file path to load the map from
    @param useFileId if true, uses the element ID's in the map file; otherwise, generates new
    element ID's
    @param defaultStatus the hoot status to assign to all elements
    @param translationScript script used to translate data; required only if the input is an OGR
    format; ignored otherwise
    @param ogrFeatureLimit limit of features to read per input; applicable to OGR inputs only;
    ignored otherwise
    @param jobSource job name for status reporting; applicable to OGR inputs only; ignored otherwise
    @param numTasks number of job tasks being performed for status reporting; applicable to OGR
    inputs only; ignored otherwise
    */
  static void loadMap(
    const OsmMapPtr& map, const QString& path, bool useFileId = true,
    Status defaultStatus = Status::Invalid, const QString& translationScript = "",
    const int ogrFeatureLimit = -1, const QString& jobSource = "", const int numTasks = -1);

  /**
    Loads multiple OSM maps into an OsmMap object

    @param map the object to load the map into
    @param paths the file paths to load the maps from
    @param useFileId if true, uses the element ID's in the map file; otherwise, generates new
    element ID's
    @param defaultStatus the hoot status to assign to all elements
    */
  static void loadMaps(
    const OsmMapPtr& map, const QStringList& paths, bool useFileId,
    Status defaultStatus = Status::Invalid);

  /**
    Saves an OSM map to an OsmMap object

    @param map the map object to save
    @param path the file path to save the map to
   */
  static void saveMap(const OsmMapPtr& map, const QString& path);

  /**
   * Crops a map to a given bounds
   *
   * @param map the map to crop
   * @param bounds the bounds to crop to
   * @param keepConnectedOobWays if true any way falling outside of the bounds but directly
   * connected to a way within the bounds will be kept
   */
  static void cropToBounds(
    OsmMapPtr& map, const geos::geom::Envelope& bounds, const bool keepConnectedOobWays = false);

  /**
   * Crops a map to a given bounds
   *
   * @param map the map to crop
   * @param bounds the bounds to crop to
   * @param keepConnectedOobWays if true any way falling outside of the bounds but directly
   * connected to a way within the bounds will be kept
   */
  static void cropToBounds(
    OsmMapPtr& map, const std::shared_ptr<geos::geom::Geometry>& bounds,
    const bool keepConnectedOobWays = false);

  /**
   * Creates an input stream with a visitor in the loop
   *
   * @param input the data soruce to read
   * @param visitorClassName the class name of the visitor to use
   * @return an input stream
   */
  static std::shared_ptr<ElementVisitorInputStream> getVisitorInputStream(
    const QString& input, const QString& visitorClassName, const bool useDataSourceIds = false);

  /**
   * Determines if an input string is a URL by the hoot definition
   *
   * @param str string to examine
   * @return true if the input is a hoot URL, false otherwise
   */
  static bool isUrl(const QString& str);

  /**
   * Writes an output directory
   *
   * @param dirName name of the directory to write
   */
  static void writeOutputDir(const QString& dirName);

  /**
   * Determines if all specified URLs are supported readers implementing Boundable
   *
   * @param urls one or more input URLs
   * @return true if all of the URLs point to file formats whose readers support the Boundable
   * interface
   */
  static bool urlsAreBoundable(const QStringList& urls);
};

}

#endif // IOUTILS_H
