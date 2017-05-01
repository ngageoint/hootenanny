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
package hoot.services.controllers.ingest;


import static hoot.services.HootProperties.HOME_FOLDER;
import static hoot.services.HootProperties.HOOTAPI_DB_URL;
import static hoot.services.HootProperties.TEMP_OUTPUT_PATH;
import static hoot.services.controllers.ingest.UploadClassification.*;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

import java.io.File;
import java.util.LinkedList;
import java.util.List;
import java.util.UUID;

import org.junit.Test;


public class ImportCommandTest {

    @Test
    public void testImportOGRCommand() {
        String jobId = UUID.randomUUID().toString();
        String debugLevel = "error";
        Class<?> caller = this.getClass();
        File workDir = new File(TEMP_OUTPUT_PATH);
        List<File> filesToImport = new LinkedList<>();
        filesToImport.add(new File("file.shp"));

        List<File> zips = null;
        String translation = "translations/TDSv40.js";
        String etlName = "ogrImport";
        Boolean isNoneTranslation = false;

        List<String> options = new LinkedList<>();
        options.add("osm2ogr.ops=hoot::DecomposeBuildingRelationsVisitor");
        options.add("hootapi.db.writer.overwrite.map=true");
        options.add("hootapi.db.writer.create.user=true");
        options.add("api.db.email=test@test.com");

        ImportCommand importCommand = new ImportCommand(jobId, workDir, filesToImport, zips, translation,
                                      etlName, isNoneTranslation, debugLevel, SHP, caller);

        String hootConvertCommand = "hoot convert --${DEBUG_LEVEL} ${HOOT_OPTIONS} ${INPUTS} ${INPUT_NAME}";
        String hootOGR2OSMCommand = "hoot ogr2osm --${DEBUG_LEVEL} ${HOOT_OPTIONS} ${TRANSLATION_PATH} ${INPUT_NAME} ${INPUTS}";

        assertEquals(jobId, importCommand.getJobId());
        assertEquals(true, importCommand.getTrackable());
        assertNotNull(importCommand.getSubstitutionMap());
        assertNotNull(importCommand.getWorkDir());
        assertNotNull(importCommand.getCommand());

        assertEquals(hootOGR2OSMCommand, importCommand.getCommand());
        assertEquals(1, ((List)importCommand.getSubstitutionMap().get("INPUTS")).size());
        assertTrue(((List)importCommand.getSubstitutionMap().get("INPUTS")).get(0).toString().endsWith("file.shp"));
        assertEquals(HOOTAPI_DB_URL + "/" + etlName, importCommand.getSubstitutionMap().get("INPUT_NAME"));
        assertTrue(importCommand.getSubstitutionMap().get("TRANSLATION_PATH").toString().endsWith(translation));

        isNoneTranslation = true;
        importCommand = new ImportCommand(jobId, workDir, filesToImport, zips, translation,
                                          etlName, isNoneTranslation, debugLevel, SHP, caller);

        assertEquals(hootConvertCommand, importCommand.getCommand());

        assertEquals(jobId, importCommand.getJobId());
        assertEquals(true, importCommand.getTrackable());
        assertNotNull(importCommand.getSubstitutionMap());
        assertNotNull(importCommand.getWorkDir());
        assertNotNull(importCommand.getCommand());

        assertEquals(1, ((List<String>) importCommand.getSubstitutionMap().get("INPUTS")).size());
        assertTrue(((List<String>) importCommand.getSubstitutionMap().get("INPUTS")).get(0).endsWith("file.shp"));
        assertEquals(HOOTAPI_DB_URL + "/" + etlName, importCommand.getSubstitutionMap().get("INPUT_NAME"));
    }

