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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.grail;

import static hoot.services.HootProperties.HOOTAPI_DB_URL;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;


/**
 * Used for pushing OSM data to the database
 */
class PushToDbCommand extends GrailCommand {
    private static final Logger logger = LoggerFactory.getLogger(PushToDbCommand.class);

    PushToDbCommand(String jobId, GrailParams params, String debugLevel, Class<?> caller) {
        super(jobId, params);

        logger.info("Params: " + params);

        List<String> options = new LinkedList<>();
        options.add("hootapi.db.writer.overwrite.map=true");
        options.add("hootapi.db.writer.remap.ids=false");
        options.add("job.id=" + jobId);
        options.add("api.db.email=" + params.getUser().getEmail());

        List<String> hootOptions = toHootOptions(options);

        String dbName = HOOTAPI_DB_URL + "/" + params.getOutput();

        Map<String, Object> substitutionMap = new HashMap<>();
        substitutionMap.put("DEBUG_LEVEL", debugLevel);
        substitutionMap.put("HOOT_OPTIONS", hootOptions);
        substitutionMap.put("DB_NAME", dbName);
        substitutionMap.put("INPUT", params.getInput1());

        String command = "hoot convert --${DEBUG_LEVEL} ${HOOT_OPTIONS} ${INPUT} ${DB_NAME}";

        super.configureCommand(command, substitutionMap, caller);
    }

}
