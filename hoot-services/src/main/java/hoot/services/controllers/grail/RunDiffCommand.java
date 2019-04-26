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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.grail;

import java.util.HashMap;
import java.util.Map;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;


/**
 * Used to get the diff between the 2 inputs
 */
class RunDiffCommand extends GrailCommand {
    private static final Logger logger = LoggerFactory.getLogger(RunDiffCommand.class);

    RunDiffCommand(String jobId, GrailParams params, String debugLevel, Class<?> caller) {
        super(jobId,params);

        logger.info("Params: " + params);

        Map<String, Object> substitutionMap = new HashMap<>();
        substitutionMap.put("INPUT1", params.getInput1());
        substitutionMap.put("INPUT2", params.getInput2());
        substitutionMap.put("OUTPUT", params.getOutput());
        substitutionMap.put("DEBUG_LEVEL", debugLevel);

        String command = "hoot conflate --${DEBUG_LEVEL} -C DifferentialConflation.conf -C NetworkAlgorithm.conf ${INPUT1} ${INPUT2} ${OUTPUT} --differential --include-tags --separate-output --stats";

        super.configureCommand(command, substitutionMap, caller);
    }
}
