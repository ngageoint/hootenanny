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

import static hoot.services.HootProperties.HOOTAPI_DB_URL;
import static hoot.services.HootProperties.OSMAPI_DB_URL;

import java.util.Arrays;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import hoot.services.command.ExternalCommand;
import hoot.services.geo.BoundingBox;


class ConflateCommand extends ExternalCommand {

    ConflateCommand(ConflateParams params, BoundingBox bounds, String debugLevel, Class<?> caller) {
        String aoi = null;

        if (bounds != null) {
            aoi = bounds.getMinLon() + "," + bounds.getMinLat() + "," + bounds.getMaxLon() + "," + bounds.getMaxLat();
        }

        //HOOT_OPTS+= -D osm2ogr.ops=hoot::DecomposeBuildingRelationsVisitor -D conflate.add.score.tags=yes
        //HOOT_OPTS+= -D hootapi.db.writer.overwrite.map=true -D hootapi.db.writer.create.user=true
        //HOOT_OPTS+= -D api.db.email=test@test.com

        List<String> options = new LinkedList<>();
        options.add("osm2ogr.ops=hoot::DecomposeBuildingRelationsVisitor");
        options.add("conflate.add.score.tags=yes");
        options.add("hootapi.db.writer.overwrite.map=true");
        options.add("hootapi.db.writer.create.user=true");
        options.add("api.db.email=test@test.com");

        //HOOT_OPTS+= $(ADV_OPTIONS)
        if (params.getAdvancedOptions() != null) {
            String[] advOptions = params.getAdvancedOptions().trim().split("-D ");
            Arrays.stream(advOptions).forEach((option) -> {
                if (!option.isEmpty()) {
                    options.add(option.trim());
                };
            });
        }

        /*
            ifeq "$(INPUT1_TYPE)" "DB"
                OP_INPUT1=$(DB_URL)/$(INPUT1)
            endif

            ifeq "$(INPUT2_TYPE)" "DB"
                OP_INPUT2=$(DB_URL)/$(INPUT2)
            endif
         */
        String input1;
        String input1Type = params.getInputType1();
        if (input1Type.equalsIgnoreCase("DB")) {
            input1 = HOOTAPI_DB_URL + "/" + params.getInput1();
        }
        else {
            input1 = params.getInput1();
        }

        String input2;
        String input2Type = params.getInputType2();
        if (input2Type.equalsIgnoreCase("DB")) {
            input2 = HOOTAPI_DB_URL + "/" + params.getInput2();
        }
        else {
            input2 = params.getInput2();
        }

        /*
           # This is also depending on some extra input validation present in ConflationResource.
          ifeq "$(REFERENCE_LAYER)" "1"
              ifeq "$(INPUT1_TYPE)" "OSM_API_DB"
                  OP_INPUT1=$(OSM_API_DB_URL)
                  HOOT_OPTS+= -D convert.bounding.box=$(conflateaoi) -D conflate.use.data.source.ids=true -D osm.map.reader.factory.reader=hoot::OsmApiDbAwareHootApiDbReader -D osm.map.writer.factory.writer=hoot::OsmApiDbAwareHootApiDbWriter -D osmapidb.id.aware.url="$(OSM_API_DB_URL)"
              endif
          endif
         */
        String referenceLayer = params.getReferenceLayer();
        if (referenceLayer.equalsIgnoreCase("1")) {
            if (input1Type.equalsIgnoreCase("OSM_API_DB")) {
                input1 = OSMAPI_DB_URL;
                options.add("convert.bounding.box=" + aoi);
                options.add("conflate.use.data.source.ids=true");
                options.add("osm.map.reader.factory.reader=hoot::OsmApiDbAwareHootApiDbReader");
                options.add("osm.map.writer.factory.writer=hoot::OsmApiDbAwareHootApiDbWriter");
                options.add("osmapidb.id.aware.url=" + OSMAPI_DB_URL);
            }
        }

        /*
          ifeq "$(REFERENCE_LAYER)" "2"
              HOOT_OPTS+= -D tag.merger.default=hoot::OverwriteTag1Merger
          endif

          ifeq "$(REFERENCE_LAYER)" "2"
              ifeq "$(INPUT2_TYPE)" "OSM_API_DB"
                  OP_INPUT2=$(OSM_API_DB_URL)
                  HOOT_OPTS+= -D convert.bounding.box=$(conflateaoi) -D conflate.use.data.source.ids=true -D osm.map.reader.factory.reader=hoot::OsmApiDbAwareHootApiDbReader -D osm.map.writer.factory.writer=hoot::OsmApiDbAwareHootApiDbWriter -D osmapidb.id.aware.url="$(OSM_API_DB_URL)"
              endif
          endif
         */
        else if (referenceLayer.equalsIgnoreCase("2")) {
            options.add("tag.merger.default=hoot::OverwriteTag1Merger");
            if (input2Type.equalsIgnoreCase("OSM_API_DB")) {
                input2 = OSMAPI_DB_URL;
                options.add("convert.bounding.box=" + aoi);
                options.add("conflate.use.data.source.ids=true");
                options.add("osm.map.reader.factory.reader=hoot::OsmApiDbAwareHootApiDbReader");
                options.add("osm.map.writer.factory.writer=hoot::OsmApiDbAwareHootApiDbWriter");
                options.add("osmapidb.id.aware.url=" + OSMAPI_DB_URL);
            }
        }

        /*
          ifeq "$(COLLECT_STATS)" "true"
              OP_STAT= --stats > $(HOOT_HOME)/userfiles/reports/$(OUTPUT_NAME)-stats.csv
              # don't include non-error log messages in stdout because we are redirecting to file
              HOOT_OPTS+= --error
          endif
         */
        String outputName = params.getOutputName();
        String output = HOOTAPI_DB_URL + "/" + outputName;

        //Hootenanny map statistics such as node and way count
        String stats = "";
        if (params.getCollectStats()) {
            // Don't include non-error log messages in stdout because we are redirecting to file
            debugLevel = "error";
            stats = "--stats";
        }

        List<String> hootOptions = toHootOptions(options);

        Map<String, Object> substitutionMap = new HashMap<>();
        substitutionMap.put("DEBUG_LEVEL", debugLevel);
        substitutionMap.put("HOOT_OPTIONS", hootOptions);
        substitutionMap.put("INPUT1", input1);
        substitutionMap.put("INPUT2", input2);
        substitutionMap.put("OUTPUT", output);
        substitutionMap.put("STATS", stats);

        String command = "hoot conflate --${DEBUG_LEVEL} -C RemoveReview2Pre.conf ${HOOT_OPTIONS} ${INPUT1} ${INPUT2} ${OUTPUT} ${STATS}";

        super.configureCommand(command, substitutionMap, caller);
    }
}
