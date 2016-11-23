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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.ingest;

import static hoot.services.HootProperties.HOME_FOLDER;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import java.io.File;
import java.io.IOException;
import java.io.OutputStream;
import java.lang.reflect.Method;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;

import org.apache.commons.fileupload.FileItem;
import org.apache.commons.fileupload.FileItemFactory;
import org.apache.commons.fileupload.disk.DiskFileItemFactory;
import org.apache.commons.io.FileUtils;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Ignore;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import hoot.services.UnitTest;
import hoot.services.testsupport.HootCustomPropertiesSetter;


public class FileUploadResourceTest {
    private static File homeFolder;
    private static String original_HOME_FOLDER;


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
            homeFolder = new File(FileUtils.getTempDirectory(), "FileUploadResourceTest");
            FileUtils.forceMkdir(homeFolder);
            assertTrue(homeFolder.exists());
            HootCustomPropertiesSetter.setProperty("HOME_FOLDER", homeFolder.getAbsolutePath());

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
    }

    @Test
    @Category(UnitTest.class)
    public void TestBuildNativeRequestFgdbOgr() throws Exception {
        String jobId = "test-id-123";
        String wkdirpath = homeFolder + "/upload/" + jobId;
        File workingDir = new File(wkdirpath);
        FileUtils.forceMkdir(workingDir);
        assertTrue(workingDir.exists());

        File srcFile = new File(homeFolder, "fgdb_ogr.zip");
        File destFile = new File(wkdirpath, "fgdb_ogr.zip");
        FileUtils.copyFile(srcFile, destFile);
        assertTrue(destFile.exists());

        FileUploadResource res = new FileUploadResource();

        // Let's test zip
        JSONArray results = new JSONArray();
        JSONObject zipStat = new JSONObject();

        Method buildNativeRequestMethod = getBuildNativeRequestMethod();
        buildNativeRequestMethod.invoke(res, jobId, "fgdb_ogr", "zip", "fgdb_ogr.zip", results, zipStat);

        assertEquals(2, results.size());

        for (Object oRes : results) {
            JSONObject cnt = (JSONObject) oRes;
            if (cnt.get("type").toString().equals("FGDB_ZIP")) {
                assertEquals("fgdb_ogr/DcGisRoads.gdb", cnt.get("name").toString());
            }
            else if (cnt.get("type").toString().equals("OGR_ZIP")) {
                assertEquals("fgdb_ogr/jakarta_raya_coastline.shp", cnt.get("name").toString());
            }
        }
        FileUtils.forceDelete(workingDir);
    }

    @Test(expected = Exception.class)
    @Category(UnitTest.class)
    public void TestBuildNativeRequestFgdbOsm() throws Exception {
        String input = "fgdb_osm.zip";
        String jobId = "test-id-123";
        String wkdirpath = homeFolder + "/upload/" + jobId;
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
        JSONObject zipStat = new JSONObject();

        try {
            Method buildNativeRequestMethod = getBuildNativeRequestMethod();
            buildNativeRequestMethod.invoke(res, jobId, "fgdb_osm", "zip", input, results, zipStat);
        }
        catch (Exception ex) {
            assertEquals("Zip should not contain both osm and ogr types.", ex.getMessage());
            throw ex;
        }

        FileUtils.forceDelete(workingDir);
    }

    @Test
    @Category(UnitTest.class)
    public void TestCreateNativeRequestFgdbOgrZip() throws Exception {
        String input = "fgdb_ogr.zip";
        String jobId = "test-id-123";
        String wkdirpath = homeFolder + "/upload/" + jobId;
        File workingDir = new File(wkdirpath);
        FileUtils.forceMkdir(workingDir);
        assertTrue(workingDir.exists());

        File srcFile = new File(homeFolder, input);
        File destFile = new File(wkdirpath, input);
        FileUtils.copyFile(srcFile, destFile);
        assertTrue(destFile.exists());

        FileUploadResource res = new FileUploadResource();

        // Let's test zip
        JSONArray results = new JSONArray();
        JSONObject zipStat = new JSONObject();
        List<String> inputsList = new ArrayList<>();
        inputsList.add(input);

        Method buildNativeRequestMethod = getBuildNativeRequestMethod();
        buildNativeRequestMethod.invoke(res, jobId, "fgdb_ogr", "zip", input, results, zipStat);

        int shpZipCnt = 0;
        List<String> zipList = new ArrayList<>();

        shpZipCnt += (Integer) zipStat.get("shpzipcnt");

        int fgdbZipCnt = 0;
        fgdbZipCnt += (Integer) zipStat.get("fgdbzipcnt");

        int osmZipCnt = 0;
        osmZipCnt += (Integer) zipStat.get("osmzipcnt");

        int geonamesZipCnt = 0;
        geonamesZipCnt += (Integer) zipStat.get("geonameszipcnt");

        zipList.add("fgdb_ogr");
        int zipCnt = 0;
        zipCnt++;

        Method createNativeRequestMethod = getCreateNativeRequestMethod();

        // Test zip containing fgdb + shp
        int shpCnt = 0;
        int osmCnt = 0;
        int geonamesCnt = 0;
        int fgdbCnt = 0;
        JSONArray resA = (JSONArray) createNativeRequestMethod.invoke(res, results, zipCnt, shpZipCnt, fgdbZipCnt,
                osmZipCnt, geonamesZipCnt, shpCnt, fgdbCnt, osmCnt, geonamesCnt, zipList, "TDSv61.js", jobId, "fgdb_ogr",
                inputsList, "test@test.com", "false", null);

        JSONObject req = (JSONObject) resA.get(0);
        JSONArray params = (JSONArray) req.get("params");

        int nP = 0;

        for (Object o : params) {
            JSONObject oJ = (JSONObject) o;

            if (oJ.get("INPUT") != null) {
                assertEquals("\"fgdb_ogr/DcGisRoads.gdb\" \"fgdb_ogr/jakarta_raya_coastline.shp\" ", oJ.get("INPUT").toString());
                nP++;
            }

            if (oJ.get("INPUT_PATH") != null) {
                assertEquals("upload/test-id-123", oJ.get("INPUT_PATH").toString());
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
        FileUtils.forceDelete(workingDir);
    }

    @Test
    @Category(UnitTest.class)
    public void TestCreateNativeRequestOgrZipAndShp() throws Exception {
        String jobId = "test-id-123";
        String wkdirpath = homeFolder + "/upload/" + jobId;
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
        JSONArray results = new JSONArray();
        JSONObject zipStat = new JSONObject();
        List<String> inputsList = new ArrayList<>();
        inputsList.add(input);

        Method buildNativeRequestMethod = getBuildNativeRequestMethod();

        buildNativeRequestMethod.invoke(res, jobId, "fgdb_ogr", "zip", input, results, zipStat);

        int shpZipCnt = 0;
        List<String> zipList = new ArrayList<>();

        shpZipCnt += (Integer) zipStat.get("shpzipcnt");

        int fgdbZipCnt = 0;
        fgdbZipCnt += (Integer) zipStat.get("fgdbzipcnt");

        int osmZipCnt = 0;
        osmZipCnt += (Integer) zipStat.get("osmzipcnt");

        int geonamesZipCnt = 0;
        geonamesZipCnt += (Integer) zipStat.get("osmzipcnt");

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

        buildNativeRequestMethod.invoke(res, jobId, "TransportationGroundCrv", "shp", input, results, zipStat);

        int shpCnt = 0;
        shpCnt += (Integer) zipStat.get("shpcnt");

        int fgdbCnt = 0;
        fgdbCnt += (Integer) zipStat.get("fgdbcnt");

        int osmCnt = 0;
        osmCnt += (Integer) zipStat.get("osmcnt");

        int geonamesCnt = 0;
        geonamesCnt += (Integer) zipStat.get("geonamescnt");

        Method createNativeRequestMethod = getCreateNativeRequestMethod();

        // Test zip containing fgdb + shp
        JSONArray resA = (JSONArray) createNativeRequestMethod.invoke(res, results, zipCnt, shpZipCnt, fgdbZipCnt, osmZipCnt,
                geonamesZipCnt, shpCnt, fgdbCnt, osmCnt, geonamesCnt, zipList, "TDSv61.js", jobId, "fgdb_ogr", inputsList,
                "test@test.com", "false", null);

        JSONObject req = (JSONObject) resA.get(0);
        JSONArray params = (JSONArray) req.get("params");

        int nP = 0;

        for (Object o : params) {
            JSONObject oJ = (JSONObject) o;

            if (oJ.get("INPUT") != null) {
                assertEquals("\"fgdb_ogr/DcGisRoads.gdb\" \"fgdb_ogr/jakarta_raya_coastline.shp\" \"TransportationGroundCrv.shp\" ", oJ.get("INPUT").toString());
                nP++;
            }

            if (oJ.get("INPUT_PATH") != null) {
                assertEquals("upload/test-id-123", oJ.get("INPUT_PATH").toString());
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
        FileUtils.forceDelete(workingDir);
    }

    @Test
    @Category(UnitTest.class)
    public void TestCreateNativeRequestOsmZipAndOsm() throws Exception {
        String jobId = "test-id-123";
        String wkdirpath = homeFolder + "/upload/" + jobId;
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
        JSONArray results = new JSONArray();
        JSONObject zipStat = new JSONObject();
        List<String> inputsList = new ArrayList<>();
        inputsList.add(input);

        Method buildNativeRequestMethod = getBuildNativeRequestMethod();
        buildNativeRequestMethod.invoke(res, jobId, "osm", "zip", input, results, zipStat);

        int shpZipCnt = 0;
        List<String> zipList = new ArrayList<>();

        shpZipCnt += (Integer) zipStat.get("shpzipcnt");

        int fgdbZipCnt = 0;
        fgdbZipCnt += (Integer) zipStat.get("fgdbzipcnt");

        int osmZipCnt = 0;
        osmZipCnt += (Integer) zipStat.get("osmzipcnt");

        int geonamesZipCnt = 0;
        geonamesZipCnt += (Integer) zipStat.get("osmzipcnt");

        zipList.add("osm");
        int zipCnt = 0;
        zipCnt++;

        // osm
        input = "osm1.osm";
        srcFile = new File(homeFolder, input);
        destFile = new File(wkdirpath, input);
        FileUtils.copyFile(srcFile, destFile);
        assertTrue(destFile.exists());

        inputsList.add(input);

        buildNativeRequestMethod.invoke(res, jobId, "osm1", "osm", input, results, zipStat);

        int shpCnt = 0;
        shpCnt += (Integer) zipStat.get("shpcnt");

        int fgdbCnt = 0;
        fgdbCnt += (Integer) zipStat.get("fgdbcnt");

        int osmCnt = 0;
        osmCnt += (Integer) zipStat.get("osmcnt");

        int geonamesCnt = 0;
        geonamesCnt += (Integer) zipStat.get("osmcnt");

        Method createNativeRequestMethod = getCreateNativeRequestMethod();

        // Test zip containing fgdb + shp
        JSONArray resA = (JSONArray) createNativeRequestMethod.invoke(res, results, zipCnt, shpZipCnt, fgdbZipCnt, osmZipCnt,
                geonamesZipCnt, shpCnt, fgdbCnt, osmCnt, geonamesCnt, zipList, "TDSv61.js", jobId, "osm", inputsList,
                "test@test.com", "false", null);

        JSONObject req = (JSONObject) resA.get(0);
        JSONArray params = (JSONArray) req.get("params");

        int nP = 0;

        for (Object o : params) {
            JSONObject oJ = (JSONObject) o;

            if (oJ.get("INPUT") != null) {
                assertEquals("\"osm/DcGisRoads.osm\" \"osm/DcTigerRoads.osm\" \"osm1.osm\" ", oJ.get("INPUT").toString());
                nP++;
            }

            if (oJ.get("INPUT_PATH") != null) {
                assertEquals("upload/test-id-123", oJ.get("INPUT_PATH").toString());
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
        FileUtils.forceDelete(workingDir);
    }

    @Test
    @Category(UnitTest.class)
    public void TestCreateNativeRequestOgrZip() throws Exception {
        String jobId = "test-id-123";
        String wkdirpath = homeFolder + "/upload/" + jobId;
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
        JSONArray results = new JSONArray();
        JSONObject zipStat = new JSONObject();
        List<String> inputsList = new ArrayList<>();
        inputsList.add(input);

        Method buildNativeRequestMethod = getBuildNativeRequestMethod();
        buildNativeRequestMethod.invoke(res, jobId, "ogr", "zip", input, results, zipStat);

        int shpZipCnt = 0;
        List<String> zipList = new ArrayList<>();

        shpZipCnt += (Integer) zipStat.get("shpzipcnt");

        int fgdbZipCnt = 0;
        fgdbZipCnt += (Integer) zipStat.get("fgdbzipcnt");

        int osmZipCnt = 0;
        osmZipCnt += (Integer) zipStat.get("osmzipcnt");

        int geonamesZipCnt = 0;
        geonamesZipCnt += (Integer) zipStat.get("osmzipcnt");

        zipList.add("ogr");
        int zipCnt = 0;
        zipCnt++;

        // shape
        input = "zip1.zip";
        srcFile = new File(homeFolder, input);
        destFile = new File(wkdirpath, input);
        FileUtils.copyFile(srcFile, destFile);
        assertTrue(destFile.exists());

        inputsList.add("zip1");

        buildNativeRequestMethod.invoke(res, jobId, "zip1", "zip", input, results, zipStat);

        shpZipCnt += (Integer) zipStat.get("shpzipcnt");
        fgdbZipCnt += (Integer) zipStat.get("fgdbzipcnt");
        osmZipCnt += (Integer) zipStat.get("osmzipcnt");
        int geonamesCnt = 0;
        geonamesCnt += (Integer) zipStat.get("osmcnt");

        zipList.add("zip1");
        zipCnt++;

        Method createNativeRequestMethod = getCreateNativeRequestMethod();

        // Test zip containing fgdb + shp
        int shpCnt = 0;
        int osmCnt = 0;
        int fgdbCnt = 0;
        JSONArray resA = (JSONArray) createNativeRequestMethod.invoke(res, results, zipCnt, shpZipCnt, fgdbZipCnt, osmZipCnt,
                geonamesZipCnt, shpCnt, fgdbCnt, osmCnt, geonamesCnt, zipList, "TDSv61.js", jobId, "ogr", inputsList,
                "test@test.com", "false", null);

        JSONObject req = (JSONObject) resA.get(0);
        JSONArray params = (JSONArray) req.get("params");

        int nP = 0;

        for (Object o : params) {
            JSONObject oJ = (JSONObject) o;

            if (oJ.get("INPUT") != null) {
                assertEquals("ogr.zip;zip1.zip", oJ.get("INPUT").toString());
                nP++;
            }

            if (oJ.get("INPUT_PATH") != null) {
                assertEquals("upload/test-id-123", oJ.get("INPUT_PATH").toString());
                nP++;
            }

            if (oJ.get("INPUT_TYPE") != null) {
                assertEquals("ZIP", oJ.get("INPUT_TYPE").toString());
                nP++;
            }
        }

        assertEquals(3, nP);
        FileUtils.forceDelete(workingDir);
    }

    @Test
    @Category(UnitTest.class)
    public void TestCreateNativeRequestOgr() throws Exception {
        String jobId = "test-id-123";
        String wkdirpath = homeFolder + "/upload/" + jobId;
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
        JSONArray results = new JSONArray();
        JSONObject zipStat = new JSONObject();
        List<String> inputsList = new ArrayList<>();
        inputsList.add(input);

        Method buildNativeRequestMethod = getBuildNativeRequestMethod();
        buildNativeRequestMethod.invoke(res, jobId, "TransportationGroundCrv", "shp", input, results, zipStat);

        int shpCnt = 0;

        List<String> zipList = new ArrayList<>();

        shpCnt += (Integer) zipStat.get("shpcnt");

        int fgdbCnt = 0;
        fgdbCnt += (Integer) zipStat.get("fgdbcnt");

        int osmCnt = 0;
        osmCnt += (Integer) zipStat.get("osmcnt");

        int geonamesZipCnt = 0;
        geonamesZipCnt += (Integer) zipStat.get("osmzipcnt");

        // shape 2
        // shape
        input = "TransportationGroundCrv2.shp";
        srcFile = new File(homeFolder, "TransportationGroundCrv.shp");
        destFile = new File(wkdirpath + "/" + input);
        FileUtils.copyFile(srcFile, destFile);
        assertTrue(destFile.exists());

        inputsList.add(input);

        buildNativeRequestMethod.invoke(res, jobId, "TransportationGroundCrv2", "shp", input, results, zipStat);

        shpCnt += (Integer) zipStat.get("shpcnt");
        fgdbCnt += (Integer) zipStat.get("fgdbcnt");
        osmCnt += (Integer) zipStat.get("osmcnt");
        int geonamesCnt = 0;
        geonamesCnt += (Integer) zipStat.get("osmcnt");

        Method createNativeRequestMethod = getCreateNativeRequestMethod();

        int zipCnt = 0;
        int shpZipCnt = 0;
        int osmZipCnt = 0;
        int fgdbZipCnt = 0;
        JSONArray resA = (JSONArray) createNativeRequestMethod.invoke(res, results, zipCnt, shpZipCnt, fgdbZipCnt, osmZipCnt,
                geonamesZipCnt, shpCnt, fgdbCnt, osmCnt, geonamesCnt, zipList, "TDSv61.js", jobId, "ogr", inputsList,
                "test@test.com", "false", null);

        JSONObject req = (JSONObject) resA.get(0);
        JSONArray params = (JSONArray) req.get("params");

        int nP = 0;

        for (Object o : params) {
            JSONObject oJ = (JSONObject) o;

            if (oJ.get("INPUT") != null) {
                assertEquals("\"TransportationGroundCrv.shp\" \"TransportationGroundCrv2.shp\" ", oJ.get("INPUT").toString());
                nP++;
            }

            if (oJ.get("INPUT_PATH") != null) {
                assertEquals("upload/test-id-123", oJ.get("INPUT_PATH").toString());
                nP++;
            }

            if (oJ.get("INPUT_TYPE") != null) {
                assertEquals("OGR", oJ.get("INPUT_TYPE").toString());
                nP++;
            }
        }
        assertEquals(3, nP);
        FileUtils.forceDelete(workingDir);
    }

    @Test
    @Category(UnitTest.class)
    public void TestCreateNativeRequestOsm() throws Exception {
        String jobId = "test-id-123";
        String wkdirpath = homeFolder + "/upload/" + jobId;
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
        JSONObject zipStat = new JSONObject();
        List<String> inputsList = new ArrayList<>();
        inputsList.add(input);

        Method buildNativeRequestMethod = getBuildNativeRequestMethod();
        buildNativeRequestMethod.invoke(res, jobId, "osm1", "osm", input, results, zipStat);

        int shpCnt = 0;

        List<String> zipList = new ArrayList<>();

        shpCnt += (Integer) zipStat.get("shpcnt");

        int fgdbCnt = 0;
        fgdbCnt += (Integer) zipStat.get("fgdbcnt");

        int osmCnt = 0;
        osmCnt += (Integer) zipStat.get("osmcnt");

        int geonamesCnt = 0;
        geonamesCnt += (Integer) zipStat.get("osmcnt");

        // shape 2
        // shape
        input = "osm2.osm";
        srcFile = new File(homeFolder, input);
        destFile = new File(wkdirpath, input);
        FileUtils.copyFile(srcFile, destFile);
        assertTrue(destFile.exists());

        inputsList.add(input);

        buildNativeRequestMethod.invoke(res, jobId, "osm2", "osm", input, results, zipStat);

        shpCnt += (Integer) zipStat.get("shpcnt");
        fgdbCnt += (Integer) zipStat.get("fgdbcnt");
        osmCnt += (Integer) zipStat.get("osmcnt");
        geonamesCnt += (Integer) zipStat.get("osmcnt");

        Method createNativeRequestMethod = getCreateNativeRequestMethod();

        int zipCnt = 0;
        int shpZipCnt = 0;
        int osmZipCnt = 0;
        int fgdbZipCnt = 0;
        int geonamesZipCnt = 0;
        JSONArray resA = (JSONArray) createNativeRequestMethod.invoke(res, results, zipCnt, shpZipCnt, fgdbZipCnt, osmZipCnt,
                geonamesZipCnt, shpCnt, fgdbCnt, osmCnt, geonamesCnt, zipList, "TDSv61.js", jobId, "osm", inputsList,
                "test@test.com", "false", null);

        JSONObject req = (JSONObject) resA.get(0);
        JSONArray params = (JSONArray) req.get("params");

        int nP = 0;

        for (Object o : params) {
            JSONObject oJ = (JSONObject) o;

            if (oJ.get("INPUT") != null) {
                assertEquals("\"osm1.osm\" \"osm2.osm\" ", oJ.get("INPUT").toString());
                nP++;
            }

            if (oJ.get("INPUT_PATH") != null) {
                assertEquals("upload/test-id-123", oJ.get("INPUT_PATH").toString());
                nP++;
            }

            if (oJ.get("INPUT_TYPE") != null) {
                assertEquals("OSM", oJ.get("INPUT_TYPE").toString());
                nP++;
            }
        }

        assertEquals(3, nP);
        FileUtils.forceDelete(workingDir);
    }

    @Ignore
    @Test
    @Category(UnitTest.class)
    public void TestCreateNativeRequestFgdb() throws Exception {
        String jobId = "test-id-123";
        String wkdirpath = homeFolder + "/upload/" + jobId;
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
        JSONArray results = new JSONArray();
        JSONObject zipStat = new JSONObject();
        List<String> inputsList = new ArrayList<>();
        inputsList.add(input);

        Method buildNativeRequestMethod = getBuildNativeRequestMethod();
        buildNativeRequestMethod.invoke(res, jobId, "DcGisRoads", "gdb", input, results, zipStat);

        int shpCnt = 0;

        List<String> zipList = new ArrayList<>();

        shpCnt += (Integer) zipStat.get("shpcnt");

        int fgdbCnt = 0;
        fgdbCnt += (Integer) zipStat.get("fgdbcnt");

        int osmCnt = 0;
        osmCnt += (Integer) zipStat.get("osmcnt");

        int geonamesCnt = 0;
        geonamesCnt += (Integer) zipStat.get("osmcnt");

        Method createNativeRequestMethod = getCreateNativeRequestMethod();

        int zipCnt = 0;
        int shpZipCnt = 0;
        int osmZipCnt = 0;
        int fgdbZipCnt = 0;
        int geonamesZipCnt = 0;
        JSONArray resA = (JSONArray) createNativeRequestMethod.invoke(res, results, zipCnt, shpZipCnt, fgdbZipCnt, osmZipCnt, geonamesZipCnt,
                shpCnt, fgdbCnt, osmCnt, geonamesCnt, zipList, "TDSv61.js", jobId, "fgdb", inputsList, "test@test.com",
                "false", null);

        JSONObject req = (JSONObject) resA.get(0);
        JSONArray params = (JSONArray) req.get("params");

        int nP = 0;

        for (Object o : params) {
            JSONObject oJ = (JSONObject) o;

            if (oJ.get("INPUT") != null) {
                assertEquals("\"DcGisRoads.gdb\" ", oJ.get("INPUT").toString());
                nP++;
            }

            if (oJ.get("INPUT_PATH") != null) {
                assertEquals("upload/test-id-123", oJ.get("INPUT_PATH").toString());
                nP++;
            }

            if (oJ.get("INPUT_TYPE") != null) {
                assertEquals("FGDB", oJ.get("INPUT_TYPE").toString());
                nP++;
            }
        }

        assertEquals(3, nP);
        FileUtils.forceDelete(workingDir);
    }

    @Test
    @Category(UnitTest.class)
    public void TestserializeFGDB() throws Exception {
        new FileUploadResource();

        // homeFolder + "/upload/" + jobId + "/" + relPath;
        // Create dummy FGDB

        String jobId = "123-456-789";
        String wkdirpath = homeFolder + "/upload/" + jobId;
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

        /*
         * Map<String,String> uploadedFiles = new HashMap<String, String>();
         * Map<String,String> uploadedFilesPaths = new HashMap<String,
         * String>();
         * 
         * res.serializeFGDB(fileItemsList, jobId, uploadedFiles,
         * uploadedFilesPaths );
         * 
         * org.junit.Assert.assertEquals("GDB", uploadedFiles.get("fgdbTest"));
         * org.junit.Assert.assertEquals("fgdbTest.gdb",
         * uploadedFilesPaths.get("fgdbTest"));
         * 
         * File fgdbpath = new File(wkdirpath + "/fgdbTest.gdb");
         * org.junit.Assert.assertTrue(fgdbpath.exists());
         * 
         * File content = new File(wkdirpath + "/fgdbTest.gdb/dummy1.gdbtable");
         * org.junit.Assert.assertTrue(content.exists());
         * 
         * FileUtils.forceDelete(workingDir);
         */
    }

    @Test
    @Category(UnitTest.class)
    public void TestserializeUploadedFiles() throws Exception {
        new FileUploadResource();

        // homeFolder + "/upload/" + jobId + "/" + relPath;
        // Create dummy FGDB

        String jobId = "123-456-789-testosm";
        String wkdirpath = homeFolder + "/upload/" + jobId;
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

        /*
         * Map<String,String> uploadedFiles = new HashMap<String, String>();
         * Map<String,String> uploadedFilesPaths = new HashMap<String,
         * String>();
         * 
         * res.serializeUploadedFiles(fileItemsList, jobId, uploadedFiles,
         * uploadedFilesPaths, wkdirpath);
         * 
         * org.junit.Assert.assertEquals("OSM", uploadedFiles.get("dummy1"));
         * org.junit.Assert.assertEquals("dummy1.osm",
         * uploadedFilesPaths.get("dummy1"));
         * 
         * 
         * 
         * File content = new File(wkdirpath + "/dummy1.osm");
         * org.junit.Assert.assertTrue(content.exists());
         * 
         * FileUtils.forceDelete(workingDir);
         */
    }

    private static Method getBuildNativeRequestMethod() throws NoSuchMethodException {
        Method buildNativeRequestMethod = FileUploadResource.class.getDeclaredMethod("buildNativeRequest", String.class,
                String.class, String.class, String.class, JSONArray.class, JSONObject.class);

        buildNativeRequestMethod.setAccessible(true);

        return buildNativeRequestMethod;
    }

    private static Method getCreateNativeRequestMethod() throws NoSuchMethodException {
        Method createNativeRequestMethod = FileUploadResource.class.getDeclaredMethod("createNativeRequest",
                JSONArray.class, int.class, int.class, int.class, int.class, int.class, int.class, int.class,
                int.class, int.class, List.class, String.class, String.class, String.class, List.class, String.class,
                String.class, String.class);

        createNativeRequestMethod.setAccessible(true);

        return createNativeRequestMethod;
    }
}