    @Test
    public void testImportFGDBCommand() {
        String jobId = UUID.randomUUID().toString();
        String debugLevel = "error";
        Class<?> caller = this.getClass();
        File workDir = new File(TEMP_OUTPUT_PATH);
        List<File> filesToImport = new LinkedList<>();
        filesToImport.add(new File("file.gdb"));

        List<File> zips = null;
        String translation = "translations/TDSv40.js";
        String etlName = "ogrImport";
        Boolean isNoneTranslation = false;

        List<String> options = new LinkedList<>();
        options.add("osm2ogr.ops=hoot::DecomposeBuildingRelationsVisitor");
        options.add("hootapi.db.writer.overwrite.map=true");
        options.add("hootapi.db.writer.create.user=true");
        options.add("api.db.email=test@test.com");

        ImportCommand importCommand = new ImportCommand(jobId, workDir, filesToImport, zips, translation,
                etlName, isNoneTranslation, debugLevel, FGDB, caller);

        String hootConvertCommand = "hoot convert --${DEBUG_LEVEL} ${HOOT_OPTIONS} ${INPUTS} ${INPUT_NAME}";
        String hootOGR2OSMCommand = "hoot ogr2osm --${DEBUG_LEVEL} ${HOOT_OPTIONS} ${TRANSLATION_PATH} ${INPUT_NAME} ${INPUTS}";

        assertEquals(jobId, importCommand.getJobId());
        assertEquals(true, importCommand.getTrackable());
        assertNotNull(importCommand.getSubstitutionMap());
        assertNotNull(importCommand.getWorkDir());
        assertNotNull(importCommand.getCommand());

        assertEquals(hootOGR2OSMCommand, importCommand.getCommand());
        assertEquals(1, ((List)importCommand.getSubstitutionMap().get("INPUTS")).size());
        assertTrue(((List)importCommand.getSubstitutionMap().get("INPUTS")).get(0).toString().endsWith("file.gdb"));
        assertEquals(HOOTAPI_DB_URL + "/" + etlName, importCommand.getSubstitutionMap().get("INPUT_NAME"));
        assertTrue(importCommand.getSubstitutionMap().get("TRANSLATION_PATH").toString().endsWith(translation));

        isNoneTranslation = true;
        importCommand = new ImportCommand(jobId, workDir, filesToImport, zips, translation,
                etlName, isNoneTranslation, debugLevel, SHP, caller);

        assertEquals(hootConvertCommand, importCommand.getCommand());

        assertEquals(jobId, importCommand.getJobId());
        assertEquals(true, importCommand.getTrackable());
        assertNotNull(importCommand.getSubstitutionMap());
        assertNotNull(importCommand.getWorkDir());
        assertNotNull(importCommand.getCommand());

        assertEquals(1, ((List<String>) importCommand.getSubstitutionMap().get("INPUTS")).size());
        assertTrue(((List<String>) importCommand.getSubstitutionMap().get("INPUTS")).get(0).endsWith("file.gdb"));
        assertEquals(HOOTAPI_DB_URL + "/" + etlName, importCommand.getSubstitutionMap().get("INPUT_NAME"));
    }

    @Test
    public void testImportZIPCommand() {
        String jobId = UUID.randomUUID().toString();
        String debugLevel = "error";
        Class<?> caller = this.getClass();
        File workDir = new File(TEMP_OUTPUT_PATH);
        List<File> filesToImport = new LinkedList<>();

        List<File> zips = new LinkedList<>();
        zips.add(new File("file.zip"));

        String translation = "translations/TDSv40.js";
        String etlName = "ogrImport";
        Boolean isNoneTranslation = false;

        List<String> options = new LinkedList<>();
        options.add("osm2ogr.ops=hoot::DecomposeBuildingRelationsVisitor");
        options.add("hootapi.db.writer.overwrite.map=true");
        options.add("hootapi.db.writer.create.user=true");
        options.add("api.db.email=test@test.com");

        ImportCommand importCommand = new ImportCommand(jobId, workDir, filesToImport, zips, translation,
                etlName, isNoneTranslation, debugLevel, ZIP, caller);

        String hootConvertCommand = "hoot convert --${DEBUG_LEVEL} ${HOOT_OPTIONS} ${INPUTS} ${INPUT_NAME}";
        String hootOGR2OSMCommand = "hoot ogr2osm --${DEBUG_LEVEL} ${HOOT_OPTIONS} ${TRANSLATION_PATH} ${INPUT_NAME} ${INPUTS}";

        assertEquals(jobId, importCommand.getJobId());
        assertEquals(true, importCommand.getTrackable());
        assertNotNull(importCommand.getSubstitutionMap());
        assertNotNull(importCommand.getWorkDir());
        assertNotNull(importCommand.getCommand());

        assertEquals(hootOGR2OSMCommand, importCommand.getCommand());

        assertEquals(1, ((List) importCommand.getSubstitutionMap().get("INPUTS")).size());
        assertTrue(((List) importCommand.getSubstitutionMap().get("INPUTS")).get(0).toString().startsWith("/vsizip/"));
        assertTrue(((List) importCommand.getSubstitutionMap().get("INPUTS")).get(0).toString().endsWith("file.zip"));

        assertEquals(HOOTAPI_DB_URL + "/" + etlName, importCommand.getSubstitutionMap().get("INPUT_NAME"));
        assertTrue(importCommand.getSubstitutionMap().get("TRANSLATION_PATH").toString().endsWith(translation));

        isNoneTranslation = true;
        importCommand = new ImportCommand(jobId, workDir, filesToImport, zips, translation,
                etlName, isNoneTranslation, debugLevel, ZIP, caller);

        assertEquals(hootConvertCommand, importCommand.getCommand());

        assertEquals(jobId, importCommand.getJobId());
        assertEquals(true, importCommand.getTrackable());
        assertNotNull(importCommand.getSubstitutionMap());
        assertNotNull(importCommand.getWorkDir());
        assertNotNull(importCommand.getCommand());

        assertEquals(1, ((List) importCommand.getSubstitutionMap().get("INPUTS")).size());
        assertTrue(((List) importCommand.getSubstitutionMap().get("INPUTS")).get(0).toString().startsWith("/vsizip/"));
        assertTrue(((List) importCommand.getSubstitutionMap().get("INPUTS")).get(0).toString().endsWith("file.zip"));

        assertTrue(((List<String>) importCommand.getSubstitutionMap().get("INPUTS")).get(0).endsWith("file.zip"));
        assertEquals(HOOTAPI_DB_URL + "/" + etlName, importCommand.getSubstitutionMap().get("INPUT_NAME"));
    }

