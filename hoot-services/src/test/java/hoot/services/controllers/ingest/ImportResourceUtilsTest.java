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
import static hoot.services.HootProperties.UPLOAD_FOLDER;
import static hoot.services.controllers.ingest.UploadClassification.*;
import static org.junit.Assert.*;

import java.io.File;
import java.io.IOException;
import java.net.URL;
import java.util.EnumMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import org.apache.commons.io.FileUtils;
import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;

import hoot.services.utils.HootCustomPropertiesSetter;


public class ImportResourceUtilsTest {

    private static File homeFolder;
    private static String original_HOME_FOLDER;
    private static String original_UPLOAD_FOLDER;

    private static void copyResourcesInfoTestFolder(String[] resources) throws IOException {
        for (String resource : resources) {
            URL inputUrl = ImportResourceUtils.class.getResource("/hoot/services/controllers/ingest/ImportResourceTest/" + resource);
            File dest = new File(homeFolder, resource);
            FileUtils.copyURLToFile(inputUrl, dest);
        }
    }

    @BeforeClass
    public static void beforeClass() throws Exception {
        try {
            original_HOME_FOLDER = HOME_FOLDER;
            original_UPLOAD_FOLDER = UPLOAD_FOLDER;
            homeFolder = new File(FileUtils.getTempDirectory(), "ImportTest");
            FileUtils.forceMkdir(homeFolder);
            assertTrue(homeFolder.exists());
            HootCustomPropertiesSetter.setProperty("HOME_FOLDER", homeFolder.getAbsolutePath());
            HootCustomPropertiesSetter.setProperty("UPLOAD_FOLDER", homeFolder.getAbsolutePath() + "/" + "upload");

            copyResourcesInfoTestFolder(new String[] {"ogr.zip", "zip1.zip", "osm.zip",
                    "osm1.osm", "osm2.osm", "fgdb_ogr.zip", "TransportationGroundCrv.shp",  "DcGisRoads.zip" });

//            String command = "/usr/bin/unzip " + new File(homeFolder, "DcGisRoads.zip").getAbsolutePath() +
//                    " -d " + homeFolder.getAbsolutePath();
//
//            Process p = Runtime.getRuntime().exec(command);
        }
        catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    @AfterClass
    public static void afterClass() throws Exception {
        FileUtils.deleteDirectory(homeFolder);
        HootCustomPropertiesSetter.setProperty("HOME_FOLDER", original_HOME_FOLDER);
        HootCustomPropertiesSetter.setProperty("UPLOAD_FOLDER", original_UPLOAD_FOLDER);
    }


    @Test
    public void testClassifyUploadedFile() throws Exception {
        int zipCnt, shpZipCnt, fgdbZipCnt, osmZipCnt, geonamesZipCnt, shpCnt, fgdbCnt, osmCnt, geonamesCnt;

        shpCnt = 1; zipCnt = shpZipCnt = fgdbZipCnt = osmZipCnt = geonamesZipCnt = fgdbCnt = osmCnt = geonamesCnt = 0;
        UploadClassification classification = ImportResourceUtils.finalizeUploadClassification(zipCnt, shpZipCnt, fgdbZipCnt, osmZipCnt,
                                        geonamesZipCnt, shpCnt, fgdbCnt, osmCnt, geonamesCnt);
        assertEquals(SHP, classification);

        osmCnt = 1; zipCnt = shpZipCnt = fgdbZipCnt = osmZipCnt = geonamesZipCnt = shpCnt = fgdbCnt = geonamesCnt = 0;
        classification = ImportResourceUtils.finalizeUploadClassification(zipCnt, shpZipCnt, fgdbZipCnt, osmZipCnt,
                                            geonamesZipCnt, shpCnt, fgdbCnt, osmCnt, geonamesCnt);
        assertEquals(OSM, classification);

        fgdbCnt = 1; zipCnt = shpZipCnt = fgdbZipCnt = osmZipCnt = geonamesZipCnt = shpCnt = osmCnt = geonamesCnt = 0;
        classification = ImportResourceUtils.finalizeUploadClassification(zipCnt, shpZipCnt, fgdbZipCnt, osmZipCnt,
                geonamesZipCnt, shpCnt, fgdbCnt, osmCnt, geonamesCnt);
        assertEquals(FGDB, classification);

        geonamesCnt = 1; zipCnt = shpZipCnt = fgdbZipCnt = osmZipCnt = geonamesZipCnt = shpCnt = osmCnt = fgdbCnt = 0;
        classification = ImportResourceUtils.finalizeUploadClassification(zipCnt, shpZipCnt, fgdbZipCnt, osmZipCnt,
                geonamesZipCnt, shpCnt, fgdbCnt, osmCnt, geonamesCnt);
        assertEquals(GEONAMES, classification);

        zipCnt = 1; fgdbZipCnt = 1; shpZipCnt = osmZipCnt = geonamesZipCnt = shpCnt = osmCnt = fgdbCnt = geonamesCnt = 0;
        classification = ImportResourceUtils.finalizeUploadClassification(zipCnt, shpZipCnt, fgdbZipCnt, osmZipCnt,
                geonamesZipCnt, shpCnt, fgdbCnt, osmCnt, geonamesCnt);
        assertEquals(FGDB, classification);

        zipCnt = 1; shpCnt = 1; shpZipCnt = osmZipCnt = geonamesZipCnt = osmCnt = fgdbZipCnt = fgdbCnt = geonamesCnt = 0;
        classification = ImportResourceUtils.finalizeUploadClassification(zipCnt, shpZipCnt, fgdbZipCnt, osmZipCnt,
                geonamesZipCnt, shpCnt, fgdbCnt, osmCnt, geonamesCnt);
        assertEquals(SHP, classification);

        zipCnt = 1; osmCnt = 1; shpZipCnt = osmZipCnt = geonamesZipCnt = shpCnt = fgdbZipCnt = fgdbCnt = geonamesCnt = 0;
        classification = ImportResourceUtils.finalizeUploadClassification(zipCnt, shpZipCnt, fgdbZipCnt, osmZipCnt,
                geonamesZipCnt, shpCnt, fgdbCnt, osmCnt, geonamesCnt);
        assertEquals(OSM, classification);

        zipCnt = 1; osmZipCnt = 1; shpZipCnt = geonamesCnt = geonamesZipCnt = shpCnt = osmCnt = fgdbZipCnt = fgdbCnt = 0;
        classification = ImportResourceUtils.finalizeUploadClassification(zipCnt, shpZipCnt, fgdbZipCnt, osmZipCnt,
                geonamesZipCnt, shpCnt, fgdbCnt, osmCnt, geonamesCnt);
        assertEquals(ZIP, classification);
    }

    @Test
    public void testHandleUploadedFile() throws Exception {
        Map<UploadClassification, Integer> counts = new EnumMap<>(UploadClassification.class);
        File workDir = new File("");
        String uploadType = "FILE";

        File uploadedFile = new File("file.shp");
        List<File> filesToImport = ImportResourceUtils.handleUploadedFile(SHP, uploadedFile, counts, workDir, uploadType);
        assertNotNull(filesToImport);
        assertEquals(1, filesToImport.size());
        assertEquals(1, counts.get(SHP).intValue());

        counts.clear();
        uploadedFile = new File("file.osm");
        filesToImport = ImportResourceUtils.handleUploadedFile(OSM, uploadedFile, counts, workDir, uploadType);
        assertNotNull(filesToImport);
        assertEquals(1, filesToImport.size());
        assertEquals(1, counts.get(OSM).intValue());

        counts.clear();
        uploadedFile = new File("file.osm.pbf");
        filesToImport = ImportResourceUtils.handleUploadedFile(PBF, uploadedFile, counts, workDir, uploadType);
        assertNotNull(filesToImport);
        assertEquals(1, filesToImport.size());
        assertEquals(1, counts.get(OSM).intValue());

        counts.clear();
        uploadedFile = new File("file.geonames");
        filesToImport = ImportResourceUtils.handleUploadedFile(GEONAMES, uploadedFile, counts, workDir, uploadType);
        assertNotNull(filesToImport);
        assertEquals(1, filesToImport.size());
        assertEquals(1, counts.get(GEONAMES).intValue());

        counts.clear();
        uploadedFile = new File("file.txt");
        filesToImport = ImportResourceUtils.handleUploadedFile(TXT, uploadedFile, counts, workDir, uploadType);
        assertNotNull(filesToImport);
        assertEquals(1, filesToImport.size());
        assertEquals(1, counts.get(GEONAMES).intValue());

        counts.clear();
        uploadedFile = new File("gdb");
        filesToImport = ImportResourceUtils.handleUploadedFile(FGDB, uploadedFile, counts, workDir, "DIR");
        assertNotNull(filesToImport);
        assertEquals(1, filesToImport.size());
        assertEquals(1, counts.get(FGDB).intValue());

        counts.clear();
        uploadedFile = new File("gdb");
        filesToImport = ImportResourceUtils.handleUploadedFile(FGDB, uploadedFile, counts, workDir, "DIR");
        assertNotNull(filesToImport);
        assertEquals(1, filesToImport.size());
        assertEquals(1, counts.get(FGDB).intValue());
    }

    @Test
    public void testSpecialHandleWhenZIP() throws Exception {
        //TODO: add implementation
    }

    @Test
    public void testHandleOSMZip() throws Exception {
        File workDir = homeFolder;
        List<File> zipsToImport = new LinkedList<>();
        zipsToImport.add(new File(homeFolder, "osm.zip"));
        List<File> filesToImport = new LinkedList<>();
        List<String> fileNames = new LinkedList<>();
        fileNames.add("osm.zip");

        String command = "/usr/bin/unzip " + new File(homeFolder, "osm.zip").getAbsolutePath() +
                " -d " + new File(homeFolder, "osm").getAbsolutePath();

        Process p = Runtime.getRuntime().exec(command);

        ImportResourceUtils.handleOSMZip(workDir, zipsToImport, filesToImport, fileNames);

        assertNotNull(filesToImport);
        assertEquals(2, filesToImport.size());
    }

    @Test
    public void testHandleGEONAMESWithTxtExtension() throws Exception {
        File workDir = homeFolder;
        File geonamesFile = new File(workDir, "file.txt");
        List<File> zipsToImport = new LinkedList<>();
        List<File> filesToImport = new LinkedList<>();
        filesToImport.add(geonamesFile);
        FileUtils.touch(geonamesFile);
        List<String> fileNames = new LinkedList<>();
        fileNames.add("file.txt");

        try {
            ImportResourceUtils.handleGEONAMESWithTxtExtension(workDir, geonamesFile, fileNames, filesToImport);

            assertNotNull(filesToImport);
            assertEquals(1, filesToImport.size());
            assertTrue(filesToImport.get(0).getName().endsWith("file.geonames"));
        }
        finally {
            FileUtils.deleteQuietly(geonamesFile);
        }
    }
}