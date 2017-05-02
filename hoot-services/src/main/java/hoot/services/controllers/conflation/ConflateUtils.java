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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.conflation;


final class ConflateUtils {

    private ConflateUtils() {}

    static boolean isAtLeastOneLayerOsmApiDb(ConflateParams conflationParams) {
        return isFirstLayerOsmApiDb(conflationParams) || isSecondLayerOsmApiDb(conflationParams);
    }

    static boolean isFirstLayerOsmApiDb(ConflateParams params) {
        return params.getInputType1().equalsIgnoreCase("OSM_API_DB");
    }

    static boolean isSecondLayerOsmApiDb(ConflateParams params) {
        return params.getInputType2().equalsIgnoreCase("OSM_API_DB");
    }

    static void validateOsmApiDbConflateParams(ConflateParams params) {
        // default REFERENCE_LAYER = 1
        if (params.getReferenceLayer() != null) {
            if ((isFirstLayerOsmApiDb(params) && params.getReferenceLayer().equalsIgnoreCase("2"))
                    || ((isSecondLayerOsmApiDb(params)) && params.getReferenceLayer().equalsIgnoreCase("1"))) {
                String msg = "OSM_API_DB is not allowed as secondary input type.";
                throw new IllegalArgumentException(msg);
            }
        }
        else if (isSecondLayerOsmApiDb(params)) {
            String msg = "OSM_API_DB is not allowed as secondary input type.";
            throw new IllegalArgumentException(msg);
        }
    }
}
