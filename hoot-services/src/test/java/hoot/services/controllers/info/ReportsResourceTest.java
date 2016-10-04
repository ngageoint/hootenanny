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
import java.lang.reflect.Method;

import org.apache.commons.io.FileUtils;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.junit.BeforeClass;
import org.junit.Ignore;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import hoot.services.UnitTest;


public class ReportsResourceTest {
    private static ReportsResource rps;

    @BeforeClass
    public static void oneTimeSetup() {
        rps = new ReportsResource();
        assertNotNull(HOME_FOLDER);
        assertNotNull(RPT_STORE_PATH);
    }

    @Ignore
    @Test
    @Category(UnitTest.class)
    public void testGetMetaData() throws Exception {
        String storePath = HOME_FOLDER + "/" + RPT_STORE_PATH;
        File dir = new File(storePath);
        File fWks = new File(storePath, "123_test");
        if (fWks.exists()) {
            FileUtils.forceDelete(fWks);
        }
        FileUtils.forceMkdir(dir);
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

    @Ignore
    @Test
    @Category(UnitTest.class)
    public void testGetReportsList() throws Exception {
        String storePath = HOME_FOLDER + "/" + RPT_STORE_PATH;
        File file = new File(storePath);
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
        FileUtils.forceMkdir(file);

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

        Method getReportsListMethod = getGetReportsListMethod();
        JSONArray out = (JSONArray) getReportsListMethod.invoke(null);

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

    @Ignore
    @Test
    @Category(UnitTest.class)
    public void testGetReportFile() throws Exception {
        String storePath = HOME_FOLDER + "/" + RPT_STORE_PATH;
        File dir = new File(storePath);
        File fWks = new File(storePath + "/123_test_file");
        if (fWks.exists()) {
            FileUtils.forceDelete(fWks);
        }
        FileUtils.forceMkdir(dir);

        FileUtils.forceMkdir(fWks);
        String currTime = String.valueOf(System.currentTimeMillis());
        JSONObject metaData = new JSONObject();
        metaData.put("name", "Test Report1");
        metaData.put("description", "This is test report 1");
        metaData.put("created", currTime);
        metaData.put("reportpath", HOME_FOLDER + "/test-files/test_report1.pdf");
        File meta = new File(storePath + "/123_test_file/meta.data");
        FileUtils.write(meta, metaData.toJSONString());

        Method getReportFileMethod = getGetReportFileMethod();
        File fout = (File) getReportFileMethod.invoke(null, "123_test_file");

        assertNotNull(fout);

        fout = (File) getReportFileMethod.invoke(null, "123_test_file_not_there");

        assertNull(fout);

        FileUtils.forceDelete(fWks);
    }

    @Ignore
    @Test
    @Category(UnitTest.class)
    public void testDeleteReport() throws Exception {
        String storePath = HOME_FOLDER + "/" + RPT_STORE_PATH;
        File fWks = new File(storePath + "/123_test_del");
        if (fWks.exists()) {
            FileUtils.forceDelete(fWks);
        }

        File dir = new File(storePath);
        FileUtils.forceMkdir(dir);

        FileUtils.forceMkdir(fWks);
        String currTime = String.valueOf(System.currentTimeMillis());
        JSONObject metaData = new JSONObject();
        metaData.put("name", "Test Report1");
        metaData.put("description", "This is test report 1");
        metaData.put("created", currTime);
        metaData.put("reportpath", HOME_FOLDER + "/test-files/test_report1.pdf");
        File meta = new File(storePath + "/123_test_del/meta.data");
        FileUtils.write(meta, metaData.toJSONString());

        Method deleteReportMethod = getDeleteReportMethod();

        boolean isDel = (Boolean) deleteReportMethod.invoke(null, "123_test_del_not_exist");
        assertFalse(isDel);

        isDel = (Boolean) deleteReportMethod.invoke(null, "123_test_del");

        assertTrue(isDel);
    }

    private static Method getDeleteReportMethod() throws NoSuchMethodException {
        Method deleteReportMethod = ReportsResource.class.getDeclaredMethod("deleteReport", String.class);
        deleteReportMethod.setAccessible(true);
        return deleteReportMethod;
    }

    private static Method getGetReportFileMethod() throws NoSuchMethodException {
        Method getReportFileMethod = ReportsResource.class.getDeclaredMethod("getReportFile", String.class);
        getReportFileMethod.setAccessible(true);
        return getReportFileMethod;
    }

    private static Method getMetaDataMethod() throws NoSuchMethodException {
        Method getMetaDataMethod = ReportsResource.class.getDeclaredMethod("getMetaData", String.class);
        getMetaDataMethod.setAccessible(true);
        return getMetaDataMethod;
    }

    private static Method getGetReportsListMethod() throws NoSuchMethodException {
        Method getReportsListMethod = ReportsResource.class.getDeclaredMethod("getReportsList");
        getReportsListMethod.setAccessible(true);
        return getReportsListMethod;
    }
}
