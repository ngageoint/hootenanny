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

import java.sql.Connection;
import java.util.List;

import org.apache.commons.lang3.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.mysema.query.sql.RelationalPathBase;
import com.mysema.query.sql.SQLQuery;
import com.mysema.query.types.path.NumberPath;
import com.mysema.query.types.path.StringPath;

import hoot.services.db.DbUtils;


public class ModelDaoUtils {
    private static final Logger log = LoggerFactory.getLogger(ModelDaoUtils.class);

    /**
     * Returns the record ID associated with the record request input string for
     * the given DAO type. First attempts to parse the request string as a
     * record ID. If that is unsuccessful, it treats the request string as a
     * record display name. This currently only supports Map and User types.
     * 
     * @param requestStr
     *            can be either a map ID or a map name
     * @param dbConn
     *            JDBC Connection
     * @return if a record ID string is passed in, it is verified and returned;
     *         if a record name string is passed in, it is verified that only
     *         one record of the requested type exists with the given name, and
     *         its ID is returned
     * @throws Exception
     *             if the requested record doesn't exist or if multiple reccords
     *             of the same type exist with the requested input name
     */
    public static long getRecordIdForInputString(final String requestStr, Connection dbConn,
            RelationalPathBase<?> table, NumberPath<Long> idField, StringPath nameField) throws Exception {
        // There must be a way to implement this generically for all DAO's but
        // haven't been able to
        // figured out how to do it yet.

        if (StringUtils.isEmpty(requestStr)) {
            throw new Exception("No record exists with ID: " + requestStr + ".  Please specify a valid record.");
        }

        boolean parsedAsNum = true;
        long idNum = -1;
        try {
            idNum = Long.parseLong(requestStr);
        }
        catch (NumberFormatException e) {
            parsedAsNum = false;
        }

        String requestStrType = parsedAsNum ? "ID" : "name";

        boolean recordExists = false;
        boolean multipleRecordsExist = false;
        log.debug("Verifying record with " + requestStrType + ": " + requestStr + " has previously been "
                + "created ...");
        if (idNum != -1) {
            recordExists = new SQLQuery(dbConn, DbUtils.getConfiguration()).from(table)
                    .where(idField.eq(new Long(idNum))).exists();
        }
        else if (!StringUtils.isEmpty(requestStr)) {
            // input wasn't parsed as a numeric ID, so let's try it as a name

            // there has to be a better way to do this against the generated
            // code but haven't been
            // able to get it to work yet

            List<Long> records = new SQLQuery(dbConn, DbUtils.getConfiguration()).from(table)
                    .where(nameField.eq(requestStr)).list(idField);

            if (records.size() == 1) {
                return records.get(0);
            }
            if (records.size() > 1) {
                recordExists = true;
                multipleRecordsExist = true;
            }
        }
        if (multipleRecordsExist) {
            throw new Exception("Multiple records exist with " + requestStrType + ": " + requestStr
                    + ".  Please specify " + "a single, valid record.");
        }
        if (!recordExists) {
            throw new Exception("No record exists with " + requestStrType + ": " + requestStr + ".  Please specify a "
                    + "valid record.");
        }

        return idNum;
    }
}
