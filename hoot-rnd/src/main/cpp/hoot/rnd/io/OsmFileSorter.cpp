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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#include "OsmFileSorter.h"

// Hoot
#include <hoot/core/io/OsmPbfReader.h>
#include <hoot/core/io/OsmPbfWriter.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OgrReader.h>
#include <hoot/core/io/GeoNamesReader.h>
#include <hoot/core/io/ElementStreamer.h>
#include <hoot/core/util/ConfigOptions.h>

// Qt
#include <QDir>
#include <QThread>

namespace hoot
{

void OsmFileSorter::sort(const QString& input, const QString& output)
{
  //I believe Osmosis handles parallelization automatically, so this is to be passed to the Unix
  //sort command used by the GeoNames format only.
  const int sortParallelCount = QThread::idealThreadCount();

  if (GeoNamesReader().isSupported(input))
  {
    //sort the input by node id (first field) using the unix sort command
    if (std::system(
         QString(
           "sort --parallel=" + QString::number(sortParallelCount) + " -n " + input +
           " --output=" + output).toStdString().c_str()) != 0)
    {
      throw HootException("Unable to sort input file.");
    }
  }
  //use osmosis to sort the OSM files by node id
  else if (OsmPbfReader().isSupported(input))
  {
    //check for sorted flag
    if (OsmPbfReader().isSorted(input))
    {
      LOG_WARN(
        "OSM PBF input file: " << input << " is marked as sorted by node ID, as " <<
        "indicated by its header, yet Hootenanny was instructed to sort the file.");
    }
    _sortPbf(input, output);
  }
  else if (OsmXmlReader().isSupported(input))
  {
    _checkForOsmosis();

    const QString cmd =
      "osmosis -q --read-xml file=\"" + input + "\" --sort --write-xml file=\"" +
      output + "\"  > /dev/null";
    if (std::system(cmd.toStdString().c_str()) != 0)
    {
      throw HootException("Unable to sort OSM XML file.");
    }
  }
  else if (OgrReader().isSupported(input))
  {
    if (!OsmPbfWriter().isSupported(output))
    {
      throw HootException("OGR files must be output to OSM PBF format during sorting.");
    }

    //Unfortunately for now, sorting an OGR input is going to require an extra pass over the data
    //to first write it to a sortable format.
    LOG_WARN("OGR inputs must be converted to the OSM format before sorting by node ID.");
    LOG_WARN("Converting input to OSM format...");
    _sortPbf(_ogrToPbfTemp(input)->fileName(), output);
  }
  else
  {
    throw HootException("Unsupported input format for node sorting.");
  }
}

void OsmFileSorter::_checkForOsmosis()
{
  if (std::system(QString("osmosis -q > /dev/null").toStdString().c_str()) != 0)
  {
    throw HootException(
      QString("Unable to access the Osmosis application.  Osmosis is required to") +
      QString("sort OSM PBF and OSM XML files.  Is Osmosis installed?"));
  }
}

void OsmFileSorter::_sortPbf(const QString& input, const QString& output)
{
  _checkForOsmosis();

  const QString cmd =
    "osmosis -q --read-pbf file=\"" + input + "\" --sort --write-pbf " +
    "omitmetadata=true file=\"" + output + "\" > /dev/null";
  if (std::system(cmd.toStdString().c_str()) != 0)
  {
    throw HootException("Unable to sort OSM PBF file.");
  }

  //this isn't working...can't update the sorted header yet - #2207
  //OsmPbfWriter::updateSorted(output, true);
}

std::shared_ptr<QTemporaryFile> OsmFileSorter::_ogrToPbfTemp(const QString& input)
{
  std::shared_ptr<QTemporaryFile> pbfTemp(
    new QTemporaryFile(
      ConfigOptions().getApidbBulkInserterTempFileDir() +
      "/multiary-ingest-sort-temp-XXXXXX.osm.pbf"));
  //for debugging only
  //pbfTemp->setAutoRemove(false);
  if (!pbfTemp->open())
  {
    throw HootException("Unable to open sort temp file: " + pbfTemp->fileName() + ".");
  }

  ElementStreamer::stream(input, pbfTemp->fileName());

  return pbfTemp;
}

}
