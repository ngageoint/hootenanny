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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.export;

import java.util.LinkedList;
import java.util.List;
import java.util.stream.Collectors;


class ExportOSMCommand extends ExportCommand {

    ExportOSMCommand(String jobId, ExportParams params, String debugLevel, Class<?> caller) {
        super(jobId, params, debugLevel, caller);

        //# Options for osm & osm.pbf export
        // OSM_OPTS=-D hootapi.db.writer.create.user=true -D api.db.email=test@test.com
        List<String> options = new LinkedList<>();
        options.add("-D hootapi.db.writer.create.user=true");
        options.add("-D api.db.email=test@test.com");

        //# Add the option to have status tags as text with "Input1" instead of "1" or "Unknown1"
        //ifeq "$(textstatus)" "true"
        //    OSM_OPTS+= -D writer.text.status=true
        //endif

        if (params.getTextStatus()) {
            options.add("-D writer.text.status=true");
        }

        String osmOptions = options.stream().collect(Collectors.joining(" "));
        String input = super.getInput();
        String outputPath = super.getOutputPath();

        //hoot convert $(OSM_OPTS) "$(INPUT_PATH)" "$(OP_OUTPUT)"
        String command = "hoot convert --" + debugLevel + " " + osmOptions + " " + quote(input) + " " + quote(outputPath);

        super.configureCommand(command, caller);
    }
}
