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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.utils;

import java.util.HashMap;
import java.util.Map;


/**
 * Postgres specific utilities
 */
public final class PostgresUtils {

    private PostgresUtils() {}

    /**
     * Converts an hstore Postgres object to Map<String, String>
     * 
     * @param postgresObj
     *            a Postgres object containing an hstore

     * @return a string map with the hstore's data
     */
    public static Map<String, String> postgresObjToHStore(Object postgresObj) {
        // type = hstore
        // value = "key 1"=>"val 1", "key 2"=>"val 2"

        Map<String, String> hstore = (Map<String, String>) postgresObj;

        if (hstore == null) {
            hstore = new HashMap<>();
        }

        return hstore;
    }
}
