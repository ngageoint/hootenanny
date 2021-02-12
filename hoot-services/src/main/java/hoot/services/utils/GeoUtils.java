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
 * @copyright Copyright (C) 2016, 2017, 2018 Maxar (http://www.maxar.com/)
 */
package hoot.services.utils;

import org.apache.commons.math3.util.Precision;


/**
 * Various geospatial utilities for hoot services
 */
public final class GeoUtils {
    public static final double DEFAULT_COORD_VALUE = -181.0;

    private GeoUtils() {
    }

    /**
     * Determines if a pair of coordinates lie within world boundaries
     * 
     * @param lat
     *            latitude coordinate
     * @param lon
     *            longitude coordinate
     * @return true if both coordinates lie within world boundaries; false
     *         otherwise
     */
    public static boolean coordsInWorld(double lat, double lon) {
        return (lat >= -90) && (lat <= 90) && (lon >= -180) && (lon <= 180);
    }

    /**
     * Calculates the quad tile for a point
     *
     * @param latitude
     *            point's latitude
     * @param longitude
     *            points longitude
     * @return tile integer
     */
    public static long tileForPoint(double latitude,double longitude) {
        int lonInt = (int) Precision.round((((longitude + 180.0) * 65535.0) / 360.0), 0);
        int latInt = (int) Precision.round((((latitude + 90.0) * 65535.0) / 180.0), 0);

        // use a long here, because java doesn't have unsigned int
        long tileUnsigned = 0;
        for (int i = 15; i >= 0; i--) {
            // use y, x ordering
            tileUnsigned = (tileUnsigned << 1) | ((lonInt >> i) & 1);
            tileUnsigned = (tileUnsigned << 1) | ((latInt >> i) & 1);
        }

        return tileUnsigned;
    }
}
