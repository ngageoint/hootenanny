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
package hoot.services.controllers.common;

import static hoot.services.HootProperties.CORE_SCRIPT_PATH;
import static hoot.services.HootProperties.EXPORT_RENDERDB_SCRIPT;
import static hoot.services.HootProperties.HOOTAPI_DB_URL;
import static hoot.services.models.db.QMaps.maps;

import java.io.File;
import java.util.HashMap;
import java.util.Map;

import hoot.services.command.CommandResult;
import hoot.services.command.ExternalCommand;
import hoot.services.models.db.Users;
import hoot.services.utils.DbUtils;


public class ExportRenderDBCommand extends ExternalCommand {
    private final String name;
    private final Class<?> caller;
    private final Users user;

    ExportRenderDBCommand(String jobId, String name, Class<?> caller, Users user) {
        super(jobId);
        this.name = name;
        this.caller = caller;
        this.user = user;
    }

    @Override
    public CommandResult execute() {
        // Note: Here we have to delay command configuration binding until actual execution time since
        // some information, specifically map info defined by the name, may not be available
        // at the time of the instantiation.
        this.performLateConfigurationBinding();

        return super.execute();
    }

    private void performLateConfigurationBinding() {
        long mapId = DbUtils.getRecordIdForInputString(name, maps, maps.id, maps.displayName);
        String script = new File(CORE_SCRIPT_PATH, EXPORT_RENDERDB_SCRIPT).getAbsolutePath();

        Map<String, Object> substitutionMap = new HashMap<>();
        substitutionMap.put("SCRIPT", script);
        substitutionMap.put("MAP_ID", Long.toString(mapId));
        substitutionMap.put("HOOTAPI_DB_URL", HOOTAPI_DB_URL);
        if(user != null) {
            substitutionMap.put("USER_EMAIL", user.getEmail());
        } else {
            substitutionMap.put("USER_EMAIL", "test@test.com");
        }

        String command = "${SCRIPT} ${MAP_ID} ${HOOTAPI_DB_URL} ${USER_EMAIL}";

        super.configureCommand(command, substitutionMap, caller);
    }
}
