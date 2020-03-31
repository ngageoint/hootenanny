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
package hoot.services.controllers.ogr;

import java.io.File;
import java.io.IOException;
import java.nio.charset.Charset;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

import org.apache.commons.io.FileUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.command.CommandResult;
import hoot.services.command.ExternalCommand;


class ExtractAttributesCommand extends ExternalCommand {
    private static final Logger logger = LoggerFactory.getLogger(ExtractAttributesCommand.class);

    private final File workDir;

    ExtractAttributesCommand(String jobId, File workDir, List<File> files, String debugLevel, Class<?> caller) {
        super(jobId);
        this.workDir = workDir;

        List<String> inputFiles = files.stream().map(File::getAbsolutePath).collect(Collectors.toList());

        Map<String, Object> substitutionMap = new HashMap<>();
        substitutionMap.put("DEBUG_LEVEL", debugLevel);
        substitutionMap.put("INPUT_FILES", inputFiles);

        //Technically this command now works for all data source, not just OGR, so we could move this class and associated classes out of the
        //hoot.services.controllers.ogr namespace.
        String command = "hoot.bin tag-info --${DEBUG_LEVEL} --tag-values-limit 30 ${INPUT_FILES}";

        super.configureCommand(command, substitutionMap, caller);
    }

    @Override
    public CommandResult execute() {
        CommandResult commandResult = super.execute();

        File outputFile = ExtractAttributesUtils.getAttributesOutputFile(super.getJobId());
        try {
            FileUtils.write(outputFile, commandResult.getStdout(), Charset.defaultCharset());
        }
        catch (IOException ioe) {
            throw new RuntimeException("Error writing attributes to: " + outputFile.getAbsolutePath(), ioe);
        }

        try {
            FileUtils.forceDelete(this.workDir);
        }
        catch (IOException ioe) {
            logger.error("Error deleting folder: {} ", this.workDir.getAbsolutePath(), ioe);
        }

        return commandResult;
    }
}
