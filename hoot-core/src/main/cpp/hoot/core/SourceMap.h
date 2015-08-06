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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef SOURCEMAP_H
#define SOURCEMAP_H

// Local Includes
class FeatureVector;

/**
 * This class represents a single source map used for conflation. This source map may be manipulated
 * to generate a new source map (e.g. vector generalization, merging ways, etc.). Similar to the
 * concepts in the RoadMatcher documentation the individual features may be in one of several
 * source states.
 */
class SourceMap
{
public:

    /**
     * Initializes the source map with a set of unknown vectors.
     */
    SourceMap(FeatureVector& fv);

    /**
     * Performs a deep copy of all SourceFeatures.
     */
    SourceMap(const SourceMap& sm);
};

#endif // SOURCEMAP_H
