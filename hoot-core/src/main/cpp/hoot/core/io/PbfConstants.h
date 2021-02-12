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
 * @copyright Copyright (C) 2015 Maxar (http://www.maxar.com/)
 */

#ifndef PBFCONSTANTS_H
#define PBFCONSTANTS_H

#define PBF_OSM_DATA "OSMData"
#define PBF_OSM_HEADER "OSMHeader"
#define PBF_OSM_SCHEMA_V06 "OsmSchema-V0.6"
#define PBF_DENSE_NODES "DenseNodes"
#define PBF_SORT_TYPE_THEN_ID "Sort.Type_then_ID"

// Don't use a raw size greater than 30MB. This should make the compressed block size in the
// 16MB range.
#define MAX_RAW_SIZE (30 * 1024 * 1024);

#endif // PBFCONSTANTS_H
