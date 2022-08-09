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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
package hoot.services.controllers.clipping;

import static hoot.services.HootProperties.HOOTAPI_DB_URL;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import hoot.services.command.ExternalCommand;
import hoot.services.models.db.Users;


class ClipDatasetCommand extends ExternalCommand {
    ClipDatasetCommand(String jobId, ClipDatasetParams params, String debugLevel, Class<?> caller, Users user) {
        super(jobId);

        List<String> options = new LinkedList<>();
        options.add("hootapi.db.writer.overwrite.map=true");
        options.add("job.id=" + jobId);
        if(user == null) {
            options.add("api.db.email=test@test.com");
        } else {
            options.add("api.db.email=" + user.getEmail());
        }

        List<String> hootOptions = toHootOptions(options);

        //The input OSM data path
        Map<String, Object> substitutionMap = new HashMap<>();
        substitutionMap.put("DEBUG_LEVEL", debugLevel);
        substitutionMap.put("HOOT_OPTIONS", hootOptions);

        //The input OSM data path
        substitutionMap.put("INPUT", HOOTAPI_DB_URL + "/" + params.getInputName());

        //The output - The output OSM data path.
        substitutionMap.put("OUTPUT", HOOTAPI_DB_URL + "/" + params.getOutputName());

        //Comma delimited bounds. minx,miny,maxx,maxy e.g.38,-105,39,-104
        substitutionMap.put("BOUNDS", params.getBounds());

        String command = "hoot.bin crop --${DEBUG_LEVEL} ${HOOT_OPTIONS} ${INPUT} ${OUTPUT} ${BOUNDS}";

        super.configureCommand(command, substitutionMap, caller);
    }
}
