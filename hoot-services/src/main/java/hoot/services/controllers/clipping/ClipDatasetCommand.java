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

import static hoot.services.HootProperties.HOOTAPI_DB_URL;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import hoot.services.command.ExternalCommand;


/*
#
#  Clip Dataset Make file
#
-include $(HOOT_HOME)/HOOT_VERSION_FILE

#HOOT_OPTS+= -D osm2ogr.ops=hoot::DecomposeBuildingRelationsVisitor -D add.review.tags=yes
HOOT_OPTS+= -D hootapi.db.writer.overwrite.map=true -D hootapi.db.writer.create.user=true
HOOT_OPTS+= -D api.db.email=test@test.com

#DB_URL=hootapidb://hoot:hoottest@localhost:5432/hoot
#OUTPUT_DIR=$(HOOT_HOME)/test-out/$(jobid)

# Clip
# crop-map (input) (output) (bounds)
OP_INPUT=$(DB_URL)/$(INPUT_NAME)
OP_OUTPUT=$(DB_URL)/$(OUTPUT_NAME)

step1:
    hoot crop-map $(HOOT_OPTS) "$(OP_INPUT)" "$(OP_OUTPUT)" "$(BBOX)"
 */

class ClipDatasetCommand extends ExternalCommand {

    ClipDatasetCommand(ClipDatasetParams params, String debugLevel, Class<?> caller) {
        List<String> options = new LinkedList<>();
        options.add("hootapi.db.writer.overwrite.map=true");
        options.add("hootapi.db.writer.create.user=true");
        options.add("api.db.email=test@test.com");
        String hootOptions = hootOptionsToString(options);

        //The input OSM data path
        Map<String, String> substitutionMap = new HashMap<>();
        substitutionMap.put("DEBUG_LEVEL", debugLevel);
        substitutionMap.put("HOOT_OPTIONS", hootOptions);

        //The input OSM data path
        substitutionMap.put("INPUT", HOOTAPI_DB_URL + "/" + params.getInputName());

        //The output - The output OSM data path.
        substitutionMap.put("OUTPUT", HOOTAPI_DB_URL + "/" + params.getOutputName());

        //Comma delimited bounds. minx,miny,maxx,maxy e.g.38,-105,39,-104
        substitutionMap.put("BOUNDS", params.getBounds());

        // '' around ${} signifies that quoting is needed
        String command = "hoot crop-map --${DEBUG_LEVEL} ${HOOT_OPTIONS} '${INPUT}' '${OUTPUT}' '${BOUNDS}'";

        super.configureCommand(command, substitutionMap, caller);
    }
}
