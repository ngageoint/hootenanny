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
 * @copyright Copyright (C) 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.conflation;

import static hoot.services.HootProperties.*;

import java.io.File;
import java.io.IOException;
import java.nio.charset.Charset;
import java.util.Arrays;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import org.apache.commons.io.FileUtils;

import hoot.services.command.CommandResult;
import hoot.services.command.ExternalCommand;
import hoot.services.geo.BoundingBox;


class ConflateCommand extends ExternalCommand {

    private final ConflateParams conflateParams;

    ConflateCommand(String jobId, ConflateParams params, String debugLevel, Class<?> caller) {
        super(jobId);
        this.conflateParams = params;

        boolean conflatingOsmApiDbData = ConflateUtils.isAtLeastOneLayerOsmApiDb(params);

        //Since we're not returning the osm api db layer to the hoot ui, this exception
        //shouldn't actually ever occur, but will leave this check here anyway.
        if (conflatingOsmApiDbData && !OSM_API_DB_ENABLED) {
            throw new IllegalArgumentException("Attempted to conflate an OSM API database data source but OSM " +
                    "API database support is disabled.");
        }

        BoundingBox bounds;

        // osm api db related input params have already been validated by
        // this point, so just check to see if any osm api db input is present
        if (conflatingOsmApiDbData && OSM_API_DB_ENABLED) {
            ConflateUtils.validateOsmApiDbConflateParams(params);

            String secondaryParameterKey = ConflateUtils.isFirstLayerOsmApiDb(params) ? params.getInput2() : params.getInput1();

            //Record the aoi of the conflation job (equal to that of the secondary layer), as
            //we'll need it to detect conflicts at export time.
            long secondaryMapId = Long.parseLong(secondaryParameterKey);
            if (!hoot.services.models.osm.Map.mapExists(secondaryMapId)) {
                String msg = "No secondary map exists with ID: " + secondaryMapId;
                throw new IllegalArgumentException(msg);
            }

            if (params.getBounds() != null) {
                bounds = new BoundingBox(params.getBounds());
            }
            else {
                hoot.services.models.osm.Map secondaryMap = new hoot.services.models.osm.Map(secondaryMapId);
                bounds = secondaryMap.getBounds();
            }
        }
        else {
            bounds = null;
        }

        String aoi = null;

        if (bounds != null) {
            aoi = bounds.getMinLon() + "," + bounds.getMinLat() + "," + bounds.getMaxLon() + "," + bounds.getMaxLat();
        }

        List<String> options = new LinkedList<>();
        options.add("convert.ops=hoot::DecomposeBuildingRelationsVisitor");
        options.add("writer.include.conflate.score.tags=false");
        options.add("hootapi.db.writer.overwrite.map=true");
        options.add("hootapi.db.writer.create.user=true");
        //options.add("writer.include.debug.tags=true");
        options.add("writer.text.status=true");
        options.add("api.db.email=test@test.com");

        String input1Type = params.getInputType1();
        String input1 = input1Type.equalsIgnoreCase("DB") ? (HOOTAPI_DB_URL + "/" + params.getInput1()) : params.getInput1();

        String input2Type = params.getInputType2();
        String input2 = input2Type.equalsIgnoreCase("DB") ? (HOOTAPI_DB_URL + "/" + params.getInput2()) : params.getInput2();

        String referenceLayer = params.getReferenceLayer();
        if (referenceLayer.equalsIgnoreCase("1")) {
            if (input1Type.equalsIgnoreCase("OSM_API_DB")) {
                input1 = OSMAPI_DB_URL; 
            }
        }
        else if (referenceLayer.equalsIgnoreCase("2")) {
            options.add("tag.merger.default=hoot::OverwriteTag1Merger");
            if (input2Type.equalsIgnoreCase("OSM_API_DB")) {
                input2 = OSMAPI_DB_URL;
            }
        }
        //This is set up for the XML changeset workflow.
        if (input1Type.equalsIgnoreCase("OSM_API_DB") || input2Type.equalsIgnoreCase("OSM_API_DB"))
        {
          options.add("convert.bounding.box=" + aoi);
          options.add("reader.conflate.use.data.source.ids.1=true");
          options.add("reader.conflate.use.data.source.ids.2=false");
          options.add("id.generator=hoot::PositiveIdGenerator");
          options.add("osm.map.writer.factory.writer=hoot::NonIdRemappingHootApiDbWriter");
          options.add("preserve.unknown1.element.id.when.modifying.features=true");
        }

        String output = HOOTAPI_DB_URL + "/" + params.getOutputName();

        if (params.getAdvancedOptions() != null) {
            String[] advOptions = params.getAdvancedOptions().trim().split("-D ");
            Arrays.stream(advOptions).forEach((option) -> {
                if (!option.isEmpty()) {
                    options.add(option.trim());
                };
            });
        }

        List<String> hootOptions = toHootOptions(options);

        String stats = "";
        if (params.getCollectStats()) {
            // Don't include non-error log messages in stdout because we are redirecting to file
            debugLevel = "error";

            //Hootenanny map statistics such as node and way count
            stats = "--stats";
        }

        String conflationCommand = params.getConflationCommand();
        boolean isDifferentialConflate = false;
        if (conflationCommand != null && conflationCommand.equals("conflate-differential"))
        {
          conflationCommand = "conflate";
          isDifferentialConflate = true;
        }

        Map<String, Object> substitutionMap = new HashMap<>();
        substitutionMap.put("CONFLATION_COMMAND", conflationCommand);
        substitutionMap.put("DEBUG_LEVEL", debugLevel);
        substitutionMap.put("HOOT_OPTIONS", hootOptions);
        substitutionMap.put("INPUT1", input1);
        substitutionMap.put("INPUT2", input2);
        substitutionMap.put("OUTPUT", output);
        substitutionMap.put("STATS", stats);

        String command = "hoot ${CONFLATION_COMMAND} --${DEBUG_LEVEL} -C RemoveReview2Pre.conf ${HOOT_OPTIONS} ${INPUT1} ${INPUT2} ${OUTPUT} ${STATS}";
        if (isDifferentialConflate)
        {
          command += " --differential";
        }

        super.configureCommand(command, substitutionMap, caller);
    }

    @Override
    public CommandResult execute() {
        CommandResult commandResult = super.execute();

        if (conflateParams.getCollectStats()) {
            File statsFile = new File(RPT_STORE_PATH, conflateParams.getOutputName() + "-stats.csv");
            try {
                FileUtils.write(statsFile, commandResult.getStdout(), Charset.defaultCharset());
            }
            catch (IOException ioe) {
                throw new RuntimeException("Error writing to " + statsFile.getAbsolutePath(), ioe);
            }
        }

        return commandResult;
    }
}
