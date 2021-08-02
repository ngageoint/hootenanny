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
import java.util.Map.Entry;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;


/**
 * Used to get the diff between the 2 inputs
 */
class RunDiffCommand extends GrailCommand {
    private static final Logger logger = LoggerFactory.getLogger(RunDiffCommand.class);

    RunDiffCommand(String jobId, GrailParams params, String debugLevel, Class<?> caller) {
        super(jobId,params);

        logger.debug("Params: " + params);

        List<String> options = new LinkedList<>();

        Map<String, String> hoot2AdvOptions = params.getAdvancedOptions();
        String algorithm = "";

        if (hoot2AdvOptions != null && !hoot2AdvOptions.isEmpty()) {
            for (Entry<String, String> option: hoot2AdvOptions.entrySet()) {
                if (configOptions.containsKey(option.getKey())) { // if option key in possible values, add new option command
                    Map<String, String> optionConfig = configOptions.get(option.getKey());
                    String optionValue = option.getValue();

                    if (optionConfig.get("type").equalsIgnoreCase("list")) {
                        optionValue = optionValue.replaceAll("\\[|\\]", "").replaceAll(",", ";");
                    }

                    options.add("\"" + optionConfig.get("key") + "=" + optionValue + "\"");
                }
            }

            if (hoot2AdvOptions.get("RoadEngines") != null) {
                algorithm = hoot2AdvOptions.get("RoadEngines") + "Algorithm.conf";
            }
        }

        if (params.getBounds() != null) {
            //Add bounds
            options.add("bounds=" + params.getBounds());
        }

        Map<String, Object> substitutionMap = new HashMap<>();
        substitutionMap.put("HOOT_OPTIONS", toHootOptions(options));
        substitutionMap.put("INPUT1", params.getInput1());
        substitutionMap.put("INPUT2", params.getInput2());
        substitutionMap.put("OUTPUT", params.getOutput());
        substitutionMap.put("DEBUG_LEVEL", debugLevel);
        substitutionMap.put("STATS_FILE", new File(params.getWorkDir(), "stats.json").getPath());
        substitutionMap.put("ROAD_ALGORITHM", algorithm);

        String command = "hoot.bin conflate --${DEBUG_LEVEL} -C DifferentialConflation.conf"
                + (!algorithm.equals("") ? " -C ${ROAD_ALGORITHM}" : "")
                + " ${HOOT_OPTIONS} ${INPUT1} ${INPUT2} ${OUTPUT} --differential --changeset-stats ${STATS_FILE} --include-tags --separate-output";

        super.configureCommand(command, substitutionMap, caller);
    }
}
