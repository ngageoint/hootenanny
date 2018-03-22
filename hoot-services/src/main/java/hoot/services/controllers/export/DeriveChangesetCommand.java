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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.export;

import static hoot.services.HootProperties.OSMAPI_DB_URL;
import static hoot.services.HootProperties.CHANGESET_DERIVE_BUFFER;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;


class DeriveChangesetCommand extends ExportCommand {
    private static final Logger logger = LoggerFactory.getLogger(DeriveChangesetCommand.class);

    DeriveChangesetCommand(String jobId, ExportParams params, String debugLevel, Class<?> caller) {
        super(jobId, params);

        Long mapId = Long.parseLong(params.getInput());
        hoot.services.models.osm.Map conflatedMap = getConflatedMap(mapId);

        String aoi = getAOI(params, conflatedMap);

        //This is set up for the XML changeset workflow.
        List<String> options = super.getCommonExportHootOptions();
        options.add("convert.bounding.box=" + aoi);
        options.add("api.db.email=test@test.com");
        options.add("reader.use.file.status=true");
        options.add("reader.keep.status.tag=true");
        double changesetBufferSize = Double.parseDouble(CHANGESET_DERIVE_BUFFER); //in degrees
        options.add("changeset.buffer=" + String.valueOf(changesetBufferSize));
        options.add("changeset.allow.deleting.reference.features=false");

        String userId = params.getUserId();
        if (userId != null) {
            options.add("changeset.user.id=" + userId);
        }
        else {
            throw new RuntimeException("changeset.user.id cannot be null.  Please provide a valid user ID!");
        }

        List<String> hootOptions = toHootOptions(options);

        Map<String, Object> substitutionMap = new HashMap<>();
        substitutionMap.put("DEBUG_LEVEL", debugLevel);
        substitutionMap.put("HOOT_OPTIONS", hootOptions);
        substitutionMap.put("OSMAPI_DB_URL", OSMAPI_DB_URL);
        substitutionMap.put("INPUT", super.getInput());

        String command;

        if (params.getOutputType().equalsIgnoreCase("osc")) {
            // Just derive without apply (Will return .osc file to the REST caller)
            substitutionMap.put("CHANGESET_OUTPUT_PATH", super.getOutputPath());
            command = "hoot changeset-derive --${DEBUG_LEVEL} ${HOOT_OPTIONS} ${OSMAPI_DB_URL} ${INPUT} ${CHANGESET_OUTPUT_PATH}";
        }
        else {
            // Derive changeset here.  The actual apply command is issued via ApplyChangesetCommand from another class.
            substitutionMap.put("CHANGESET_OUTPUT_PATH", super.getSQLChangesetPath()); //"changeset-" + getJobId() + ".osc.sql"
            command = "hoot changeset-derive --${DEBUG_LEVEL} ${HOOT_OPTIONS} ${OSMAPI_DB_URL} ${INPUT} ${CHANGESET_OUTPUT_PATH} ${OSMAPI_DB_URL}";
        }

        super.configureCommand(command, substitutionMap, caller);
    }
}