    @Test
    public void testImportOSMCommand() {
        String jobId = UUID.randomUUID().toString();
        String debugLevel = "error";
        Class<?> caller = this.getClass();
        File workDir = new File(TEMP_OUTPUT_PATH);
        List<File> filesToImport = new LinkedList<>();
        filesToImport.add(new File("file.osm"));

        List<File> zips = null;
        String translation = "translations/TDSv40.js";
        String etlName = "ogrImport";
        Boolean isNoneTranslation = false;

        List<String> options = new LinkedList<>();
        options.add("osm2ogr.ops=hoot::DecomposeBuildingRelationsVisitor");
        options.add("hootapi.db.writer.overwrite.map=true");
        options.add("hootapi.db.writer.create.user=true");
        options.add("api.db.email=test@test.com");
        options.add("convert.ops=hoot::TranslationOp");
        options.add("translation.script=" + "/" + translation);

        String hootConvertCommand = "hoot convert --${DEBUG_LEVEL} ${HOOT_OPTIONS} ${INPUTS} ${INPUT_NAME}";

        ImportCommand importCommand = new ImportCommand(jobId, workDir, filesToImport, zips, translation,
                                          etlName, isNoneTranslation, debugLevel, OSM, caller);

        assertEquals(hootConvertCommand, importCommand.getCommand());

        assertEquals(jobId, importCommand.getJobId());
        assertEquals(true, importCommand.getTrackable());
        assertNotNull(importCommand.getSubstitutionMap());
        assertNotNull(importCommand.getWorkDir());
        assertNotNull(importCommand.getCommand());

        assertEquals(1, ((List<String>) importCommand.getSubstitutionMap().get("INPUTS")).size());
        assertTrue(((List<String>) importCommand.getSubstitutionMap().get("INPUTS")).get(0).endsWith("file.osm"));
        assertEquals(HOOTAPI_DB_URL + "/" + etlName, importCommand.getSubstitutionMap().get("INPUT_NAME"));

        List<String> hootOptions = new LinkedList<>();
        options.forEach(option -> { hootOptions.add("-D"); hootOptions.add(option); });

        assertEquals(hootOptions, importCommand.getSubstitutionMap().get("HOOT_OPTIONS"));
    }

    @Test
    public void testImportGEONAMESCommand() {
        String jobId = UUID.randomUUID().toString();
        String debugLevel = "error";
        Class<?> caller = this.getClass();
        File workDir = new File(TEMP_OUTPUT_PATH);
        List<File> filesToImport = new LinkedList<>();
        filesToImport.add(new File("file.geonames"));

        List<File> zips = null;
        String translation = "translations/TDSv40.js";
        String etlName = "ogrImport";
        Boolean isNoneTranslation = false;

        List<String> options = new LinkedList<>();
        options.add("osm2ogr.ops=hoot::DecomposeBuildingRelationsVisitor");
        options.add("hootapi.db.writer.overwrite.map=true");
        options.add("hootapi.db.writer.create.user=true");
        options.add("api.db.email=test@test.com");
        options.add("convert.ops=hoot::TranslationOp");
        options.add("translation.script=" + HOME_FOLDER + "/" + translation);

        String hootConvertCommand = "hoot convert --${DEBUG_LEVEL} ${HOOT_OPTIONS} ${INPUTS} ${INPUT_NAME}";

        ImportCommand importCommand = new ImportCommand(jobId, workDir, filesToImport, zips, translation,
                etlName, isNoneTranslation, debugLevel, OSM, caller);

        assertEquals(hootConvertCommand, importCommand.getCommand());

        assertEquals(jobId, importCommand.getJobId());
        assertEquals(true, importCommand.getTrackable());
        assertNotNull(importCommand.getSubstitutionMap());
        assertNotNull(importCommand.getWorkDir());
        assertNotNull(importCommand.getCommand());

        assertEquals(1, ((List<String>) importCommand.getSubstitutionMap().get("INPUTS")).size());
        assertTrue(((List<String>) importCommand.getSubstitutionMap().get("INPUTS")).get(0).endsWith("file.geonames"));
        assertEquals(HOOTAPI_DB_URL + "/" + etlName, importCommand.getSubstitutionMap().get("INPUT_NAME"));

        List<String> hootOptions = new LinkedList<>();
        options.forEach(option -> { hootOptions.add("-D"); hootOptions.add(option); });

        assertEquals(hootOptions, importCommand.getSubstitutionMap().get("HOOT_OPTIONS"));
    }
}