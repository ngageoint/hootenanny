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
package hoot.services.controllers.conflation;

import static hoot.services.HootProperties.HOOTAPI_DB_URL;
import static hoot.services.HootProperties.RPT_STORE_PATH;

import java.io.File;
import java.io.IOException;
import java.nio.charset.Charset;
import java.util.Arrays;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import org.apache.commons.io.FileUtils;

import hoot.services.command.CommandResult;
import hoot.services.command.ExternalCommand;
import hoot.services.geo.BoundingBox;
import hoot.services.models.db.Users;


class ConflateCommand extends ExternalCommand {

    private final ConflateParams conflateParams;

    ConflateCommand(String jobId, ConflateParams params, String debugLevel, Class<?> caller, Users user) {
        super(jobId);
        this.conflateParams = params;

        List<String> options = new LinkedList<>();
        options.add("convert.ops=hoot::DecomposeBuildingRelationsVisitor");
        options.add("writer.include.conflate.score.tags=false");
        options.add("hootapi.db.writer.overwrite.map=true");
        options.add("writer.text.status=true");
        options.add("hootapi.db.writer.job.id=" + jobId);
        if(user == null) {
            options.add("api.db.email=test@test.com");
        } else {
            options.add("api.db.email=" + user.getEmail());
        }

        String input1Type = params.getInputType1();
        String input1 = input1Type.equalsIgnoreCase("DB") ? (HOOTAPI_DB_URL + "/" + params.getInput1()) : params.getInput1();

        String input2Type = params.getInputType2();
        String input2 = input2Type.equalsIgnoreCase("DB") ? (HOOTAPI_DB_URL + "/" + params.getInput2()) : params.getInput2();

        String output = HOOTAPI_DB_URL + "/" + params.getOutputName();

        String stats = "";
        if (params.getCollectStats()) {
            // Don't include non-error log messages in stdout because we are redirecting to file
            debugLevel = "error";

            //Hootenanny map statistics such as node and way count
            stats = "--stats";
        }

        Map<String, Object> substitutionMap = new HashMap<>();

        // Detect Differential Conflation
        String conflationCommand = params.getConflationCommand();
        String diffTags = "";
        String differential = "";
        if (conflationCommand != null && conflationCommand.contains("differential-tags")){
          conflationCommand = "conflate";
          differential = "--differential";
          diffTags = "--include-tags";
        }
        else if (conflationCommand != null && conflationCommand.contains("differential")){
          conflationCommand = "conflate";
          differential = "--differential";
        }

        if (params.getAdvancedOptions() != null && !params.getAdvancedOptions().isEmpty()) { // hoot 1
            substitutionMap.put("CONFLATION_TYPE", "");
        	Arrays.stream(toOptionsList(params.getAdvancedOptions())).forEach((option) -> {
                if (!option.isEmpty()) {
                    options.add(option.trim());
                };
            });
        }

        if (params.getHoot2() != null) { // hoot 2
        	String conflationType = ConflationConfs.get(params.getConflationType());
        	if (conflationType != null) {
        		substitutionMap.put("CONFLATION_TYPE", conflationType + ".conf");
        	}
        }

        substitutionMap.put("CONFLATION_COMMAND", conflationCommand);
        substitutionMap.put("DEBUG_LEVEL", debugLevel);
        substitutionMap.put("HOOT_OPTIONS", toHootOptions(options));
        substitutionMap.put("INPUT1", input1);
        substitutionMap.put("INPUT2", input2);
        substitutionMap.put("OUTPUT", output);
        substitutionMap.put("DIFFERENTIAL", differential);
        substitutionMap.put("DIFF_TAGS", diffTags);
        substitutionMap.put("STATS", stats);

        String command = "hoot ${CONFLATION_COMMAND} --${DEBUG_LEVEL} -C RemoveReview2Pre.conf -C ${CONFLATION_TYPE} ${HOOT_OPTIONS} ${INPUT1} ${INPUT2} ${OUTPUT} ${DIFFERENTIAL} ${DIFF_TAGS} ${STATS}";
        super.configureCommand(command, substitutionMap, caller);
    }

    private String[] toOptionsList(String optionsString) {
    	return optionsString.trim().split("-D ");
    }

    @Override
    public CommandResult execute() {
        CommandResult commandResult = super.execute();

        if (conflateParams.getCollectStats()) {
            File statsFile = new File(RPT_STORE_PATH, conflateParams.getOutputName() + "-stats.csv");
            try {
                FileUtils.write(statsFile, commandResult.getStdout(), Charset.defaultCharset());
            }
            catch (IOException ioe) {
                throw new RuntimeException("Error writing to " + statsFile.getAbsolutePath(), ioe);
            }
        }

        return commandResult;
    }
}
