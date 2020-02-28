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

#ifndef METADATAEXPORT_H
#define METADATAEXPORT_H

// Hoot
#include <hoot/core/ops/MetadataOp.h>
#include <hoot/core/elements/Way.h>

namespace hoot
{

/*
 * MetadataExport exports a specific set of tags collected from all elements into one or more
 * 'dataset' ways.
 *
 * A dataset is marked with the tag/value pair defined in the 'metadata.dataset.indicator.tag'
 * configuration variable.
 *
 * The specific set of tags and their default values are defined in the 'metadata.tags'
 * configuration variable. The list of key/value pairs specifies the tags that are to be
 * considered for export from the source elements and their default values (called MetadataTags).
 *
 * Process:
 * - Create a polygon grid or a single bounding polygon defining the new dataset object(s) and
 *   Mark them with the DatasetTag.
 * - Find all (non-dataset) way and node elements which have one or more non-debug tags and find
 *   out which dataset contains them. This uses the same logic as in import
 *   (MetadataOp::assignToDataset).
 * - From each element copy the MetadataTags to the dataset. If there are MetadataTags with the
 *   same key but different values, add them to the respective dataset tag in a semicolon
 *   delimited list.
 * - Make sure each dataset has all DatasetTags assigned. Fill in the missing ones with the
 *   default values defined in the config.
 */
class MetadataExport : public MetadataOp
{
public:

  static std::string className() { return "hoot::MetadataExport"; }

  // OsmMapOperation
  virtual QString getDescription() const override { return "Creates and exports metadata"; }

  virtual std::string getClassName() const { return className(); }

private:

  // MetadataOp
  virtual void _apply();

  // private data
  QList<WayPtr> _modifiedDatasets;

  // process sequence functions

  // creates the dataset cells or bounding polygon
  void _createCells();

  // copies the metadata tags from the elements to the corresponding datasets
  void _exportMetadataFromElements();

  // helper functions
  long _addNodeToPoly(double x, double y, WayPtr& pPoly);
};

}

#endif // METADATAEXPORT_H
