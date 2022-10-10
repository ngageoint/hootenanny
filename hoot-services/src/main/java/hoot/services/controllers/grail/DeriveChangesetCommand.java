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
package hoot.services.controllers.grail;

import java.io.File;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;


/**
 * Used to construct a changeset-derive command
 */
class DeriveChangesetCommand extends GrailCommand {

    DeriveChangesetCommand(String jobId, GrailParams params, String debugLevel, Class<?> caller) {
        super(jobId, params);

        String confFile = "DeriveChangeset.conf";

        List<String> options = new LinkedList<>();
        options.add("api.db.email=" + params.getUser().getEmail());

        if(params.getConflationType() != null && params.getConflationType().contains("Differential")) {
            options.add("changeset.allow.deleting.reference.features=false");
        }

        if (params.getBounds() != null) {
            //Add bounds
            options.add("bounds=" + params.getBounds());
        }

        if(params.getOutput().endsWith(".osm")) {
            confFile = "DeriveJosmOsm.conf";
        }

        List<String> hootOptions = toHootOptions(options);

        Map<String, Object> substitutionMap = new HashMap<>();
        substitutionMap.put("CONF_FILE", confFile);
        substitutionMap.put("INPUT1", params.getInput1());
        substitutionMap.put("INPUT2", params.getInput2());
        substitutionMap.put("OSC_FILE", params.getOutput());
        substitutionMap.put("HOOT_OPTIONS", hootOptions);
        substitutionMap.put("DEBUG_LEVEL", debugLevel);
        substitutionMap.put("STATS_FILE", new File(params.getWorkDir(), "stats.json").getPath());

        String command = "hoot.bin changeset-derive --${DEBUG_LEVEL} -C ${CONF_FILE} ${HOOT_OPTIONS} ${INPUT1} ${INPUT2} ${OSC_FILE} --stats ${STATS_FILE}";

        super.configureCommand(command, substitutionMap, caller);
    }
}
