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
package hoot.services.controllers.info;


import static hoot.services.HootProperties.HOME_FOLDER;
import static hoot.services.HootProperties.RPT_STORE_PATH;
import static org.junit.Assert.*;

import java.io.File;
import java.io.InputStream;
import java.lang.reflect.Method;
import java.net.URL;

import javax.ws.rs.NotFoundException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import org.apache.commons.io.FileUtils;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import hoot.services.UnitTest;
import hoot.services.testsupport.HootCustomPropertiesSetter;
import hoot.services.testsupport.HootServicesJerseyTestAbstract;


public class ReportsResourceTest extends HootServicesJerseyTestAbstract {
    private static File homeFolder;
    private static String original_HOME_FOLDER;
    private static String storePath;

    @BeforeClass
    public static void beforeClass() throws Exception {
        original_HOME_FOLDER = HOME_FOLDER;
        homeFolder = new File(FileUtils.getTempDirectory(), "ReportsResourceTest");
        FileUtils.forceMkdir(homeFolder);
        assertTrue(homeFolder.exists());
        HootCustomPropertiesSetter.setProperty("HOME_FOLDER", homeFolder.getAbsolutePath());
        HootCustomPropertiesSetter.setProperty("REPORTS_PATH", homeFolder.getAbsolutePath() + "/" + RPT_STORE_PATH, ReportsResource.class);

        storePath = HOME_FOLDER + "/" + RPT_STORE_PATH;
        File dir = new File(storePath);
        FileUtils.forceMkdir(dir);
    }

    @AfterClass
    public static void afterClass() throws Exception {
        FileUtils.deleteDirectory(homeFolder);
        HootCustomPropertiesSetter.setProperty("HOME_FOLDER", original_HOME_FOLDER);
    }

    @Test
    @Category(UnitTest.class)
    public void testGetMetaData() throws Exception {
        ReportsResource rps = new ReportsResource();

        File fWks = new File(storePath, "123_test");
        if (fWks.exists()) {
            FileUtils.forceDelete(fWks);
        }
        Method getMetaDataMethod = getMetaDataMethod();

        JSONObject res = (JSONObject) getMetaDataMethod.invoke(null, "123_test");
        assertNull(res.get("name"));

        FileUtils.forceMkdir(fWks);
        String currTime = String.valueOf(System.currentTimeMillis());
        JSONObject metaData = new JSONObject();
        metaData.put("name", "Test Report1");
        metaData.put("description", "This is test report 1");
        metaData.put("created", currTime);
        metaData.put("reportpath", storePath + "/123_test/report.pdf");
        File meta = new File(storePath + "/123_test/meta.data");
        FileUtils.write(meta, metaData.toJSONString());

        res = (JSONObject) getMetaDataMethod.invoke(null, "123_test");

        assertEquals("Test Report1", res.get("name").toString());
        assertEquals("This is test report 1", res.get("description").toString());
        assertEquals(currTime, res.get("created").toString());
        assertEquals("reportpath", storePath + "/123_test/report.pdf", res.get("reportpath").toString());

        FileUtils.forceDelete(fWks);
    }

    @Test
    @Category(UnitTest.class)
    public void testGetReportsList() throws Exception {
        File fWks1 = new File(storePath + "/123_test1");
        if (fWks1.exists()) {
            FileUtils.forceDelete(fWks1);
        }

        File fWks2 = new File(storePath + "/123_test2");
        if (fWks2.exists()) {
            FileUtils.forceDelete(fWks2);
        }

        File fWks3 = new File(storePath + "/123_test3");
        if (fWks3.exists()) {
            FileUtils.forceDelete(fWks3);
        }

        FileUtils.forceMkdir(fWks1);
        String currTime = String.valueOf(System.currentTimeMillis());
        JSONObject metaData = new JSONObject();
        metaData.put("name", "Test Report1");
        metaData.put("description", "This is test report 1");
        metaData.put("created", currTime);
        metaData.put("reportpath", storePath + "/123_test1/report.pdf");
        File meta = new File(storePath, "123_test1/meta.data");
        FileUtils.write(meta, metaData.toJSONString());

        FileUtils.forceMkdir(fWks2);
        currTime = String.valueOf(System.currentTimeMillis());
        metaData = new JSONObject();
        metaData.put("name", "Test Report2");
        metaData.put("description", "This is test report 2");
        metaData.put("created", currTime);
        metaData.put("reportpath", storePath + "/123_test2/report.pdf");
        meta = new File(storePath, "123_test2/meta.data");
        FileUtils.write(meta, metaData.toJSONString());

        FileUtils.forceMkdir(fWks3);
        currTime = String.valueOf(System.currentTimeMillis());
        metaData = new JSONObject();
        metaData.put("name", "Test Report3");
        metaData.put("description", "This is test report 3");
        metaData.put("created", currTime);
        metaData.put("reportpath", storePath + "/123_test3/report.pdf");
        meta = new File(storePath + "/123_test3/meta.data");
        FileUtils.write(meta, metaData.toJSONString());

        Response responseData =
                target("/reports/list")
                        .request(MediaType.APPLICATION_JSON)
                        .get();

        String jsonStr = responseData.readEntity(String.class);
        JSONParser parser = new JSONParser();
        JSONArray out = (JSONArray) parser.parse(jsonStr);

        int nCount1 = 0;
        int nCount2 = 0;
        int nCount3 = 0;
        for (Object o : out) {
            JSONObject jo = (JSONObject) o;
            if (jo.get("name").toString().equals("Test Report1")) {
                nCount1++;
            }

            if (jo.get("name").toString().equals("Test Report2")) {
                nCount2++;
            }

            if (jo.get("name").toString().equals("Test Report3")) {
                nCount3++;
            }
        }

        assertEquals(1, nCount1);
        assertEquals(1, nCount2);
        assertEquals(1, nCount3);

        FileUtils.forceDelete(fWks1);
        FileUtils.forceDelete(fWks2);
        FileUtils.forceDelete(fWks3);
    }

