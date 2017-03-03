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
 * @copyright Copyright (C) 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.conflation;

import static hoot.services.HootProperties.*;

import java.io.File;
import java.time.LocalDateTime;
import java.util.HashMap;
import java.util.Map;

import org.apache.commons.io.FileUtils;
import org.joda.time.DateTime;
import org.joda.time.DateTimeZone;
import org.joda.time.format.DateTimeFormat;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.command.CommandResult;
import hoot.services.command.InternalCommand;
import hoot.services.utils.DbUtils;
import hoot.services.utils.JsonUtils;


class UpdateTagsCommand implements InternalCommand {
    private static final Logger logger = LoggerFactory.getLogger(UpdateTagsCommand.class);

    private final Map<String, String> tags;
    private final String mapName;
    private final String jobId;

    UpdateTagsCommand(String params, String mapName, String jobId) {
        this.mapName = mapName;
        this.jobId = jobId;
        this.tags = new HashMap<>();

        JSONParser parser = new JSONParser();
        JSONObject oParams;
        try {
            oParams = (JSONObject) parser.parse(params);
        }
        catch (ParseException pe) {
            throw new RuntimeException("Error parsing: " + params, pe);
        }

        String confOutputName = oParams.get("OUTPUT_NAME").toString();
        String input1Name = oParams.get("INPUT1").toString();
        String input2Name = oParams.get("INPUT2").toString();

        // add map tags
        // WILL BE DEPRECATED WHEN CORE IMPLEMENTS THIS
        tags.put("input1", input1Name);
        tags.put("input2", input2Name);

        // Need to reformat the list of hoot command options to json properties
        try {
            tags.put("params", JsonUtils.escapeJson(params));
        }
        catch (ParseException e) {
            throw new RuntimeException("Error escaping JSON: " + params);
        }

        // Hack alert!
        // Write stats file name to tags, if the file exists when this updateMapsTagsCommand job is run, the
        // file will be read and its contents placed in the stats tag.
        if ((oParams.get("COLLECT_STATS") != null)
                && oParams.get("COLLECT_STATS").toString().equalsIgnoreCase("true")) {
            String statsName = RPT_STORE_PATH + File.separator + confOutputName + "-stats.csv";
            tags.put("stats", statsName);
        }

        // osm api db related input params have already been validated by
        // this point, so just check to see if any osm api db input is present
        if (ConflationResource.oneLayerIsOsmApiDb(oParams) && OSM_API_DB_ENABLED) {
            // write a timestamp representing the time the osm api db data was queried out
            // from the source; to be used conflict detection during export of conflated
            // data back into the osm api db at a later time; timestamp must be 24 hour utc
            // to match rails port
            String now = DateTimeFormat
                    .forPattern(DbUtils.OSM_API_TIMESTAMP_FORMAT)
                    .withZone(DateTimeZone.UTC)
                    .print(new DateTime());

            tags.put("osm_api_db_export_time", now);
        }
    }

    @Override
    public CommandResult execute() {
        CommandResult commandResult = new CommandResult();
        commandResult.setJobId(jobId);
        commandResult.setCommand("[Update Map Tags] of map with name = " + mapName);
        commandResult.setStart(LocalDateTime.now());

        updateTagsDirect();

        commandResult.setFinish(LocalDateTime.now());
        commandResult.setExitCode(CommandResult.SUCCESS);

        return commandResult;
    }

    private void updateTagsDirect() {
        try {
            // Currently we do not have any way to get map id directly from hoot
            // core command when it runs so for now we need get the all the map ids matching name and pick
            // first one..
            // THIS WILL NEED TO CHANGE when we implement handle map by Id instead of name..

            Long mapId = DbUtils.getMapIdByName(mapName);
            if (mapId != null) {
                // Hack alert!
                // Add special handling of stats tag key
                // We need to read the file in here, because the file doesn't
                // exist at the time the updateMapsTagsCommand job is created in ConflationResource.java
                String statsKey = "stats";
                if (tags.containsKey(statsKey)) {
                    String statsName = tags.get(statsKey);
                    File statsFile = new File(statsName);
                    if (statsFile.exists()) {
                        String stats = FileUtils.readFileToString(statsFile, "UTF-8");
                        tags.put(statsKey, stats);

                        if (!statsFile.delete()) {
                            logger.error("Error deleting {} file", statsFile.getAbsolutePath());
                        }
                    }
                    else {
                        logger.error("Can't find {}", statsName);
                        tags.remove(statsKey);
                    }
                }

                DbUtils.updateMapsTableTags(tags, mapId);
            }
        }
        catch (Exception ex) {
            String msg = "Failure update map tags resource" + ex.getMessage();
            throw new RuntimeException(msg, ex);
        }
    }
}
