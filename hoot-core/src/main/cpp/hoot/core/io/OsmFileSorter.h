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
 * @copyright Copyright (C) 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef OSMFILESORTER_H
#define OSMFILESORTER_H

// Qt
#include <QString>
#include <QTemporaryFile>

namespace hoot
{

/**
 * Sorts OSM files by element type, then element ID.  Supports OSM XML, OSM PBF, OGR, and GeoNames
 * formats.
 *
 * This class uses the Unix sort command to sort the geonames input, which is possible due to the
 * single line records.  Osmosis is used for sorting OSM files.  Osmosis could be replaced with a
 * custom file based merge sort routine in the future to reduce the dependency on it, but
 * for now, using it is the best solution.  There is no good solution yet for sorting OGR inputs.
 * Those inputs must be converted to an OSM format before sorting, which unfortunately roughly
 * doubles the input parsing time.
 */
class OsmFileSorter
{

public:

  /**
   * Reads in the input and writes a file sorted by OSM element type, then element ID
   *
   * @param input an OSM, OGR, or GeoNames file
   * @param output the output file path; for OSM and GeoNames inputs, the output extension must
   * match; for OGR inputs, the output format must be OSM PBF
   */
  static void sort(const QString input, const QString output);

private:

  static void _checkForOsmosis();
  static void _sortPbf(const QString input, const QString output);
  static boost::shared_ptr<QTemporaryFile> _ogrToPbfTemp(const QString input);

};

}

#endif // OSMFILESORTER_H
