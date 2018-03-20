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
package hoot.services.controllers.clipping;

import static hoot.services.HootProperties.HOOTAPI_DB_URL;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.command.ExternalCommand;


class ClipDatasetCommand extends ExternalCommand {
    private static final Logger logger = LoggerFactory.getLogger(ClipDatasetCommand.class);

    ClipDatasetCommand(String jobId, ClipDatasetParams params, String debugLevel, Class<?> caller) {
        super(jobId);

        List<String> options = new LinkedList<>();
        //options.add("osm2ogr.ops=hoot::DecomposeBuildingRelationsVisitor");
        options.add("hootapi.db.writer.overwrite.map=true");
        options.add("hootapi.db.writer.create.user=true");
        options.add("api.db.email=test@test.com");

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

        String command = "hoot crop --${DEBUG_LEVEL} ${HOOT_OPTIONS} ${INPUT} ${OUTPUT} ${BOUNDS}";

        super.configureCommand(command, substitutionMap, caller);
    }
}
