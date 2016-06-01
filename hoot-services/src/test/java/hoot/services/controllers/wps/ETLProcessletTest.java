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
package hoot.services.controllers.wps;

import java.lang.reflect.Method;
import java.util.LinkedList;

import org.deegree.services.wps.ProcessletInputs;
import org.deegree.services.wps.input.ProcessletInput;
import org.json.simple.JSONArray;
import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import hoot.services.HootProperties;
import hoot.services.IntegrationTest;
import hoot.services.wps.WpsUtils;


public class ETLProcessletTest {

    @Test
    @Category(IntegrationTest.class)
    public void testParseParam() throws Exception {
        String ret = generateJobParam();
        String expected = "";
        String ETLMakefile = HootProperties.getPropertyOrDefault("ETLMakefile");

        expected = "{\"caller\":\"ETLProcesslet\",\"exec\":\"" + ETLMakefile
                + "\",\"params\":[{\"translation\":\"\\/test\\/file\\/test.js\"},";
        expected += "{\"INPUT_TYPE\":\"OSM\"},";
        expected += "{\"INPUT\":\"\\/test\\/file\\/INPUT.osm\"}],\"exectype\":\"make\"}";

        Assert.assertEquals(expected, ret);
    }

    public String generateJobParam() throws Exception {
        ETLProcesslet processlet = new ETLProcesslet();
        LinkedList<ProcessletInput> allInputs = new LinkedList<ProcessletInput>();
        allInputs.add(WpsUtils.createLiteralInput("translation", "/test/file/test.js"));
        allInputs.add(WpsUtils.createLiteralInput("INPUT_TYPE", "OSM"));
        allInputs.add(WpsUtils.createLiteralInput("INPUT", "/test/file/INPUT.osm"));
        ProcessletInputs in = new ProcessletInputs(allInputs);

        JSONArray arr = processlet.parseRequestParams(in);

        Class<?>[] cArg = new Class[1];
        cArg[0] = JSONArray.class;
        Method method = JobProcesslet.class.getDeclaredMethod("createPostBody", cArg);
        method.setAccessible(true);
        String ret = (String) method.invoke(processlet, arr);
        return ret;
    }
}
