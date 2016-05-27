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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.db.postgres;

import java.util.HashMap;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.commons.lang3.StringUtils;
import org.postgresql.util.PGobject;


/**
 * Postgres specific utilities
 */
public class PostgresUtils {
    /**
     * Converts an hstore Postgres object to a string map
     * 
     * @param postgresObj
     *            a Postgres object containing an hstore
     * @return a string map with the hstore's data
     * @throws Exception
     */
    public static Map<String, String> postgresObjToHStore(final PGobject postgresObj) throws Exception {
        // type = hstore
        // value = "key 1"=>"val 1", "key 2"=>"val 2"

        Map<String, String> hstore = new HashMap<String, String>();

        if (postgresObj != null && postgresObj.getValue() != null
                && StringUtils.trimToNull(postgresObj.getValue()) != null) {
            String tagsStr = postgresObj.getValue();
            hstore = _parseTags(tagsStr);
        }
        return hstore;
    }

    protected static final Map<String, String> _parseTags(final String tagsStr) throws Exception {
        Map<String, String> tagsMap = new HashMap<String, String>();

        if (tagsStr != null && tagsStr.length() > 0) {
            Pattern regex = Pattern.compile("(\"[^\"]*\")=>(\"(?:\\\\.|[^\"\\\\]+)*\"|[^,\"]*)");
            Matcher regexMatcher = regex.matcher(tagsStr);
            while (regexMatcher.find()) {
                String key = regexMatcher.group(1);
                key = StringUtils.removeStart(key, "\"");
                key = StringUtils.removeEnd(key, "\"");
                String val = regexMatcher.group(2);
                val = StringUtils.removeStart(val, "\"");
                val = StringUtils.removeEnd(val, "\"");
                tagsMap.put(key, val);
            }
        }

        return tagsMap;
    }

}
