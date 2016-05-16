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
package hoot.services.controllers.job;

import static org.mockito.Mockito.anyString;
import static org.mockito.Mockito.verify;

import java.io.File;

import javax.ws.rs.core.Response;

import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.mockito.Matchers;
import org.mockito.Mockito;

import hoot.services.HootProperties;
import hoot.services.UnitTest;


public class ExportJobResourceTest {

    @Test
    @Category(UnitTest.class)
    public void testProcess() throws Exception {
        String params = "{\"translation\":\"MGCP.js\",\"inputtype\":\"db\",\"input\":\"ToyTestA\",\"outputtype\":\"gdb\",\"removereview\" : \"false\"}";

        String tempOutputPath = HootProperties.getProperty("tempOutputPath");
        JSONObject arg = new JSONObject();
        arg.put("outputfolder", tempOutputPath + "/" + "f9a8d471");
        tempOutputPath = arg.toJSONString();
        String jobArgs = ",\"exec\":\"osm2ogrscript\",\"params\":[{\"input\":\"ToyTestA\"},{\"translation\":\"MGCP.js\"},";
        jobArgs += "{\"outputtype\":\"gdb\"},{\"removereview\":\"false\"},{\"inputtype\":\"db\"},";
        jobArgs += tempOutputPath + ",";
        jobArgs += "{\"output\":\"f9a8d471\"},{\"outputname\":\"f9a8d471\"}],";
        jobArgs += "\"exectype\":\"make\"}";

        ExportJobResource spy = Mockito.spy(new ExportJobResource());
        Mockito.doNothing().when((JobControllerBase) spy).postJobRquest(anyString(), anyString());
        Response resp = spy.process(params);
        String result = resp.getEntity().toString();
        JSONParser parser = new JSONParser();
        JSONObject o = (JSONObject) parser.parse(result);
        String jobId = o.get("jobid").toString();

        jobArgs = jobArgs.replaceAll("f9a8d471", jobId);
        verify(spy).postJobRquest(Matchers.matches(jobId), Matchers.endsWith(jobArgs));
    }

    @Test
    @Category(UnitTest.class)
    public void testProcessForWFS() throws Exception {
        String wfsStoreDb = HootProperties.getProperty("wfsStoreDb");
        String userid = HootProperties.getProperty("dbUserId");
        String pwd = HootProperties.getProperty("dbPassword");
        String host = HootProperties.getProperty("dbHost");
        String[] hostParts = host.split(":");
        String pgUrl = "host='" + hostParts[0] + "' port='" + hostParts[1] + "' user='" + userid + "' password='" + pwd
                + "' dbname='" + wfsStoreDb + "'";

        String params = "{\"translation\":\"MGCP.js\",\"inputtype\":\"db\",\"input\":\"ToyTestA\",\"outputtype\":\"wfs\",\"removereview\" : \"false\"}";

        String tempOutputPath = HootProperties.getProperty("tempOutputPath");
        JSONObject arg = new JSONObject();
        arg.put("outputfolder", tempOutputPath + "/" + "f9a8d471");
        tempOutputPath = arg.toJSONString();
        String jobArgs = ",\"exec\":\"osm2ogrscript\",\"params\":[{\"input\":\"ToyTestA\"},{\"translation\":\"MGCP.js\"},{\"outputtype\":\"wfs\"},"
                + "{\"removereview\":\"false\"},{\"inputtype\":\"db\"},";
        jobArgs += tempOutputPath + ",";
        jobArgs += "{\"output\":\"f9a8d471\"},{\"outputname\":\"f9a8d471\"}," + "{\"PG_URL\":\"" + pgUrl
                + "\"}],\"exectype\":\"make\"},{\"class\":\"hoot.services.controllers.wfs.WfsManager\","
                + "\"method\":\"createWfsResource\",\"params\":[{\"isprimitivetype\":\"false\",\"value\":\"f9a8d471\","
                + "\"paramtype\":\"java.lang.String\"}],\"exectype\":\"reflection_sync\"}]";

        ExportJobResource spy = Mockito.spy(new ExportJobResource());
        Mockito.doNothing().when((JobControllerBase) spy).postChainJobRquest(anyString(), anyString());
        Response resp = spy.process(params);
        String result = resp.getEntity().toString();
        JSONParser parser = new JSONParser();
        JSONObject o = (JSONObject) parser.parse(result);
        String jobId = o.get("jobid").toString();

        jobArgs = jobArgs.replaceAll("f9a8d471", jobId);
        verify(spy).postChainJobRquest(Matchers.matches(jobId), Matchers.endsWith(jobArgs));
    }

    @Test
    @Category(UnitTest.class)
    public void testGetExportResources() throws Exception {
        ExportJobResource exRes = new ExportJobResource();
        Response resp = exRes.getExportResources();
        String result = resp.getEntity().toString();

        String homeFolder = HootProperties.getProperty("homeFolder");
        String translationExtPath = HootProperties.getProperty("translationExtPath");

        String transExtPath = homeFolder + "/" + "/plugins-local/script/utp";
        if (translationExtPath != null && translationExtPath.length() > 0) {
            transExtPath = translationExtPath;
        }

        String expected = "";
        File f = new File(transExtPath);
        if (f.exists() && f.isDirectory()) {
            expected = "[{\"description\":\"LTDS 4.0\",\"name\":\"TDS\"},{\"description\":\"MGCP\",\"name\":\"MGCP\"},{\"description\":\"UTP\",\"name\":\"UTP\"}]";
        }
        else {
            expected = "[{\"description\":\"LTDS 4.0\",\"name\":\"TDS\"},{\"description\":\"MGCP\",\"name\":\"MGCP\"}]";
        }
        Assert.assertEquals(expected, result);
    }
}