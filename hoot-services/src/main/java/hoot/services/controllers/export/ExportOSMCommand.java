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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.export;

import java.io.File;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import hoot.services.HootProperties;
import hoot.services.models.db.Users;


class ExportOSMCommand extends ExportCommand {
    ExportOSMCommand(String jobId, ExportParams params, String debugLevel, Class<?> caller, Users user) {
        super(jobId, params);
        if(user != null) {
            // ensure user's email set properly.
            params.setUserEmail(user.getEmail());
        }


        List<String> options = new LinkedList<>();
        options.add("api.db.email=" + params.getUserEmail());

        if (!params.getTagOverrides().isEmpty() && !params.getIncludeHootTags()) {
            options.add("convert.ops=hoot::TranslationOp;hoot::RemoveElementsVisitor");
            options.add("remove.elements.visitor.element.criterion=hoot::ReviewRelationCriterion");
            options.add("remove.elements.visitor.recursive=false");
        } else if (!params.getTagOverrides().isEmpty()) {
            options.add("convert.ops=hoot::TranslationOp");
        }

        if (!params.getTagOverrides().isEmpty()) {

            File trans = new File(new File(HootProperties.HOME_FOLDER, "translations"),"OSM_Export.js");
            options.add("translation.script=" + trans.getAbsolutePath());

            options.add("translation.override=" + params.getTagOverrides() );
        }

        if (params.getTextStatus()) {
            options.add("writer.text.status=true");
        }

        List<String> hootOptions = toHootOptions(options);

        String input = super.getInput();
        String outputPath = super.getOutputPath();

        Map<String, Object> substitutionMap = new HashMap<>();
        substitutionMap.put("DEBUG_LEVEL", debugLevel);
        substitutionMap.put("HOOT_OPTIONS", hootOptions);
        substitutionMap.put("INPUT", input);
        substitutionMap.put("OUTPUT_PATH", outputPath);

        String command = "hoot convert --${DEBUG_LEVEL} ${HOOT_OPTIONS} ${INPUT} ${OUTPUT_PATH}";

        super.configureCommand(command, substitutionMap, caller);
    }
}
