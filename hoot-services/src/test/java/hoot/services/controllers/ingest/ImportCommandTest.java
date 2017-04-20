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


import static hoot.services.HootProperties.TEMP_OUTPUT_PATH;
import static hoot.services.controllers.ingest.UploadClassification.SHP;

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
        List<File> requests = new LinkedList<>();

        List<File> zips = new LinkedList<>();
        zips.add(new File("file.zip"));

        String translation = "";
        String etlName = "ogrImport";
        Boolean isNoneTranslation = false;

        List<String> options = new LinkedList<>();
        options.add("osm2ogr.ops=hoot::DecomposeBuildingRelationsVisitor");
        options.add("hootapi.db.writer.overwrite.map=true");
        options.add("hootapi.db.writer.create.user=true");
        options.add("api.db.email=test@test.com");

        ImportCommand importCommand = new ImportCommand(jobId, workDir, requests, zips, translation,
                etlName, isNoneTranslation, debugLevel, SHP, caller);
    }

    @Test
    public void testImportFGDBCommand() {

    }

    @Test
    public void testImportZIPCommand() {

    }

    @Test
    public void testImportOSMCommand() {

    }

    @Test
    public void testImportGEONAMESCommand() {

    }
}