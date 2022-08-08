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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */
package hoot.services.controllers.osm.map;

import java.time.LocalDateTime;
import java.util.Map;

import hoot.services.command.CommandResult;
import hoot.services.command.InternalCommand;
import hoot.services.utils.DbUtils;

public class SetMapTagsCommand implements InternalCommand {

        private final String jobId;
        private final Map<String, String> tags;

        SetMapTagsCommand(Map<String, String> tags, String jobId) {
            this.jobId = jobId;
            this.tags = tags;
        }

        @Override
        public CommandResult execute() {
            Long mapId = DbUtils.getMapIdByJobId(jobId);

            CommandResult commandResult = new CommandResult();
            commandResult.setJobId(jobId);
            commandResult.setCommand("[Set Map Tags] of map with id = " + mapId);
            commandResult.setStart(LocalDateTime.now());

            if (mapId != null) {
                DbUtils.updateMapsTableTags(tags, mapId);
            }

            commandResult.setFinish(LocalDateTime.now());
            commandResult.setExitCode(CommandResult.SUCCESS);

            return commandResult;
        }

}
