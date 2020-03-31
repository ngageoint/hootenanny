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

#ifndef METADATAIMPORT_H
#define METADATAIMPORT_H

// Hoot
#include <hoot/core/ops/MetadataOp.h>

// geos


namespace hoot
{

/*
 * MetadataImport imports a specific set of tags from one as 'dataset' specified way to all
 * elements containd in it.
 *
 * A dataset is defined by the tag/value pair defined in the 'metadata.dataset.indicator.tag'
 * configuration variable. The key/value pair defines the tag and value expected from the
 * element that is to be considered the dataset (called DatasetTag).
 *
 * The specific set of tags and their default values are defined in the 'metadata.tags'
 * configuration variable. The list of key/value pairs specifies the tags that are to be
 * considered for import from the dataset element and their default values (called MetadataTags).
 *
 * Process:
 * - Find all ways marked as datasets (meaning they contain the matching DatasetTag) and convert
 *   them to geos::geom::Polygons.
 * - Merge all resulting Polygons whose MetadataTags values are identical into one or more
 *   DatasetPolygons.
 * - Find all (non-dataset) way and node elements which have one or more non-debug tags and check
 *   if they are contained in the DatasetPolygon. If an element is overlapping more than one
 *   DatasetPolygon assign it to the one that contains the greatest number of its nodes. This way
 *   processing is basically the same for all element types.
 * - Copy all tags and values defined in the MetadataTags list from the DatasetPolygon to the
 *   element. If one or more MetadataTags are not specified in the DatasetPolygon assign it using
 *   its default value as specified in the ConfigOptions.
 */
class MetadataImport : public MetadataOp
{
public:

  static std::string className() { return "hoot::MetadataImport"; }

  // OsmMapOperation
  virtual QString getDescription() const override { return "Imports metadata"; }

  virtual std::string getClassName() const { return className(); }

private:

  // MetadataOp
  virtual void _apply();

  // process sequence functions

  // finds all way polygons that are marked as datasets
  void _findDatasetWays();

  // merge all geos geometries/polygons of datasets which have the exact same Metadata tags
  // and values
  void _mergePolygonsWithMatchingMetadata();

  // copies all metadata tags from the dataset to the elements
  void _importMetadataToElements();

  // helper functions
  bool _areMetadataTagsEqual(ElementPtr p1, ElementPtr p2);
  bool _applyToElement(ElementPtr pElement);
};

}

#endif // METADATAIMPORT_H
