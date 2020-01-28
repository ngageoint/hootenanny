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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.grail;

import java.io.File;
import java.io.FilenameFilter;
import java.util.Arrays;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.command.CommandResult;


/**
 * Used for pushing OSM data to the database
 */
class MergeOsmFilesCommand extends GrailCommand {
    private static final Logger logger = LoggerFactory.getLogger(MergeOsmFilesCommand.class);

    private final Map<String, Object> substitutionMap = new HashMap<>();
    private final Class<?> caller;

    private static final FilenameFilter filter = new FilenameFilter() {
        @Override
        public boolean accept(File f, String name) {
            // We want to find only .osm files
            return name.endsWith(".osm");
        }
    };

    MergeOsmFilesCommand(String jobId, GrailParams params, String debugLevel, Class<?> caller) {
        super(jobId, params);
        logger.info("Params: " + params);

        List<String> options = new LinkedList<>();

        List<String> hootOptions = toHootOptions(options);

        this.caller = caller;

        substitutionMap.put("DEBUG_LEVEL", debugLevel);
        substitutionMap.put("HOOT_OPTIONS", hootOptions);
        substitutionMap.put("OUTPUT", params.getOutput());
    }

    @Override
    public CommandResult execute() {
        //Get list of osm files in work dir
        File workDir = params.getWorkDir();
        File[] osmfiles = workDir.listFiles(filter);
        List<String> filePaths = Arrays.asList(osmfiles).stream().map(File::getAbsolutePath).collect(Collectors.toList());

        substitutionMap.put("INPUT", String.join(" ", filePaths));

        String command = "hoot convert --${DEBUG_LEVEL} ${HOOT_OPTIONS} ${INPUT} ${OUTPUT}";

        super.configureCommand(command, substitutionMap, caller);

        CommandResult commandResult = super.execute();

        return commandResult;
    }
}
