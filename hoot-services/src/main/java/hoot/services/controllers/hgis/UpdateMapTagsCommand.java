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
package hoot.services.controllers.hgis;

import static hoot.services.models.db.QMaps.maps;

import java.time.LocalDateTime;
import java.util.HashMap;
import java.util.Map;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.command.CommandResult;
import hoot.services.command.InternalCommand;
import hoot.services.utils.DbUtils;


final class UpdateMapTagsCommand implements InternalCommand {
    private static final Logger logger = LoggerFactory.getLogger(UpdateMapTagsCommand.class);

    private final String jobId;
    private final String mapName;

    UpdateMapTagsCommand(String jobId, String mapName) {
        this.jobId = jobId;
        this.mapName = mapName;
    }

    @Override
    public CommandResult execute() {
        CommandResult commandResult = new CommandResult();
        commandResult.setCommand("[Update Map Tags] of map with name = " + mapName);
        commandResult.setJobId(this.jobId);
        commandResult.setStart(LocalDateTime.now());

        this.updateMapTags();

        commandResult.setExitCode(CommandResult.SUCCESS);
        commandResult.setFinish(LocalDateTime.now());

        return commandResult;
    }

    private void updateMapTags() {
        long mapId = DbUtils.getRecordIdForInputString(this.mapName, maps, maps.id, maps.displayName);
        Map<String, String> tags = new HashMap<>();
        tags.put("reviewtype", "hgisvalidation");

        long count = DbUtils.updateMapsTableTags(tags, mapId);

        if (count < 1) {
            throw new RuntimeException("Error updating map " + mapId + "'s tags!");
        }
    }
}
