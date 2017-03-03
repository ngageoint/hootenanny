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
package hoot.services.controllers.clipping;

import static hoot.services.HootProperties.CLIP_DATASET_MAKEFILE_PATH;
import static hoot.services.HootProperties.HOOTAPI_DB_URL;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.ParseException;

import hoot.services.command.ExternalCommand;
import hoot.services.utils.JsonUtils;


class ClipDatasetCommand extends ExternalCommand {

    ClipDatasetCommand(String params, Class<?> caller) {
        JSONArray commandArgs;

        try {
            // scripts/makeclipdataset: hoot crop-map $(HOOT_OPTS) "$(OP_INPUT)" "$(OP_OUTPUT)" "$(BBOX)"
            commandArgs = JsonUtils.parseParams(params);
        }
        catch (ParseException pe) {
            throw new RuntimeException("Error parsing: " + params, pe);
        }

        JSONObject hootDBURL = new JSONObject();
        hootDBURL.put("DB_URL", HOOTAPI_DB_URL);
        commandArgs.add(hootDBURL);

        super.configureAsMakeCommand(CLIP_DATASET_MAKEFILE_PATH, caller, commandArgs);
    }
}
