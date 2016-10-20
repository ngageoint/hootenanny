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
package hoot.services.models.osm;

import static hoot.services.utils.DbUtils.createQuery;

import java.util.List;

import org.apache.commons.lang3.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.querydsl.core.types.dsl.NumberPath;
import com.querydsl.core.types.dsl.StringPath;
import com.querydsl.sql.RelationalPathBase;


public final class ModelDaoUtils {
    private static final Logger logger = LoggerFactory.getLogger(ModelDaoUtils.class);

    private ModelDaoUtils() {}

    /**
     * Returns the record ID associated with the record request input string for
     * the given DAO type. First attempts to parse the request string as a
     * record ID. If that is unsuccessful, it treats the request string as a
     * record display name. This currently only supports Map and User types.
     * 
     * @param input
     *            can be either a map ID or a map name
     * @return if a record ID string is passed in, it is verified and returned;
     *         if a record name string is passed in, it is verified that only
     *         one record of the requested type exists with the given name, and
     *         its ID is returned
     */
    public static long getRecordIdForInputString(String input, RelationalPathBase<?> table,
            NumberPath<Long> idField, StringPath nameField) {

        if (StringUtils.isEmpty(input)) {
            throw new IllegalArgumentException("No record exists with ID: " + input
                    + ".  Please specify a valid record.");
        }

        // Check if we can compare by ID
        if (StringUtils.isNumeric(input)) {
            logger.debug("Verifying that record with ID = {} in '{}' table has previously been created ...",
                    input, table.getTableName());

            long recordCount = createQuery()
                    .from(table)
                    .where(idField.eq(Long.valueOf(input)))
                    .fetchCount();

            if (recordCount == 0) {
                throw new IllegalArgumentException("No record exists with ID = " + input +
                        " in '" + table + "' table.  Please specify a valid record.");
            }

            if (recordCount == 1) {
                return Long.valueOf(input);
            }

            if (recordCount > 1) {
                throw new IllegalArgumentException("Multiple records exist with ID " + " = " + input
                        + " in '" + table + "' table.  Please specify a single, valid record.");
            }
        }
        else { // input wasn't parsed as a numeric ID, so let's try it as a name
            logger.debug("Verifying that record with NAME = {} in '{}' table has previously been created ...",
                    input, table.getTableName());

            // there has to be a better way to do this against the generated
            // code but haven't been able to get it to work yet
            List<Long> records = createQuery()
                    .select(idField)
                    .from(table)
                    .where(nameField.eq(input))
                    .fetch();

            if (records.isEmpty()) {
                throw new IllegalArgumentException("No record exists with NAME = " + input +
                        " in '" + table + "' table.  Please specify a valid record.");
            }

            if (records.size() == 1) {
                return records.get(0);
            }

            if (records.size() > 1) {
                throw new IllegalArgumentException("Multiple records exist with NAME " + " = " + input
                        + " in '" + table + "' table.  Please specify a single, valid record.");
            }
        }

        return -1;
    }
}
