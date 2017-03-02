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
package hoot.services.controllers;

import static hoot.services.HootProperties.EXPORT_RENDERDB_SCRIPT;
import static hoot.services.HootProperties.HOOTAPI_DB_URL;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.command.ExternalCommand;
import hoot.services.models.db.QMaps;
import hoot.services.utils.DbUtils;


class ExportRenderDBCommand extends ExternalCommand {
    private static final Logger logger = LoggerFactory.getLogger(ExportRenderDBCommand.class);

    ExportRenderDBCommand(String name, Class<?> caller) {
        long mapId = DbUtils.getRecordIdForInputString(name, QMaps.maps, QMaps.maps.id, QMaps.maps.displayName);

        JSONArray commandArgs = new JSONArray();

        JSONObject argument = new JSONObject();
        argument.put("MAP_ID", String.valueOf(mapId));
        commandArgs.add(argument);

        argument = new JSONObject();
        argument.put("DB_URL", HOOTAPI_DB_URL);
        commandArgs.add(argument);

        super.configureAsBashCommand(EXPORT_RENDERDB_SCRIPT, caller, commandArgs);
    }
}
