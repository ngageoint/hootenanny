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
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef CHANGESET_CREATOR_H
#define CHANGESET_CREATOR_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/ElementInputStream.h>
#include <hoot/core/util/Progress.h>

namespace hoot
{

/**
 * High level class for prepping data for changeset generation and then calls on appropriate
 * changeset file writers.
 *
 * Streaming I/O and external element can be used from here.  However, the in-memory input
 * reading/sorting has been left in place to support faster I/O in the situation where large inputs
 * are being dealt with and large amounts of memory are available for reading/sorting or when
 * conversion operations are passed in which require reading an entire map into memory. Access to
 * the the sorter implementation is controlled by the configuration option,
 * element.sorter.element.buffer.size where a size = -1 results in the in-memory implementation
 * being used and a positive size results in the external sorter being used.
 *
 * If convert operations are passed in and any are not streamable, then in memory sorting is forced
 * to occur. Its important to note that any map consuming operations (OsmMapOperation, etc.) must
 * operate over the entire map with data from both inputs (if there are two inputs). This requires
 * a different set of logic for convert ops handling when map consuming ops are present (with
 * in-memory sorting only).
 *
 * Everywhere we load data source element IDs are preserved. This is important for the changeset
 * to be derived correctly. If there are any overlapping element IDs and a map consuming op is used,
 * an error will occur. This is a limitation that we probably cannot overcome. In those situations,
 * one of the datasets should have IDs renumbered with a convert op beforehand, if possible. We
 * also could, at some point, make loading source IDs for datasets optional and could only be used
 * on one of the datasets if writing a changeset back to authoritative store (e.g. OpenStreetMap).
 */
class ChangesetCreator
{

public:

  static const QString JOB_SOURCE;

  /**
   * Constructor
   *
   * @param printStats prints statistics for the output changeset
   * @param statsOutputFile optional file to output the changeset statistics to
   * @param osmApiDbUrl URL to an OSM API database used to calculate element IDs; required only if
   * the output changeset is of type .osc.sql.
   */
  ChangesetCreator(
    const bool printDetailedStats = false, const QString& statsOutputFile = "",
    const QString osmApiDbUrl = "");

  /**
   * Writes a changeset between one or two inputs to an output file. If only one input is
   * specified, the resulting changeset will be made entirely of the elements from the input.
   *
   * @param output the changeset output file target
   * @param input1 the first input source
   * @param input2 the optional second input source
   */
  void create(const QString& output, const QString& input1, const QString& input2 = "");

  /**
   * Writes a changeset between two maps to an output file.
   *
   * @param map1 the first input source
   * @param map2 the second input source
   * @param output the changeset output file target
   */
  void create(OsmMapPtr& map1, OsmMapPtr& map2, const QString& output);

  /**
   * Writes a single combined changeset between multiple sets of maps to an output file. The number
   * of maps in each set must be equal to each other
   *
   * @param map1Inputs A set of maps with the original state of the data.
   * @param map2Inputs A set of maps with the new state of the data.
   * @param output the changeset output file target
   */
  void create(const QList<OsmMapPtr>& map1Inputs, const QList<OsmMapPtr>& map2Inputs,
              const QString& output);

  int getNumCreateChanges() const { return _numCreateChanges; }
  int getNumModifyChanges() const { return _numModifyChanges; }
  int getNumDeleteChanges() const { return _numDeleteChanges; }

private:

  friend class ChangesetReplacementCreatorTest;

  QString _osmApiDbUrl;

  int _numTotalTasks;
  int _currentTaskNum;

  bool _printDetailedStats;
  QString _statsOutputFile;

  // If true, we are generating a changeset that will be made up of everything in the single input
  // provided.
  bool _singleInput;

  int _numCreateChanges;
  int _numModifyChanges;
  int _numDeleteChanges;

  bool _isSupportedOutputFormat(const QString& format) const;
  bool _inputIsSorted(const QString& input) const;
  bool _inputIsStreamable(const QString& input) const;

  ElementInputStreamPtr _getEmptyInputStream();

  // IN MEMORY SORTING BASED METHODS

  /*
   * Reads both inputs into memory and populates maps for them
   */
  void _readInputsFully(const QString& input1, const QString& input2, OsmMapPtr& map1,
                        OsmMapPtr& map2, Progress progress);
  /*
   * Logic for dealing with map consuming convert ops
   */
  void _handleUnstreamableConvertOpsInMemory(const QString& input1, const QString& input2,
                                             OsmMapPtr& map1, OsmMapPtr& map2, Progress progress);
  /*
   * Logic for dealing with non-map consuming convert ops
   */
  void _handleStreamableConvertOpsInMemory(const QString& input1, const QString& input2,
                                           OsmMapPtr& map1, OsmMapPtr& map2, Progress progress);
  ElementInputStreamPtr _sortElementsInMemory(OsmMapPtr map);

  // EXTERNAL DISK SORTING BASED METHODS

  ElementInputStreamPtr _getExternallySortedElements(const QString& input, Progress progress);
  ElementInputStreamPtr _getFilteredInputStream(const QString& input);
  ElementInputStreamPtr _sortElementsExternally(const QString& input);

  /*
   * Runs the two data source streams through a changeset deriver
   */
  void _streamChangesetOutput(ElementInputStreamPtr input1, ElementInputStreamPtr input2,
                              const QString& output);
  void _streamChangesetOutput(const QList<ElementInputStreamPtr>& inputs1,
                              const QList<ElementInputStreamPtr>& inputs2, const QString& output);
};

}

#endif // CHANGESET_CREATOR_H
