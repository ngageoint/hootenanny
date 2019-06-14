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
package hoot.services.controllers.export;

import static hoot.services.HootProperties.HOME_FOLDER;
import static hoot.services.HootProperties.HOOTAPI_DB_URL;
import static hoot.services.HootProperties.TEMP_OUTPUT_PATH;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;

import org.apache.commons.io.FileUtils;
import org.apache.commons.lang3.StringUtils;

import hoot.services.command.ExternalCommand;
import hoot.services.command.common.UnTARFileCommand;
import hoot.services.models.db.Users;


class ExportCommand extends ExternalCommand {
    private final ExportParams params;

    ExportCommand(String jobId, ExportParams exportParams) {
        super(jobId);
        this.params = exportParams;
    }

    ExportCommand(String jobId, ExportParams params, String debugLevel, Class<?> caller, Users user) {
        this(jobId, params);
        if(user != null) {
            params.setUserEmail(user.getEmail());
        }
        //If no translation, we are keeping osm tag schema, but still provide the
        //following no-op translation so that tag overrides can be run
        if (params.getTranslation() == null || params.getTranslation().isEmpty()) {
            params.setTranslation("translations/OSM_Export.js");
        }
        if (params.getAppend()) {
            appendToFGDB();
        }

        List<String> hootOptions = toHootOptions(this.getCommonExportHootOptions());

        java.util.Map<String, Object> substitutionMap = new HashMap<>();
        substitutionMap.put("DEBUG_LEVEL", debugLevel);
        substitutionMap.put("HOOT_OPTIONS", hootOptions);
        substitutionMap.put("INPUT_PATH", this.getInput());
        substitutionMap.put("OUTPUT_PATH", this.getOutputPath());

        String command = "hoot convert --${DEBUG_LEVEL} ${HOOT_OPTIONS} ${INPUT_PATH} ${OUTPUT_PATH}";

        super.configureCommand(command, substitutionMap, caller);
    }

    private void appendToFGDB() {
        //Appends data to a blank fgdb. The template is stored with the fouo translations.

        //$(HOOT_HOME)/translations-local/template
        File templateHome = new File(new File(HOME_FOLDER, "translations-local"), "template");

        String translation = params.getTranslation();
        File tdsTemplate = null;

        if (translation.equalsIgnoreCase("translations/TDSv61.js")) {
            tdsTemplate = new File(templateHome, "tds61.tgz");
        }
        else if (translation.equalsIgnoreCase("translations/TDSv40.js")) {
            tdsTemplate = new File(templateHome, "tds40.tgz");
        }

        if ((tdsTemplate != null) && tdsTemplate.exists()) {
            File outputDir = new File(this.getWorkFolder(), params.getOutputName() + "."     + params.getOutputType().toLowerCase());
            try {
                FileUtils.forceMkdir(outputDir);
            }
            catch (IOException ioe) {
                throw new RuntimeException("Error creating directory: " + outputDir.getAbsolutePath(), ioe);
            }

            ExternalCommand untarFileCommand = new UnTARFileCommand(tdsTemplate, outputDir, this.getClass());
            untarFileCommand.execute();
        }
    }

    List<String> getCommonExportHootOptions() {
        List<String> options = new LinkedList<>();

        //Sets the user reading this data
        options.add("api.db.email=" + params.getUserEmail());

        //Convert ops, ORDER IS IMPORTANT
        //we want to translate last
        List<String> convertOps = new ArrayList<>();

        //Remove review relations, but do not remove matched review features
        //like we do before conflating a merged dataset as a new input
        if (!params.getIncludeHootTags()) {
            convertOps.add("hoot::RemoveElementsVisitor");
            options.add("remove.elements.visitor.element.criteria=hoot::ReviewRelationCriterion");
            options.add("remove.elements.visitor.recursive=false");
            options.add("writer.include.circular.error.tags=false");

            convertOps.add("hoot::RemoveTagsVisitor");
            options.add("tag.filter.keys=hoot:status;hoot:building:match;error:circular");
        }

        //Decompose building relations for non-osm formats only
        if (!params.getOutputType().equalsIgnoreCase("osm") && !params.getOutputType().equalsIgnoreCase("osm.pbf")) {
            convertOps.add("hoot::DecomposeBuildingRelationsVisitor");

        }

        //Translate the features (which includes applying tag overrides set below)
        convertOps.add("hoot::SchemaTranslationVisitor");
        options.add("schema.translation.direction=toogr");
        options.add("schema.translation.script=" + new File(HOME_FOLDER, params.getTranslation()).getAbsolutePath());

        // By default export removes hoot conflation review related tags
        if (!params.getTagOverrides().isEmpty()) {
            options.add("schema.translation.override=" + params.getTagOverrides() );
        }

        // Add the option to have status tags as text with "Input1" instead of "1" or "Unknown1"
        if (params.getTextStatus()) {
            options.add("writer.text.status=true");
        }

        // Add the option to append to a gdb template
        if (params.getAppend()) {
            options.add("ogr.append.data=true");
        }

        //Add the orders convert operations
        options.add("convert.ops=" + String.join(";", convertOps));

        //Add conf files for specific translation ops
        if (params.getTranslation().equalsIgnoreCase("translations/DNC.js")) {
            options.add("-C DncExport.conf");
        }

        if (params.getTranslation().equalsIgnoreCase("translations/MGCP_TRD4_Cartographic.js")) {
            options.add("-C MgcpCartoExport.conf");
        }

        return options;
    }

    String getOutputPath() {
        File outputFolder = this.getWorkFolder();
        File outputFile;

        if (!StringUtils.isBlank(params.getOutputName())) {
            outputFile = new File(outputFolder, params.getOutputName() + "." + params.getOutputType());
        }
        else {
            outputFile = new File(outputFolder, getJobId() + "." + params.getOutputType());
        }

        return outputFile.getAbsolutePath();
    }

    String getInput() {
        if (! params.getInputType().equalsIgnoreCase("file")) {
            return HOOTAPI_DB_URL + "/" + params.getInput();
        }

        return params.getInput();
    }

    private File getWorkFolder() {
        return new File(TEMP_OUTPUT_PATH, getJobId());
    }
}