    @Test
    @Category(UnitTest.class)
    public void testGetExistingReportFile() throws Exception {
        File fWks = new File(storePath + "/123_test_file");
        if (fWks.exists()) {
            FileUtils.forceDelete(fWks);
        }

        FileUtils.forceMkdir(fWks);
        String currTime = String.valueOf(System.currentTimeMillis());
        JSONObject metaData = new JSONObject();
        metaData.put("name", "Test Report1");
        metaData.put("description", "This is test report 1");
        metaData.put("created", currTime);
        metaData.put("reportpath", HOME_FOLDER + "/test_report1.pdf");

        File dest = new File(HOME_FOLDER, "test_report1.pdf");
        URL inputUrl = ReportsResourceTest.class.getResource("/hoot/services/controllers/info/test_report1.pdf");
        FileUtils.copyURLToFile(inputUrl, dest);

        File meta = new File(storePath + "/123_test_file/meta.data");
        FileUtils.write(meta, metaData.toJSONString());

        Response responseData =
                target("/reports/get")
                        .queryParam("id", "123_test_file")
                        .queryParam("reportname", "test")
                        .request(MediaType.APPLICATION_OCTET_STREAM)
                        .get();

        InputStream is = responseData.readEntity(InputStream.class);

        assertNotNull(is);

        FileUtils.forceDelete(fWks);
    }

    @Test(expected = NotFoundException.class)
    @Category(UnitTest.class)
    public void getNonExistingReportFile() throws Exception {
        InputStream responseData =
                target("/reports/get")
                        .queryParam("id", "non-existent-id")
                        .queryParam("reportname", "non-existent-report")
                        .request(MediaType.APPLICATION_OCTET_STREAM)
                        .get(InputStream.class);
    }

    @Test
    @Category(UnitTest.class)
    public void testDeleteExistingReport() throws Exception {
        File fWks = new File(storePath + "/123_test_del");
        if (fWks.exists()) {
            FileUtils.forceDelete(fWks);
        }

        FileUtils.forceMkdir(fWks);
        String currTime = String.valueOf(System.currentTimeMillis());
        JSONObject metaData = new JSONObject();
        metaData.put("name", "Test Report1");
        metaData.put("description", "This is test report 1");
        metaData.put("created", currTime);
        metaData.put("reportpath", HOME_FOLDER + "/test-files/test_report1.pdf");
        File meta = new File(storePath + "/123_test_del/meta.data");
        FileUtils.write(meta, metaData.toJSONString());

        Response responseData =
                target("/reports/delete")
                        .queryParam("id", "123_test_del")
                        .request(MediaType.APPLICATION_JSON)
                        .get();

        String jsonStr = responseData.readEntity(String.class);
        JSONParser parser = new JSONParser();
        JSONObject json = (JSONObject) parser.parse(jsonStr);

        assertTrue(Boolean.valueOf((String) json.get("deleted")));
    }

    @Test(expected = NotFoundException.class)
    @Category(UnitTest.class)
    public void testDeleteNonExistingReport() throws Exception {
        JSONObject responseData =
                target("/reports/delete")
                        .queryParam("id", "non-existing-id")
                        .request(MediaType.APPLICATION_JSON)
                        .get(JSONObject.class);
    }

    private static Method getMetaDataMethod() throws NoSuchMethodException {
        Method getMetaDataMethod = ReportsResource.class.getDeclaredMethod("getMetaData", String.class);
        getMetaDataMethod.setAccessible(true);
        return getMetaDataMethod;
    }
}
