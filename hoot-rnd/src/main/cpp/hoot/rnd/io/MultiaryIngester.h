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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef MULTIARYINGESTER_H
#define MULTIARYINGESTER_H

// Hoot
#include <hoot/core/io/Change.h>
#include <hoot/core/io/HootApiDb.h>

// Qt
#include <QElapsedTimer>
#include <QTemporaryFile>

namespace hoot
{

class ElementInputStream;

/**
 * This class ingests data into the Multiary POI conflation workflow.  It takes a supported
 * data input, a reference database layer, and a changeset output location.  The input is
 * filtered down to POIs only and translated to OSM, then sorted by element ID (if not already).
 * Finally, the input data is then compared to the database ref layer in order to derive the
 * difference between the two in the form of a changeset.  The changeset changes are written both
 * to the reference layer as features and to a changeset output file as change statements for later
 * use by Spark.  Alternatively, if the specified database reference layer is empty, no changeset
 * is derived and the entire contents of the data input are simply written directly to the
 * reference layer.
 *
 * This class requires that the input be a streamable format, the output layer be a Hootenanny
 * API database layer, and the changeset output format be a Spark changeset.
 *
 * This class uses the Unix sort command to sort the geonames input, which is possible due to the
 * single line records.  Osmosis is used for sorting OSM files.  This could be replaced with a
 * custom file based merge sort routine in the future to reduce the dependency on Osmosis, but
 * for now using it is the best solution.  There is no good solution yet for sorting OGR inputs.
 * Those inputs must be converted to an OSM format before sorting, which unfortunately roughly
 * doubles the input parsing time.
 */
class MultiaryIngester
{

public:

  MultiaryIngester();

  /**
   * Ingests multiary POI data
   *
   * @param newInput path to the input data; must be a streamable format
   * @param referenceOutput path to the output data; must be a Hootenanny API database
   * @param changesetOuput path to the output changeset; must be a Spark changeset
   * @param sortInput if true, the input is sorted by element ID
   */
  void ingest(const QString newInput, const QString referenceOutput, const QString changesetOuput,
              const bool sortInput = true);

private:

  bool _sortInput;
  QString _sortedNewInput;
  boost::shared_ptr<QTemporaryFile> _sortTempFile;
  bool _addToExistingRefDb;

  long _changesParsed;
  long _logUpdateInterval;
  QMap<Change::ChangeType, long> _changesByType;
  long _numNodesBeforeApplyingChangeset;
  long _numNodesAfterApplyingChangeset;
  HootApiDb _referenceDb;

  QElapsedTimer _timer;

  boost::shared_ptr<ElementInputStream> _getFilteredNewInputStream(const QString sortedNewInput);

  /*
   * Writes data to the reference layer when no data exists there (no changeset derivation)
   */
  void _writeNewReferenceData(boost::shared_ptr<ElementInputStream> filteredNewInputStream,
                              const QString referenceOutput, const QString changesetOutput);

  /*
   * Derives a changeset between the ref layer and another source; writes the output to a changeset
   * file; returns the changeset file
   */
  boost::shared_ptr<QTemporaryFile> _deriveAndWriteChangesToChangeset(
    boost::shared_ptr<ElementInputStream> filteredNewInputStream, const QString referenceInput,
    const QString changesetOutput);

  /*
   * Writes the contents of a changeset to the ref layer
   */
  void _writeChangesToReferenceLayer(const QString changesetOutput, const QString referenceOutput);

  void _printSummary();
  void _clearChangeTypeCounts();

  void _sortInputFile(const QString input);
};

}

#endif // MULTIARYINGESTER_H
