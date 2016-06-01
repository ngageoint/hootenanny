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

import java.io.File;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;

import org.apache.commons.fileupload.FileItem;
import org.apache.commons.fileupload.FileItemFactory;
import org.apache.commons.fileupload.disk.DiskFileItemFactory;
import org.apache.commons.io.FileUtils;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import hoot.services.HootProperties;
import hoot.services.UnitTest;


public class FileUploadResourceTest {
    private static String homeFolder = null;

    @BeforeClass
    public static void oneTimeSetup() throws Exception {
        String processScriptName = HootProperties.getProperty("ETLMakefile");

        Assert.assertNotNull(processScriptName);
        Assert.assertFalse(processScriptName.isEmpty());

        homeFolder = HootProperties.getProperty("homeFolder");
        Assert.assertNotNull(homeFolder);
        Assert.assertFalse(homeFolder.isEmpty());
    }

    @Test
    @Category(UnitTest.class)
    public void TestBuildNativeRequestFgdbOgr() throws Exception {
        String jobId = "test-id-123";
        String wkdirpath = homeFolder + "/upload/" + jobId;
        File workingDir = new File(wkdirpath);
        FileUtils.forceMkdir(workingDir);
        Assert.assertTrue(workingDir.exists());

        File srcFile = new File(homeFolder + "/test-files/service/FileUploadResourceTest/fgdb_ogr.zip");
        File destFile = new File(wkdirpath + "/fgdb_ogr.zip");
        FileUtils.copyFile(srcFile, destFile);
        Assert.assertTrue(destFile.exists());

        FileUploadResource res = new FileUploadResource();

        // Let's test zip
        JSONArray results = new JSONArray();
        JSONObject zipStat = new JSONObject();
        res._buildNativeRequest(jobId, "fgdb_ogr", "zip", "fgdb_ogr.zip", results, zipStat);

        Assert.assertEquals(2, results.size());

        for (Object oRes : results) {
            JSONObject cnt = (JSONObject) oRes;
            if (cnt.get("type").toString().equals("FGDB_ZIP")) {
                Assert.assertEquals("fgdb_ogr/DcGisRoads.gdb", cnt.get("name").toString());
            }
            else if (cnt.get("type").toString().equals("OGR_ZIP")) {
                Assert.assertEquals("fgdb_ogr/jakarta_raya_coastline.shp", cnt.get("name").toString());
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
        Assert.assertTrue(workingDir.exists());

        File srcFile = new File(homeFolder + "/test-files/service/FileUploadResourceTest/" + input);
        File destFile = new File(wkdirpath + "/" + input);
        FileUtils.copyFile(srcFile, destFile);
        Assert.assertTrue(destFile.exists());

        FileUploadResource res = new FileUploadResource();

        // Let's test zip
        JSONArray results = new JSONArray();
        JSONObject zipStat = new JSONObject();

        try {
            res._buildNativeRequest(jobId, "fgdb_osm", "zip", input, results, zipStat);
        }
        catch (Exception ex) {
            Assert.assertEquals("Zip should not contain both osm and ogr types.", ex.getMessage());
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
        Assert.assertTrue(workingDir.exists());

        File srcFile = new File(homeFolder + "/test-files/service/FileUploadResourceTest/" + input);
        File destFile = new File(wkdirpath + "/" + input);
        FileUtils.copyFile(srcFile, destFile);
        Assert.assertTrue(destFile.exists());

        FileUploadResource res = new FileUploadResource();

        // Let's test zip
        JSONArray results = new JSONArray();
        JSONObject zipStat = new JSONObject();
        List<String> inputsList = new ArrayList<>();
        inputsList.add(input);

        res._buildNativeRequest(jobId, "fgdb_ogr", "zip", input, results, zipStat);

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

        // Test zip containing fgdb + shp
        int shpCnt = 0;
        int osmCnt = 0;
        int geonamesCnt = 0;
        int fgdbCnt = 0;
        JSONArray resA = res._createNativeRequest(results, zipCnt, shpZipCnt, fgdbZipCnt, osmZipCnt, geonamesZipCnt,
                shpCnt, fgdbCnt, osmCnt, geonamesCnt, zipList, "TDSv61.js", jobId, "fgdb_ogr", inputsList,
                "test@test.com", "false", null);

        JSONObject req = (JSONObject) resA.get(0);
        JSONArray params = (JSONArray) req.get("params");

        int nP = 0;

        for (Object o : params) {
            JSONObject oJ = (JSONObject) o;

            if (oJ.get("INPUT") != null) {
                Assert.assertEquals("\"fgdb_ogr/DcGisRoads.gdb\" \"fgdb_ogr/jakarta_raya_coastline.shp\" ", oJ.get("INPUT").toString());
                nP++;
            }

            if (oJ.get("INPUT_PATH") != null) {
                Assert.assertEquals("upload/test-id-123", oJ.get("INPUT_PATH").toString());
                nP++;
            }

            if (oJ.get("INPUT_TYPE") != null) {
                Assert.assertEquals("OGR", oJ.get("INPUT_TYPE").toString());
                nP++;
            }

            if (oJ.get("UNZIP_LIST") != null) {
                Assert.assertEquals("fgdb_ogr", oJ.get("UNZIP_LIST").toString());
                nP++;
            }
        }

        Assert.assertEquals(4, nP);
        FileUtils.forceDelete(workingDir);
    }

    @Test
    @Category(UnitTest.class)
    public void TestCreateNativeRequestOgrZipAndShp() throws Exception {
        String jobId = "test-id-123";
        String wkdirpath = homeFolder + "/upload/" + jobId;
        File workingDir = new File(wkdirpath);
        FileUtils.forceMkdir(workingDir);
        Assert.assertTrue(workingDir.exists());

        String input = "fgdb_ogr.zip";
        File srcFile = new File(homeFolder + "/test-files/service/FileUploadResourceTest/" + input);
        File destFile = new File(wkdirpath + "/" + input);
        FileUtils.copyFile(srcFile, destFile);
        Assert.assertTrue(destFile.exists());

        FileUploadResource res = new FileUploadResource();

        // Let's test zip
        JSONArray results = new JSONArray();
        JSONObject zipStat = new JSONObject();
        List<String> inputsList = new ArrayList<>();
        inputsList.add(input);

        res._buildNativeRequest(jobId, "fgdb_ogr", "zip", input, results, zipStat);

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
        srcFile = new File(homeFolder + "/test-files/service/FileUploadResourceTest/" + input);
        destFile = new File(wkdirpath + "/" + input);
        FileUtils.copyFile(srcFile, destFile);
        Assert.assertTrue(destFile.exists());

        inputsList.add(input);

        res._buildNativeRequest(jobId, "TransportationGroundCrv", "shp", input, results, zipStat);

        int shpCnt = 0;
        shpCnt += (Integer) zipStat.get("shpcnt");

        int fgdbCnt = 0;
        fgdbCnt += (Integer) zipStat.get("fgdbcnt");

        int osmCnt = 0;
        osmCnt += (Integer) zipStat.get("osmcnt");

        int geonamesCnt = 0;
        geonamesCnt += (Integer) zipStat.get("geonamescnt");

        // Test zip containing fgdb + shp
        JSONArray resA = res._createNativeRequest(results, zipCnt, shpZipCnt, fgdbZipCnt, osmZipCnt, geonamesZipCnt,
                shpCnt, fgdbCnt, osmCnt, geonamesCnt, zipList, "TDSv61.js", jobId, "fgdb_ogr", inputsList,
                "test@test.com", "false", null);

        JSONObject req = (JSONObject) resA.get(0);
        JSONArray params = (JSONArray) req.get("params");

        int nP = 0;

        for (Object o : params) {
            JSONObject oJ = (JSONObject) o;

            if (oJ.get("INPUT") != null) {
                Assert.assertEquals("\"fgdb_ogr/DcGisRoads.gdb\" \"fgdb_ogr/jakarta_raya_coastline.shp\" \"TransportationGroundCrv.shp\" ", oJ.get("INPUT").toString());
                nP++;
            }

            if (oJ.get("INPUT_PATH") != null) {
                Assert.assertEquals("upload/test-id-123", oJ.get("INPUT_PATH").toString());
                nP++;
            }

            if (oJ.get("INPUT_TYPE") != null) {
                Assert.assertEquals("OGR", oJ.get("INPUT_TYPE").toString());
                nP++;
            }

            if (oJ.get("UNZIP_LIST") != null) {
                Assert.assertEquals("fgdb_ogr", oJ.get("UNZIP_LIST").toString());
                nP++;
            }
        }

        Assert.assertEquals(4, nP);
        FileUtils.forceDelete(workingDir);
    }

    @Test
    @Category(UnitTest.class)
    public void TestCreateNativeRequestOsmZipAndOsm() throws Exception {
        String jobId = "test-id-123";
        String wkdirpath = homeFolder + "/upload/" + jobId;
        File workingDir = new File(wkdirpath);
        FileUtils.forceMkdir(workingDir);

        Assert.assertTrue(workingDir.exists());

        String input = "osm.zip";
        File srcFile = new File(homeFolder + "/test-files/service/FileUploadResourceTest/" + input);
        File destFile = new File(wkdirpath + "/" + input);
        FileUtils.copyFile(srcFile, destFile);

        Assert.assertTrue(destFile.exists());

        FileUploadResource res = new FileUploadResource();

        // Let's test zip
        JSONArray results = new JSONArray();
        JSONObject zipStat = new JSONObject();
        List<String> inputsList = new ArrayList<>();
        inputsList.add(input);

        res._buildNativeRequest(jobId, "osm", "zip", input, results, zipStat);

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
        srcFile = new File(homeFolder + "/test-files/service/FileUploadResourceTest/" + input);
        destFile = new File(wkdirpath + "/" + input);
        FileUtils.copyFile(srcFile, destFile);
        Assert.assertTrue(destFile.exists());

        inputsList.add(input);

        res._buildNativeRequest(jobId, "osm1", "osm", input, results, zipStat);

        int shpCnt = 0;
        shpCnt += (Integer) zipStat.get("shpcnt");

        int fgdbCnt = 0;
        fgdbCnt += (Integer) zipStat.get("fgdbcnt");

        int osmCnt = 0;
        osmCnt += (Integer) zipStat.get("osmcnt");

        int geonamesCnt = 0;
        geonamesCnt += (Integer) zipStat.get("osmcnt");

        // Test zip containing fgdb + shp
        JSONArray resA = res._createNativeRequest(results, zipCnt, shpZipCnt, fgdbZipCnt, osmZipCnt, geonamesZipCnt,
                shpCnt, fgdbCnt, osmCnt, geonamesCnt, zipList, "TDSv61.js", jobId, "osm", inputsList, "test@test.com",
                "false", null);

        JSONObject req = (JSONObject) resA.get(0);
        JSONArray params = (JSONArray) req.get("params");

        int nP = 0;

        for (Object o : params) {
            JSONObject oJ = (JSONObject) o;

            if (oJ.get("INPUT") != null) {
                Assert.assertEquals("\"osm/DcGisRoads.osm\" \"osm/DcTigerRoads.osm\" \"osm1.osm\" ", oJ.get("INPUT").toString());
                nP++;
            }

            if (oJ.get("INPUT_PATH") != null) {
                Assert.assertEquals("upload/test-id-123", oJ.get("INPUT_PATH").toString());
                nP++;
            }

            if (oJ.get("INPUT_TYPE") != null) {
                Assert.assertEquals("OSM", oJ.get("INPUT_TYPE").toString());
                nP++;
            }

            if (oJ.get("UNZIP_LIST") != null) {
                Assert.assertEquals("osm", oJ.get("UNZIP_LIST").toString());
                nP++;
            }
        }

        Assert.assertEquals(4, nP);
        FileUtils.forceDelete(workingDir);
    }

    @Test
    @Category(UnitTest.class)
    public void TestCreateNativeRequestOgrZip() throws Exception {
        String jobId = "test-id-123";
        String wkdirpath = homeFolder + "/upload/" + jobId;
        File workingDir = new File(wkdirpath);
        FileUtils.forceMkdir(workingDir);
        Assert.assertTrue(workingDir.exists());

        String input = "ogr.zip";
        File srcFile = new File(homeFolder + "/test-files/service/FileUploadResourceTest/" + input);
        File destFile = new File(wkdirpath + "/" + input);
        FileUtils.copyFile(srcFile, destFile);
        Assert.assertTrue(destFile.exists());

        FileUploadResource res = new FileUploadResource();

        // Let's test zip
        JSONArray results = new JSONArray();
        JSONObject zipStat = new JSONObject();
        List<String> inputsList = new ArrayList<>();
        inputsList.add(input);

        res._buildNativeRequest(jobId, "ogr", "zip", input, results, zipStat);

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
        srcFile = new File(homeFolder + "/test-files/service/FileUploadResourceTest/" + input);
        destFile = new File(wkdirpath + "/" + input);
        FileUtils.copyFile(srcFile, destFile);
        Assert.assertTrue(destFile.exists());

        inputsList.add("zip1");

        res._buildNativeRequest(jobId, "zip1", "zip", input, results, zipStat);

        shpZipCnt += (Integer) zipStat.get("shpzipcnt");
        fgdbZipCnt += (Integer) zipStat.get("fgdbzipcnt");
        osmZipCnt += (Integer) zipStat.get("osmzipcnt");
        int geonamesCnt = 0;
        geonamesCnt += (Integer) zipStat.get("osmcnt");

        zipList.add("zip1");
        zipCnt++;

        // Test zip containing fgdb + shp
        int shpCnt = 0;
        int osmCnt = 0;
        int fgdbCnt = 0;
        JSONArray resA = res._createNativeRequest(results, zipCnt, shpZipCnt, fgdbZipCnt, osmZipCnt, geonamesZipCnt,
                shpCnt, fgdbCnt, osmCnt, geonamesCnt, zipList, "TDSv61.js", jobId, "ogr", inputsList, "test@test.com",
                "false", null);

        JSONObject req = (JSONObject) resA.get(0);
        JSONArray params = (JSONArray) req.get("params");

        int nP = 0;

        for (Object o : params) {
            JSONObject oJ = (JSONObject) o;

            if (oJ.get("INPUT") != null) {
                Assert.assertEquals("ogr.zip;zip1.zip", oJ.get("INPUT").toString());
                nP++;
            }

            if (oJ.get("INPUT_PATH") != null) {
                Assert.assertEquals("upload/test-id-123", oJ.get("INPUT_PATH").toString());
                nP++;
            }

            if (oJ.get("INPUT_TYPE") != null) {
                Assert.assertEquals("ZIP", oJ.get("INPUT_TYPE").toString());
                nP++;
            }

        }

        Assert.assertEquals(3, nP);
        FileUtils.forceDelete(workingDir);
    }

    @Test
    @Category(UnitTest.class)
    public void TestCreateNativeRequestOgr() throws Exception {
        String jobId = "test-id-123";
        String wkdirpath = homeFolder + "/upload/" + jobId;
        File workingDir = new File(wkdirpath);
        FileUtils.forceMkdir(workingDir);
        Assert.assertTrue(workingDir.exists());

        String input = "TransportationGroundCrv.shp";
        File srcFile = new File(homeFolder + "/test-files/service/FileUploadResourceTest/" + input);
        File destFile = new File(wkdirpath + "/" + input);
        FileUtils.copyFile(srcFile, destFile);
        Assert.assertTrue(destFile.exists());

        FileUploadResource res = new FileUploadResource();

        // Shape 1
        JSONArray results = new JSONArray();
        JSONObject zipStat = new JSONObject();
        List<String> inputsList = new ArrayList<>();
        inputsList.add(input);

        res._buildNativeRequest(jobId, "TransportationGroundCrv", "shp", input, results, zipStat);

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
        srcFile = new File(homeFolder + "/test-files/service/FileUploadResourceTest/TransportationGroundCrv.shp");
        destFile = new File(wkdirpath + "/" + input);
        FileUtils.copyFile(srcFile, destFile);
        Assert.assertTrue(destFile.exists());

        inputsList.add(input);

        res._buildNativeRequest(jobId, "TransportationGroundCrv2", "shp", input, results, zipStat);

        shpCnt += (Integer) zipStat.get("shpcnt");
        fgdbCnt += (Integer) zipStat.get("fgdbcnt");
        osmCnt += (Integer) zipStat.get("osmcnt");
        int geonamesCnt = 0;
        geonamesCnt += (Integer) zipStat.get("osmcnt");

        int zipCnt = 0;
        int shpZipCnt = 0;
        int osmZipCnt = 0;
        int fgdbZipCnt = 0;
        JSONArray resA = res._createNativeRequest(results, zipCnt, shpZipCnt, fgdbZipCnt, osmZipCnt, geonamesZipCnt,
                shpCnt, fgdbCnt, osmCnt, geonamesCnt, zipList, "TDSv61.js", jobId, "ogr", inputsList, "test@test.com",
                "false", null);

        JSONObject req = (JSONObject) resA.get(0);
        JSONArray params = (JSONArray) req.get("params");

        int nP = 0;

        for (Object o : params) {
            JSONObject oJ = (JSONObject) o;

            if (oJ.get("INPUT") != null) {
                Assert.assertEquals("\"TransportationGroundCrv.shp\" \"TransportationGroundCrv2.shp\" ", oJ.get("INPUT").toString());
                nP++;
            }

            if (oJ.get("INPUT_PATH") != null) {
                Assert.assertEquals("upload/test-id-123", oJ.get("INPUT_PATH").toString());
                nP++;
            }

            if (oJ.get("INPUT_TYPE") != null) {
                Assert.assertEquals("OGR", oJ.get("INPUT_TYPE").toString());
                nP++;
            }

        }
        Assert.assertEquals(3, nP);
        FileUtils.forceDelete(workingDir);
    }

    @Test
    @Category(UnitTest.class)
    public void TestCreateNativeRequestOsm() throws Exception {
        String jobId = "test-id-123";
        String wkdirpath = homeFolder + "/upload/" + jobId;
        File workingDir = new File(wkdirpath);
        FileUtils.forceMkdir(workingDir);
        Assert.assertTrue(workingDir.exists());

        String input = "osm1.osm";
        File srcFile = new File(homeFolder + "/test-files/service/FileUploadResourceTest/" + input);
        File destFile = new File(wkdirpath + "/" + input);
        FileUtils.copyFile(srcFile, destFile);
        Assert.assertTrue(destFile.exists());

        FileUploadResource res = new FileUploadResource();

        // Shape 1
        JSONArray results = new JSONArray();
        JSONObject zipStat = new JSONObject();
        List<String> inputsList = new ArrayList<>();
        inputsList.add(input);

        res._buildNativeRequest(jobId, "osm1", "osm", input, results, zipStat);

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
        srcFile = new File(homeFolder + "/test-files/service/FileUploadResourceTest/" + input);
        destFile = new File(wkdirpath + "/" + input);
        FileUtils.copyFile(srcFile, destFile);
        Assert.assertTrue(destFile.exists());

        inputsList.add(input);

        res._buildNativeRequest(jobId, "osm2", "osm", input, results, zipStat);

        shpCnt += (Integer) zipStat.get("shpcnt");
        fgdbCnt += (Integer) zipStat.get("fgdbcnt");
        osmCnt += (Integer) zipStat.get("osmcnt");
        geonamesCnt += (Integer) zipStat.get("osmcnt");

        int zipCnt = 0;
        int shpZipCnt = 0;
        int osmZipCnt = 0;
        int fgdbZipCnt = 0;
        int geonamesZipCnt = 0;
        JSONArray resA = res._createNativeRequest(results, zipCnt, shpZipCnt, fgdbZipCnt, osmZipCnt, geonamesZipCnt,
                shpCnt, fgdbCnt, osmCnt, geonamesCnt, zipList, "TDSv61.js", jobId, "osm", inputsList, "test@test.com",
                "false", null);

        JSONObject req = (JSONObject) resA.get(0);
        JSONArray params = (JSONArray) req.get("params");

        int nP = 0;

        for (Object o : params) {
            JSONObject oJ = (JSONObject) o;

            if (oJ.get("INPUT") != null) {
                Assert.assertEquals("\"osm1.osm\" \"osm2.osm\" ", oJ.get("INPUT").toString());
                nP++;
            }

            if (oJ.get("INPUT_PATH") != null) {
                Assert.assertEquals("upload/test-id-123", oJ.get("INPUT_PATH").toString());
                nP++;
            }

            if (oJ.get("INPUT_TYPE") != null) {
                Assert.assertEquals("OSM", oJ.get("INPUT_TYPE").toString());
                nP++;
            }

        }

        Assert.assertEquals(3, nP);
        FileUtils.forceDelete(workingDir);
    }

    @Test
    @Category(UnitTest.class)
    public void TestCreateNativeRequestFgdb() throws Exception {
        String jobId = "test-id-123";
        String wkdirpath = homeFolder + "/upload/" + jobId;
        File workingDir = new File(wkdirpath);
        FileUtils.forceMkdir(workingDir);
        Assert.assertTrue(workingDir.exists());

        String input = "DcGisRoads.gdb";
        File srcDir = new File(homeFolder + "/test-files/service/FileUploadResourceTest/" + input);
        File destDir = new File(wkdirpath + "/" + input);
        FileUtils.copyDirectory(srcDir, destDir);
        Assert.assertTrue(destDir.exists());

        FileUploadResource res = new FileUploadResource();

        // Shape 1
        JSONArray results = new JSONArray();
        JSONObject zipStat = new JSONObject();
        List<String> inputsList = new ArrayList<>();
        inputsList.add(input);

        res._buildNativeRequest(jobId, "DcGisRoads", "gdb", input, results, zipStat);

        int shpCnt = 0;

        List<String> zipList = new ArrayList<>();

        shpCnt += (Integer) zipStat.get("shpcnt");

        int fgdbCnt = 0;
        fgdbCnt += (Integer) zipStat.get("fgdbcnt");

        int osmCnt = 0;
        osmCnt += (Integer) zipStat.get("osmcnt");

        int geonamesCnt = 0;
        geonamesCnt += (Integer) zipStat.get("osmcnt");

        int zipCnt = 0;
        int shpZipCnt = 0;
        int osmZipCnt = 0;
        int fgdbZipCnt = 0;
        int geonamesZipCnt = 0;
        JSONArray resA = res._createNativeRequest(results, zipCnt, shpZipCnt, fgdbZipCnt, osmZipCnt, geonamesZipCnt,
                shpCnt, fgdbCnt, osmCnt, geonamesCnt, zipList, "TDSv61.js", jobId, "fgdb", inputsList, "test@test.com",
                "false", null);

        JSONObject req = (JSONObject) resA.get(0);
        JSONArray params = (JSONArray) req.get("params");

        int nP = 0;

        for (Object o : params) {
            JSONObject oJ = (JSONObject) o;

            if (oJ.get("INPUT") != null) {
                Assert.assertEquals("\"DcGisRoads.gdb\" ", oJ.get("INPUT").toString());
                nP++;
            }

            if (oJ.get("INPUT_PATH") != null) {
                Assert.assertEquals("upload/test-id-123", oJ.get("INPUT_PATH").toString());
                nP++;
            }

            if (oJ.get("INPUT_TYPE") != null) {
                Assert.assertEquals("FGDB", oJ.get("INPUT_TYPE").toString());
                nP++;
            }

        }

        Assert.assertEquals(3, nP);
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
        Assert.assertTrue(workingDir.exists());

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
        Assert.assertTrue(out.exists());

        /*
         * Map<String,String> uploadedFiles = new HashMap<String, String>();
         * Map<String,String> uploadedFilesPaths = new HashMap<String,
         * String>();
         * 
         * res._serializeFGDB(fileItemsList, jobId, uploadedFiles,
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
        Assert.assertTrue(workingDir.exists());

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
        Assert.assertTrue(out.exists());

        /*
         * Map<String,String> uploadedFiles = new HashMap<String, String>();
         * Map<String,String> uploadedFilesPaths = new HashMap<String,
         * String>();
         * 
         * res._serializeUploadedFiles(fileItemsList, jobId, uploadedFiles,
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

}
