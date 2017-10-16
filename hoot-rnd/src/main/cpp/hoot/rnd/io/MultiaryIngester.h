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

// Qt
#include <QElapsedTimer>
#include <QTemporaryFile>

namespace hoot
{

class ElementInputStream;

/**
 * This class ingests data into the Multiary POI conflation workflow.  It takes a supported
 * data input, a reference database layer, a changeset output location, and an optional translation
 * script.  The input is filtered down to POIs only and translated to OSM, then sorted by element
 * ID (if not already).  Finally, the input data is then compared to the database ref layer in
 * order to derive the difference between the two in the form of a changeset.  The changeset
 * changes are written both to the reference layer as features and to a changeset output file as
 * change statements for later use by Spark.  Alternatively, if the specified database reference
 * layer is empty, no changeset is derived and the entire contents of the data input are simply
 * written directly to the reference layer.
 *
 * This class requires that the input be a streamable format, the output layer be a Hootenanny
 * API database layer, and the changeset output format be a Spark changeset.
 */
class MultiaryIngester
{

public:

  MultiaryIngester();

  /**
   * Ingests multiary POI data
   *
   * @param newInput path to the input data; must be a streamable format
   * @param translationScript script used to translate the input data into OSM
   * @param referenceOutput path to the output data; must be a Hootenanny API database
   * @param changesetOuput path to the output changeset; must be a Spark changeset
   */
  void ingest(const QString newInput, const QString translationScript,
              const QString referenceOutput, const QString changesetOuput);

private:

  QString _sortedNewInput;
  boost::shared_ptr<QTemporaryFile> _sortTempFile;

  long _logUpdateInterval;

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

  void _sortInputFile(const QString input);

  void _doInputErrorChecking(const QString newInput, const QString translationScript,
                             const QString referenceOutput, const QString changesetOutput);
};

}

#endif // MULTIARYINGESTER_H
