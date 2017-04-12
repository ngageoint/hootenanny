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
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import java.io.File;
import java.io.IOException;
import java.io.OutputStream;
import java.lang.reflect.Method;
import java.net.URL;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import org.apache.commons.fileupload.FileItem;
import org.apache.commons.fileupload.FileItemFactory;
import org.apache.commons.fileupload.disk.DiskFileItemFactory;
import org.apache.commons.io.FileUtils;
import org.json.simple.JSONArray;
import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Ignore;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import hoot.services.UnitTest;
import hoot.services.utils.HootCustomPropertiesSetter;


@Ignore
public class FileUploadResourceTest {
    private static File homeFolder;
    private static String original_HOME_FOLDER;
    private static String original_UPLOAD_FOLDER;

    private static void copyResourcesInfoTestFolder(String[] resources) throws IOException {
        for (String resource : resources) {
            URL inputUrl = FileUploadResourceTest.class.getResource("/hoot/services/controllers/ingest/FileUploadResourceTest/" + resource);
            File dest = new File(homeFolder, resource);
            FileUtils.copyURLToFile(inputUrl, dest);
        }
    }


    @BeforeClass
    public static void beforeClass() throws Exception {
        try {
            original_HOME_FOLDER = HOME_FOLDER;
            original_UPLOAD_FOLDER = UPLOAD_FOLDER;
            homeFolder = new File(FileUtils.getTempDirectory(), "FileUploadResourceTest");
            FileUtils.forceMkdir(homeFolder);
            assertTrue(homeFolder.exists());

            HootCustomPropertiesSetter.setProperty("HOME_FOLDER", homeFolder.getAbsolutePath());
            HootCustomPropertiesSetter.setProperty("UPLOAD_FOLDER", homeFolder.getAbsolutePath() + "/" + "upload");

            copyResourcesInfoTestFolder(new String[]
                    {"ogr.zip", "zip1.zip", "osm.zip", "osm1.osm", "osm2.osm",
                     "fgdb_ogr.zip", "TransportationGroundCrv.shp",  "DcGisRoads.zip" });

            String command = "/usr/bin/unzip " + new File(homeFolder, "DcGisRoads.zip").getAbsolutePath() +
                    " -d " + homeFolder.getAbsolutePath();
            Process p = Runtime.getRuntime().exec(command);
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
    @Category(UnitTest.class)
    public void TestFgdbOgrRequest() throws Exception {
        String jobId = "test-id-123";
        String wkdirpath = UPLOAD_FOLDER + File.separator + jobId;
        File workingDir = new File(wkdirpath);
        FileUtils.forceMkdir(workingDir);
        assertTrue(workingDir.exists());

        File srcFile = new File(homeFolder, "fgdb_ogr.zip");
        File destFile = new File(wkdirpath, "fgdb_ogr.zip");
        FileUtils.copyFile(srcFile, destFile);
        assertTrue(destFile.exists());

        FileUploadResource res = new FileUploadResource();

        // Let's test zip
        List<Map<String, String>> results = new LinkedList<>();
        Map<String, Integer> zipStat = new HashMap<>();

        Method analyzeUploadedFileMethod = getAnalyzeUploadedFileMethod();
        results.addAll((List<Map<String, String>>) analyzeUploadedFileMethod.invoke(res, "zip", destFile, zipStat, workingDir, "FILE"));

        assertEquals(2, results.size());

        for (Map<String, String> request : results) {
            if (request.get("type").equalsIgnoreCase("FGDB_ZIP")) {
                assertEquals(workingDir.getAbsoluteFile() + "/DcGisRoads.gdb", request.get("name"));
            }
            else if (request.get("type").equalsIgnoreCase("OGR_ZIP")) {
                assertEquals(workingDir.getAbsoluteFile() + "/fgdb_ogr/jakarta_raya_coastline.shp", request.get("name"));
            }
        }

        FileUtils.forceDelete(workingDir);
    }

    @Test(expected = Exception.class)
    @Category(UnitTest.class)
    public void TestFgdbOsmRequest() throws Exception {
        String input = "fgdb_osm.zip";
        String jobId = "test-id-123";
        String wkdirpath = UPLOAD_FOLDER + File.separator + jobId;
        File workingDir = new File(wkdirpath);
        FileUtils.forceMkdir(workingDir);
        assertTrue(workingDir.exists());

        File srcFile = new File(homeFolder, "test-files/service/FileUploadResourceTest/" + input);
        File destFile = new File(wkdirpath, input);
        FileUtils.copyFile(srcFile, destFile);
        assertTrue(destFile.exists());

        FileUploadResource res = new FileUploadResource();

        // Let's test zip
        JSONArray results = new JSONArray();
        Map<String, Integer> zipStat = new HashMap<>();

        try {
            Method analyzeUploadedFileMethod = getAnalyzeUploadedFileMethod();
            analyzeUploadedFileMethod.invoke(res, "fgdb_osm", "zip", input, results, zipStat);
        }
        catch (Exception ex) {
            assertEquals("Zip should not contain both osm and ogr types.", ex.getMessage());
            throw ex;
        }

        FileUtils.forceDelete(workingDir);
    }

    @Test
    @Category(UnitTest.class)
    public void testFgdbOgrZipRequest() throws Exception {
        String input = "fgdb_ogr.zip";
        String jobId = "test-id-123";
        String wkdirpath = UPLOAD_FOLDER + File.separator + jobId;
        File workingDir = new File(wkdirpath);
        FileUtils.forceMkdir(workingDir);
        assertTrue(workingDir.exists());

        File srcFile = new File(homeFolder, input);
        File destFile = new File(wkdirpath, input);
        FileUtils.copyFile(srcFile, destFile);
        assertTrue(destFile.exists());

        FileUploadResource res = new FileUploadResource();

        // Let's test zip
        List<Map<String, String>> results = new LinkedList<>();
        Map<String, Integer> zipStat = new HashMap<>();
        List<String> inputsList = new ArrayList<>();
        inputsList.add(input);

        Method analyzeUploadedFileMethod = getAnalyzeUploadedFileMethod();
        results.addAll((List<Map<String, String>>) analyzeUploadedFileMethod.invoke(res, "zip", destFile, zipStat, workingDir, "FILE", jobId));

        List<File> zipList = new ArrayList<>();

        int shpZipCnt = zipStat.get("shpzipcnt");
        int fgdbZipCnt = zipStat.get("fgdbzipcnt");
        int osmZipCnt = zipStat.get("osmzipcnt");
        int geonamesZipCnt = zipStat.get("geonameszipcnt");

        zipList.add(destFile);
        int zipCnt = 0;
        zipCnt++;

        // Test zip containing fgdb + shp
        int shpCnt = 0;
        int osmCnt = 0;
        int geonamesCnt = 0;
        int fgdbCnt = 0;

        String inputType = "";

        FileETLCommand command = new FileETLCommand(results, zipList, "TDSv61.js", "fgdb_ogr",
                Boolean.FALSE, "error", inputType, this.getClass());
/*
        JSONArray params = (JSONArray) command.get("params");
        int nP = 0;

        for (Object o : params) {
            JSONObject oJ = (JSONObject) o;

            if (oJ.get("INPUT") != null) {
                assertEquals("\"fgdb_ogr/DcGisRoads.gdb\" \"fgdb_ogr/jakarta_raya_coastline.shp\" ", oJ.get("INPUT").toString());
                nP++;
            }

            if (oJ.get("INPUT_PATH") != null) {
                assertTrue(oJ.get("INPUT_PATH").toString().endsWith("/upload/test-id-123"));
                nP++;
            }

            if (oJ.get("INPUT_TYPE") != null) {
                assertEquals("OGR", oJ.get("INPUT_TYPE").toString());
                nP++;
            }

            if (oJ.get("UNZIP_LIST") != null) {
                assertEquals("fgdb_ogr", oJ.get("UNZIP_LIST").toString());
                nP++;
            }
        }

        assertEquals(4, nP);
*/
        FileUtils.forceDelete(workingDir);
    }

    @Test
    @Category(UnitTest.class)
    public void TestOgrZipAndShpRequest() throws Exception {
        String jobId = "test-id-123";
        String wkdirpath = UPLOAD_FOLDER + File.separator + jobId;
        File workingDir = new File(wkdirpath);
        FileUtils.forceMkdir(workingDir);
        assertTrue(workingDir.exists());

        String input = "fgdb_ogr.zip";
        File srcFile = new File(homeFolder, input);
        File destFile = new File(wkdirpath, input);
        FileUtils.copyFile(srcFile, destFile);
        assertTrue(destFile.exists());

        FileUploadResource res = new FileUploadResource();

        // Let's test zip
        List<Map<String, String>> results = new LinkedList<>();
        Map<String, Integer> zipStat = new HashMap<>();
        List<String> inputsList = new ArrayList<>();
        inputsList.add(input);

        Method analyzeUploadedFileMethod = getAnalyzeUploadedFileMethod();
        results.addAll((List<Map<String, String>>) analyzeUploadedFileMethod.invoke(res, "zip", destFile, zipStat, workingDir, "FILE"));

        List<String> zipList = new ArrayList<>();

        int shpZipCnt = zipStat.get("shpzipcnt");
        int fgdbZipCnt = zipStat.get("fgdbzipcnt");
        int osmZipCnt = zipStat.get("osmzipcnt");
        int geonamesZipCnt = zipStat.get("osmzipcnt");

        zipList.add("fgdb_ogr");
        int zipCnt = 0;
        zipCnt++;

        // shape
        input = "TransportationGroundCrv.shp";
        srcFile = new File(homeFolder, input);
        destFile = new File(wkdirpath, input);
        FileUtils.copyFile(srcFile, destFile);
        assertTrue(destFile.exists());

        inputsList.add(input);

/*
        buildNativeRequestMethod.invoke(res, jobId, "TransportationGroundCrv", "shp", input, results, zipStat);

        int shpCnt = 0;
        shpCnt += (Integer) zipStat.get("shpcnt");

        int fgdbCnt = 0;
        fgdbCnt += (Integer) zipStat.get("fgdbcnt");

        int osmCnt = 0;
        osmCnt += (Integer) zipStat.get("osmcnt");

        int geonamesCnt = 0;
        geonamesCnt += (Integer) zipStat.get("geonamescnt");

        String inputType = "";

        // Test zip containing fgdb + shp
        FileETLCommand command = new FileETLCommand(results, zipList, "TDSv61.js", jobId,
                "fgdb_ogr", Boolean.FALSE, "error", null, inputType, this.getClass());

        JSONArray params = (JSONArray) command.get("params");

        int nP = 0;

        for (Object o : params) {
            JSONObject oJ = (JSONObject) o;

            if (oJ.get("INPUT") != null) {
                assertEquals("\"fgdb_ogr/DcGisRoads.gdb\" \"fgdb_ogr/jakarta_raya_coastline.shp\" \"TransportationGroundCrv.shp\" ", oJ.get("INPUT").toString());
                nP++;
            }

            if (oJ.get("INPUT_PATH") != null) {
                assertTrue(oJ.get("INPUT_PATH").toString().endsWith("/upload/test-id-123"));
                nP++;
            }

            if (oJ.get("INPUT_TYPE") != null) {
                assertEquals("OGR", oJ.get("INPUT_TYPE").toString());
                nP++;
            }

            if (oJ.get("UNZIP_LIST") != null) {
                assertEquals("fgdb_ogr", oJ.get("UNZIP_LIST").toString());
                nP++;
            }
        }

        assertEquals(4, nP);
*/
        FileUtils.forceDelete(workingDir);
    }

    @Test
    @Category(UnitTest.class)
    public void TestOsmZipAndOsmRequest() throws Exception {
        String jobId = "test-id-123";
        String wkdirpath = UPLOAD_FOLDER + File.separator + jobId;
        File workingDir = new File(wkdirpath);
        FileUtils.forceMkdir(workingDir);

        assertTrue(workingDir.exists());

        String input = "osm.zip";
        File srcFile = new File(homeFolder, input);
        File destFile = new File(wkdirpath, input);
        FileUtils.copyFile(srcFile, destFile);

        assertTrue(destFile.exists());

        FileUploadResource res = new FileUploadResource();

        // Let's test zip
        List<Map<String, String>> results = new LinkedList<>();
        Map<String, Integer> zipStat = new HashMap<>();
        List<String> inputsList = new ArrayList<>();
        inputsList.add(input);

        Method analyzeUploadedFileMethod = getAnalyzeUploadedFileMethod();
        results.addAll((List<Map<String, String>>) analyzeUploadedFileMethod.invoke(res, "zip", destFile, zipStat, workingDir, "FILE"));

        List<File> zipList = new ArrayList<>();

        int shpZipCnt = zipStat.get("shpzipcnt");
        int fgdbZipCnt = zipStat.get("fgdbzipcnt");
        int osmZipCnt = zipStat.get("osmzipcnt");
        int geonamesZipCnt = zipStat.get("osmzipcnt");

        zipList.add(destFile);
        int zipCnt = 0;
        zipCnt++;

        // osm
        input = "osm1.osm";
        srcFile = new File(homeFolder, input);
        destFile = new File(wkdirpath, input);
        FileUtils.copyFile(srcFile, destFile);
        assertTrue(destFile.exists());

        inputsList.add(input);

        results.addAll((List<Map<String, String>>) analyzeUploadedFileMethod.invoke(res, "osm", destFile, zipStat, workingDir, "FILE"));

        int shpCnt = zipStat.get("shpcnt");
        int fgdbCnt = zipStat.get("fgdbcnt");
        int osmCnt = zipStat.get("osmcnt");
        int geonamesCnt = zipStat.get("osmcnt");

        String inputType = "";

        // Test zip containing fgdb + shp
        FileETLCommand command = new FileETLCommand(results, zipList, "TDSv61.js", "osm",
                Boolean.FALSE, "error", inputType, this.getClass());

/*
        JSONArray params = (JSONArray) command.get("params");

        int nP = 0;

        for (Object o : params) {
            JSONObject oJ = (JSONObject) o;

            if (oJ.get("INPUT") != null) {
                assertEquals("\"osm/DcGisRoads.osm\" \"osm/DcTigerRoads.osm\" \"osm1.osm\" ", oJ.get("INPUT").toString());
                nP++;
            }

            if (oJ.get("INPUT_PATH") != null) {
                assertTrue(oJ.get("INPUT_PATH").toString().endsWith("/upload/test-id-123"));
                nP++;
            }

            if (oJ.get("INPUT_TYPE") != null) {
                assertEquals("OSM", oJ.get("INPUT_TYPE").toString());
                nP++;
            }

            if (oJ.get("UNZIP_LIST") != null) {
                assertEquals("osm", oJ.get("UNZIP_LIST").toString());
                nP++;
            }
        }

        assertEquals(4, nP);
*/
        FileUtils.forceDelete(workingDir);
    }

    @Test
    @Category(UnitTest.class)
    public void testOgrZipRequest() throws Exception {
        String jobId = "test-id-123";
        String wkdirpath = UPLOAD_FOLDER + File.separator + jobId;
        File workingDir = new File(wkdirpath);
        FileUtils.forceMkdir(workingDir);
        assertTrue(workingDir.exists());

        String input = "ogr.zip";
        File srcFile = new File(homeFolder, input);
        File destFile = new File(wkdirpath, input);
        FileUtils.copyFile(srcFile, destFile);
        assertTrue(destFile.exists());

        FileUploadResource res = new FileUploadResource();

        // Let's test zip
        List<Map<String, String>> results = new LinkedList<>();
        Map<String, Integer> zipStat = new HashMap<>();
        List<String> inputsList = new ArrayList<>();
        inputsList.add(input);

        Method analyzeUploadedFileMethod = getAnalyzeUploadedFileMethod();
        results.addAll((List<Map<String, String>>) analyzeUploadedFileMethod.invoke(res, "zip", destFile, zipStat, workingDir, "FILE"));

        List<File> zipList = new ArrayList<>();

        int shpZipCnt = zipStat.get("shpzipcnt");
        int fgdbZipCnt = zipStat.get("fgdbzipcnt");
        int osmZipCnt = zipStat.get("osmzipcnt");
        int geonamesZipCnt = zipStat.get("osmzipcnt");

        zipList.add(destFile);
        int zipCnt = 0;
        zipCnt++;

        // shape
        input = "zip1.zip";
        srcFile = new File(homeFolder, input);
        destFile = new File(wkdirpath, input);
        FileUtils.copyFile(srcFile, destFile);
        assertTrue(destFile.exists());

        inputsList.add("zip1");

        results.addAll((List<Map<String, String>>) analyzeUploadedFileMethod.invoke(res, "zip", destFile, zipStat, workingDir, "FILE"));

        shpZipCnt += zipStat.get("shpzipcnt");
        fgdbZipCnt += zipStat.get("fgdbzipcnt");
        osmZipCnt += zipStat.get("osmzipcnt");
        int geonamesCnt = zipStat.get("osmcnt");

        zipList.add(destFile);
        zipCnt++;

        // Test zip containing fgdb + shp
        int shpCnt = 0;
        int osmCnt = 0;
        int fgdbCnt = 0;

        String inputType = "";

        FileETLCommand command = new FileETLCommand(results, zipList,
                "TDSv61.js", "ogr", Boolean.FALSE,
                "error", inputType, this.getClass());

/*
        JSONArray params = (JSONArray) command.get("params");

        int nP = 0;

        for (Object o : params) {
            JSONObject oJ = (JSONObject) o;

            if (oJ.get("INPUT") != null) {
                assertEquals("ogr.zip;zip1.zip", oJ.get("INPUT").toString());
                nP++;
            }

            if (oJ.get("INPUT_PATH") != null) {
                assertTrue(oJ.get("INPUT_PATH").toString().endsWith("/upload/test-id-123"));
                nP++;
            }

            if (oJ.get("INPUT_TYPE") != null) {
                assertEquals("ZIP", oJ.get("INPUT_TYPE").toString());
                nP++;
            }
        }

        assertEquals(3, nP);
        FileUtils.forceDelete(workingDir);
 */
    }

    @Test
    @Category(UnitTest.class)
    public void TestOgrRequest() throws Exception {
        String jobId = "test-id-123";
        String wkdirpath = UPLOAD_FOLDER + File.separator + jobId;
        File workingDir = new File(wkdirpath);
        FileUtils.forceMkdir(workingDir);
        assertTrue(workingDir.exists());

        String input = "TransportationGroundCrv.shp";
        File srcFile = new File(homeFolder, input);
        File destFile = new File(wkdirpath, input);
        FileUtils.copyFile(srcFile, destFile);
        assertTrue(destFile.exists());

        FileUploadResource res = new FileUploadResource();

        // Shape 1
        List<Map<String, String>> results = new LinkedList<>();
        Map<String, Integer> zipStat = new HashMap<>();
        List<String> inputsList = new ArrayList<>();
        inputsList.add(input);

        Method analyzeUploadedFileMethod = getAnalyzeUploadedFileMethod();
        results.addAll((List<Map<String, String>>) analyzeUploadedFileMethod.invoke(res, "shp", destFile, zipStat, workingDir, "FILE"));

        List<File> zipList = new ArrayList<>();

        int shpCnt = zipStat.get("shpcnt");
        int fgdbCnt = zipStat.get("fgdbcnt");
        int osmCnt = zipStat.get("osmcnt");
        int geonamesZipCnt = zipStat.get("osmzipcnt");

        // shape 2
        // shape
        input = "TransportationGroundCrv2.shp";
        srcFile = new File(homeFolder, "TransportationGroundCrv.shp");
        destFile = new File(wkdirpath + "/" + input);
        FileUtils.copyFile(srcFile, destFile);
        assertTrue(destFile.exists());

        inputsList.add(input);

        results.addAll((List<Map<String, String>>) analyzeUploadedFileMethod.invoke(res, "shp", destFile, zipStat, workingDir, "FILE"));

        shpCnt += zipStat.get("shpcnt");
        fgdbCnt += zipStat.get("fgdbcnt");
        osmCnt += zipStat.get("osmcnt");

        int geonamesCnt = zipStat.get("osmcnt");
        int zipCnt = 0;
        int shpZipCnt = 0;
        int osmZipCnt = 0;
        int fgdbZipCnt = 0;
        String inputType = "";

        FileETLCommand command = new FileETLCommand(results, zipList, "TDSv61.js",
                         "ogr", Boolean.FALSE, "error",
                                  inputType, this.getClass());

/*
        JSONArray params = (JSONArray) command.get("params");
        int nP = 0;

        for (Object o : params) {
            JSONObject oJ = (JSONObject) o;

            if (oJ.get("INPUT") != null) {
                assertEquals("\"TransportationGroundCrv.shp\" \"TransportationGroundCrv2.shp\" ", oJ.get("INPUT").toString());
                nP++;
            }

            if (oJ.get("INPUT_PATH") != null) {
                assertTrue(oJ.get("INPUT_PATH").toString().endsWith("/upload/test-id-123"));
                nP++;
            }

            if (oJ.get("INPUT_TYPE") != null) {
                assertEquals("OGR", oJ.get("INPUT_TYPE").toString());
                nP++;
            }
        }
        assertEquals(3, nP);
*/
        FileUtils.forceDelete(workingDir);
    }

    @Test
    @Category(UnitTest.class)
    public void testOsmRequest() throws Exception {
        String jobId = "test-id-123";
        String wkdirpath = UPLOAD_FOLDER + File.separator + jobId;
        File workingDir = new File(wkdirpath);
        FileUtils.forceMkdir(workingDir);
        assertTrue(workingDir.exists());

        String input = "osm1.osm";
        File srcFile = new File(homeFolder, input);
        File destFile = new File(wkdirpath, input);
        FileUtils.copyFile(srcFile, destFile);
        assertTrue(destFile.exists());

        FileUploadResource res = new FileUploadResource();

        // Shape 1
        JSONArray results = new JSONArray();
        Map<String, Integer> zipStat = new HashMap<>();
        List<String> inputsList = new ArrayList<>();
        inputsList.add(input);

        Method analyzeUploadedFileMethod = getAnalyzeUploadedFileMethod();
        results.addAll((List<Map<String, String>>) analyzeUploadedFileMethod.invoke(res, "osm", destFile, zipStat, workingDir, "FILE"));

        List<File> zipList = new ArrayList<>();

        int shpCnt = zipStat.get("shpcnt");
        int fgdbCnt = zipStat.get("fgdbcnt");
        int osmCnt = zipStat.get("osmcnt");
        int geonamesCnt = zipStat.get("osmcnt");

        // shape 2
        // shape
        input = "osm2.osm";
        srcFile = new File(homeFolder, input);
        destFile = new File(wkdirpath, input);
        FileUtils.copyFile(srcFile, destFile);
        assertTrue(destFile.exists());

        inputsList.add(input);

        results.addAll((List<Map<String, String>>) analyzeUploadedFileMethod.invoke(res, "osm", destFile, zipStat, workingDir, "FILE"));

        shpCnt += zipStat.get("shpcnt");
        fgdbCnt += zipStat.get("fgdbcnt");
        osmCnt += zipStat.get("osmcnt");
        geonamesCnt += zipStat.get("osmcnt");

        int zipCnt = 0;
        int shpZipCnt = 0;
        int osmZipCnt = 0;
        int fgdbZipCnt = 0;
        int geonamesZipCnt = 0;
        String inputType = "";

        FileETLCommand command = new FileETLCommand(results, zipList, "TDSv61.js",
                                            "osm", Boolean.FALSE,
                                          "error", inputType, this.getClass());

/*
        JSONArray params = (JSONArray) command.get("params");
        int nP = 0;

        for (Object o : params) {
            JSONObject oJ = (JSONObject) o;

            if (oJ.get("INPUT") != null) {
                assertEquals("\"osm1.osm\" \"osm2.osm\" ", oJ.get("INPUT").toString());
                nP++;
            }

            if (oJ.get("INPUT_PATH") != null) {
                assertTrue(oJ.get("INPUT_PATH").toString().endsWith("/upload/test-id-123"));
                nP++;
            }

            if (oJ.get("INPUT_TYPE") != null) {
                assertEquals("OSM", oJ.get("INPUT_TYPE").toString());
                nP++;
            }
        }

        assertEquals(3, nP);
*/
        FileUtils.forceDelete(workingDir);
    }

    @Test
    @Category(UnitTest.class)
    public void testFgdbRequest() throws Exception {
        String jobId = "test-id-123";
        String wkdirpath = UPLOAD_FOLDER + File.separator + jobId;
        File workingDir = new File(wkdirpath);
        FileUtils.forceMkdir(workingDir);
        assertTrue(workingDir.exists());

        String input = "DcGisRoads.gdb";
        File srcDir = new File(homeFolder, input);
        File destDir = new File(wkdirpath, input);
        FileUtils.copyDirectory(srcDir, destDir);
        assertTrue(destDir.exists());

        FileUploadResource res = new FileUploadResource();

        // Shape 1
        List<Map<String, String>> results = new LinkedList<>();
        Map<String, Integer> zipStat = new HashMap<>();
        List<String> inputsList = new ArrayList<>();
        inputsList.add(input);

        Method analyzeUploadedFileMethod = getAnalyzeUploadedFileMethod();
        results.addAll((List<Map<String, String>>) analyzeUploadedFileMethod.invoke(res, "gdb", destDir, zipStat, workingDir, "DIR"));

        List<File> zipList = new ArrayList<>();

        int shpCnt = zipStat.get("shpcnt");
        int fgdbCnt = zipStat.get("fgdbcnt");
        int osmCnt = zipStat.get("osmcnt");
        int geonamesCnt = zipStat.get("osmcnt");

        int zipCnt = 0;
        int shpZipCnt = 0;
        int osmZipCnt = 0;
        int fgdbZipCnt = 0;
        int geonamesZipCnt = 0;

        String inputType = "";

        FileETLCommand command = new FileETLCommand(results, zipList, "TDSv61.js", "fgdb",
                Boolean.FALSE, "error", inputType, this.getClass());

/*
        JSONArray params = (JSONArray) command.get("params");
        int nP = 0;

        for (Object o : params) {
            JSONObject oJ = (JSONObject) o;

            if (oJ.get("INPUT") != null) {
                assertEquals("\"DcGisRoads.gdb\" ", oJ.get("INPUT").toString());
                nP++;
            }

            if (oJ.get("INPUT_PATH") != null) {
                assertTrue(oJ.get("INPUT_PATH").toString().endsWith("/upload/test-id-123"));
                nP++;
            }

            if (oJ.get("INPUT_TYPE") != null) {
                assertEquals("FGDB", oJ.get("INPUT_TYPE").toString());
                nP++;
            }
        }

        assertEquals(3, nP);
*/
        FileUtils.forceDelete(workingDir);
    }

    @Test
    @Category(UnitTest.class)
    public void TestserializeFGDB() throws Exception {
        new FileUploadResource();

        // homeFolder + "/upload/" + jobId + "/" + relPath;
        // Create dummy FGDB

        String jobId = "123-456-789";
        String wkdirpath = UPLOAD_FOLDER + File.separator + jobId;
        File workingDir = new File(wkdirpath);
        FileUtils.forceMkdir(workingDir);
        assertTrue(workingDir.exists());

        List<FileItem> fileItemsList = new ArrayList<>();

        FileItemFactory factory = new DiskFileItemFactory(16, null);
        String textFieldName = "textField";

        FileItem item = factory.createItem(textFieldName, "application/octet-stream", true, "fgdbTest.gdb/dummy1.gdbtable");

        String textFieldValue = "0123456789";
        byte[] testFieldValueBytes = textFieldValue.getBytes();

        try (OutputStream os = item.getOutputStream()) {
            os.write(testFieldValueBytes);
            os.close();
        }

        File out = new File(wkdirpath + "/buffer.tmp");
        item.write(out);
        fileItemsList.add(item);
        assertTrue(out.exists());
    }

    @Test
    @Category(UnitTest.class)
    public void TestserializeUploadedFiles() throws Exception {
        new FileUploadResource();

        // homeFolder + "/upload/" + jobId + "/" + relPath;
        // Create dummy FGDB

        String jobId = "123-456-789-testosm";
        String wkdirpath = UPLOAD_FOLDER + File.separator + jobId;
        File workingDir = new File(wkdirpath);
        FileUtils.forceMkdir(workingDir);
        assertTrue(workingDir.exists());

        List<FileItem> fileItemsList = new ArrayList<>();

        FileItemFactory factory = new DiskFileItemFactory(16, null);
        String textFieldName = "textField";

        FileItem item = factory.createItem(textFieldName, "application/octet-stream", true, "dummy1.osm");

        String textFieldValue = "0123456789";
        byte[] testFieldValueBytes = textFieldValue.getBytes();

        try (OutputStream os = item.getOutputStream()) {
            os.write(testFieldValueBytes);
            os.close();
        }

        File out = new File(wkdirpath + "/buffer.tmp");
        item.write(out);
        fileItemsList.add(item);
        assertTrue(out.exists());
    }

    private static Method getAnalyzeUploadedFileMethod() throws NoSuchMethodException {
        Method analyzeUploadedFileMethod = FileUploadResource.class
                .getDeclaredMethod("handleUploadedFile", String.class, File.class, Map.class, File.class, String.class, String.class);
        analyzeUploadedFileMethod.setAccessible(true);
        return analyzeUploadedFileMethod;
    }
}
