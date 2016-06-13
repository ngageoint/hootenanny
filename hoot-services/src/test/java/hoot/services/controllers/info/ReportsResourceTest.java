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


import java.io.File;

import org.apache.commons.io.FileUtils;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import hoot.services.UnitTest;


public class ReportsResourceTest {
    private static ReportsResource rps;

    @BeforeClass
    public static void oneTimeSetup() {
        rps = new ReportsResource();
        Assert.assertNotNull(ReportsResource._homeFolder);
        Assert.assertNotNull(ReportsResource._rptStorePath);
    }

    @Test
    @Category(UnitTest.class)
    public void testGetMetaData() throws Exception {
        String storePath = ReportsResource._homeFolder + "/" + ReportsResource._rptStorePath;
        File dir = new File(storePath);
        File fWks = new File(storePath + "/123_test");
        if (fWks.exists()) {
            FileUtils.forceDelete(fWks);
        }
        FileUtils.forceMkdir(dir);
        JSONObject res = rps._getMetaData("123_test");
        Assert.assertNull(res.get("name"));

        FileUtils.forceMkdir(fWks);
        String currTime = String.valueOf(System.currentTimeMillis());
        JSONObject metaData = new JSONObject();
        metaData.put("name", "Test Report1");
        metaData.put("description", "This is test report 1");
        metaData.put("created", currTime);
        metaData.put("reportpath", storePath + "/123_test/report.pdf");
        File meta = new File(storePath + "/123_test/meta.data");
        FileUtils.write(meta, metaData.toJSONString());

        res = rps._getMetaData("123_test");

        Assert.assertEquals("Test Report1", res.get("name").toString());
        Assert.assertEquals("This is test report 1", res.get("description").toString());
        Assert.assertEquals(currTime, res.get("created").toString());
        Assert.assertEquals("reportpath", storePath + "/123_test/report.pdf", res.get("reportpath").toString());

        FileUtils.forceDelete(fWks);
    }

    @Test
    @Category(UnitTest.class)
    public void testGetReportsList() throws Exception {
        String storePath = ReportsResource._homeFolder + "/" + ReportsResource._rptStorePath;
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
        File meta = new File(storePath + "/123_test1/meta.data");
        FileUtils.write(meta, metaData.toJSONString());

        FileUtils.forceMkdir(fWks2);
        currTime = String.valueOf(System.currentTimeMillis());
        metaData = new JSONObject();
        metaData.put("name", "Test Report2");
        metaData.put("description", "This is test report 2");
        metaData.put("created", currTime);
        metaData.put("reportpath", storePath + "/123_test2/report.pdf");
        meta = new File(storePath + "/123_test2/meta.data");
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

        JSONArray out = rps._getReportsList();

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

        Assert.assertEquals(1, nCount1);
        Assert.assertEquals(1, nCount2);
        Assert.assertEquals(1, nCount3);

        FileUtils.forceDelete(fWks1);
        FileUtils.forceDelete(fWks2);
        FileUtils.forceDelete(fWks3);
    }

    @Test
    @Category(UnitTest.class)
    public void testGetReportFile() throws Exception {
        String storePath = ReportsResource._homeFolder + "/" + ReportsResource._rptStorePath;
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
        metaData.put("reportpath", ReportsResource._homeFolder + "/test-files/test_report1.pdf");
        File meta = new File(storePath + "/123_test_file/meta.data");
        FileUtils.write(meta, metaData.toJSONString());

        File fout = rps._getReportFile("123_test_file");

        Assert.assertNotNull(fout);

        fout = rps._getReportFile("123_test_file_not_there");

        Assert.assertNull(fout);

        FileUtils.forceDelete(fWks);
    }

    @Test
    @Category(UnitTest.class)
    public void testDeleteReport() throws Exception {
        String storePath = ReportsResource._homeFolder + "/" + ReportsResource._rptStorePath;
        File dir = new File(storePath);
        File fWks = new File(storePath + "/123_test_del");
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
        metaData.put("reportpath", ReportsResource._homeFolder + "/test-files/test_report1.pdf");
        File meta = new File(storePath + "/123_test_del/meta.data");
        FileUtils.write(meta, metaData.toJSONString());

        boolean isDel = rps._deleteReport("123_test_del_not_exist");
        Assert.assertFalse(isDel);

        isDel = rps._deleteReport("123_test_del");

        Assert.assertTrue(isDel);
    }
}
