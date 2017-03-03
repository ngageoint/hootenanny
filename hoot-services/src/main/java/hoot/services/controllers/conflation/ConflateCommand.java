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

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.ParseException;

import hoot.services.command.ExternalCommand;
import hoot.services.geo.BoundingBox;
import hoot.services.utils.JsonUtils;


class ConflateCommand extends ExternalCommand {

    ConflateCommand(String params, BoundingBox bounds, Class<?> caller) {
        JSONArray commandArgs;
        try {
            commandArgs = JsonUtils.parseParams(params);
        }
        catch (ParseException pe) {
            throw new RuntimeException("Error parsing: " + params, pe);
        }

        if (bounds != null) {
            JSONObject conflateAOI = new JSONObject();
            conflateAOI.put("conflateaoi", bounds.getMinLon() + "," + bounds.getMinLat() + "," + bounds.getMaxLon() + "," + bounds.getMaxLat());
            commandArgs.add(conflateAOI);
        }

        JSONObject hootDBURL = new JSONObject();
        hootDBURL.put("DB_URL", HOOTAPI_DB_URL);
        commandArgs.add(hootDBURL);

        JSONObject osmAPIDBURL = new JSONObject();
        osmAPIDBURL.put("OSM_API_DB_URL", OSMAPI_DB_URL);
        commandArgs.add(osmAPIDBURL);

        super.configureAsMakeCommand(CONFLATE_MAKEFILE_PATH, caller, commandArgs);
    }
}
