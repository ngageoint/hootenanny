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
package hoot.services.controllers.nonblocking.conflation;

import java.io.File;
import java.time.LocalDateTime;
import java.util.Map;

import org.apache.commons.io.FileUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.command.CommandResult;
import hoot.services.command.InternalCommand;
import hoot.services.utils.DbUtils;


final class UpdateTagsCommand implements InternalCommand {
    private static final Logger logger = LoggerFactory.getLogger(UpdateTagsCommand.class);

    private final Map<String, String> tags;
    private final String mapName;
    private final String jobId;

    UpdateTagsCommand(Map<String, String> tags, String mapName, String jobId) {
        this.tags = tags;
        this.mapName = mapName;
        this.jobId = jobId;
    }

    @Override
    public CommandResult execute() {
        CommandResult commandResult = new CommandResult();
        commandResult.setJobId(jobId);
        commandResult.setCommand("updateTagsDirect()");
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
