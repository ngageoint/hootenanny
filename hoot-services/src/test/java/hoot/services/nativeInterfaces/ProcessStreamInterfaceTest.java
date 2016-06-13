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
package hoot.services.nativeInterfaces;

import java.lang.reflect.Method;
import java.util.LinkedList;
import java.util.List;

import org.apache.commons.lang3.ArrayUtils;
import org.deegree.commons.tom.ows.LanguageString;
import org.deegree.process.jaxb.java.CodeType;
import org.deegree.process.jaxb.java.LiteralInputDefinition;
import org.deegree.services.wps.ProcessletInputs;
import org.deegree.services.wps.input.LiteralInput;
import org.deegree.services.wps.input.LiteralInputImpl;
import org.deegree.services.wps.input.ProcessletInput;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.HootProperties;
import hoot.services.UnitTest;


public class ProcessStreamInterfaceTest {

    @SuppressWarnings("unused")
    private static final Logger log = LoggerFactory.getLogger(ProcessStreamInterfaceTest.class);
    
    @Test
    @Category(UnitTest.class)
    public void testcreateCmd() throws Exception {
        JSONArray args = new JSONArray();
        JSONObject translation = new JSONObject();
        translation.put("translation", "/test/loc/translation.js");
        args.add(translation);

        JSONObject output = new JSONObject();
        output.put("output", "/test/loc/out.osm");
        args.add(output);

        JSONObject input = new JSONObject();
        input.put("input", "/test/loc/input.shp");
        args.add(output);

        JSONObject command = new JSONObject();
        command.put("exectype", "hoot");
        command.put("exec", "ogr2osm");
        command.put("params", args);

        ProcessStreamInterface ps = new ProcessStreamInterface();

        Class<?>[] cArg = new Class[1];
        cArg[0] = JSONObject.class;
        Method method = ProcessStreamInterface.class.getDeclaredMethod("createCmdArray", cArg);
        method.setAccessible(true);
        String[] ret = (String[]) method.invoke(ps, command);
        String commandStr = ArrayUtils.toString(ret);

        String expected = "{hoot,--ogr2osm,/test/loc/translation.js,/test/loc/out.osm,/test/loc/out.osm}";
        Assert.assertEquals(expected, commandStr);
    }


    private JSONArray parseRequestParams(ProcessletInputs in) {
        JSONArray commandArgs = new JSONArray();
        List<ProcessletInput> params = in.getParameters();

        for (ProcessletInput param : params) {
            JSONObject commandArg = new JSONObject();
            ProcessletInput input = param;
            String id = input.getIdentifier().getCode();
            String value = ((LiteralInput) in.getParameter(id)).getValue().trim();
            try {
                commandArg.put(id, value);
                commandArgs.add(commandArg);
            }
            catch (Exception e) {
                log.error(e.getMessage());
            }
        }
        return commandArgs;
    }

    private String createPostBody(JSONArray args) {
        String resourceName = this.getClass().getSimpleName();

        JSONObject command = new JSONObject();
        command.put("exectype", "make");
        command.put("exec", HootProperties.getPropertyOrDefault("ETLMakefile"));
        command.put("caller", resourceName);
        command.put("params", args);

        return command.toString();
    }

    private static LiteralInputImpl createLiteralInput(String key, String value) {
        LiteralInputDefinition lid = new LiteralInputDefinition();
        CodeType ct = new CodeType();
        ct.setValue(key);
        lid.setIdentifier(ct);
        LiteralInputImpl lii = new LiteralInputImpl(lid, new LanguageString(key, null), new LanguageString(key, null),
                value, null);
        return lii;
    }

    private String generateJobParam() throws Exception {
        LinkedList<ProcessletInput> allInputs = new LinkedList<>();
        allInputs.add(createLiteralInput("translation", "/test/file/test.js"));
        allInputs.add(createLiteralInput("INPUT_TYPE", "OSM"));
        allInputs.add(createLiteralInput("INPUT", "/test/file/INPUT.osm"));
        ProcessletInputs in = new ProcessletInputs(allInputs);
        JSONArray arr = this.parseRequestParams(in);
        return createPostBody(arr);
    }

    @Test
    @Category(UnitTest.class)
    public void testcreateScriptCmd() throws Exception {
        String sParam = this.generateJobParam();
        JSONParser parser = new JSONParser();
        JSONObject command = (JSONObject) parser.parse(sParam);

        ProcessStreamInterface ps = new ProcessStreamInterface();

        Class<?>[] cArg = new Class[1];
        cArg[0] = JSONObject.class;
        Method method = ProcessStreamInterface.class.getDeclaredMethod("createScriptCmdArray", cArg);
        method.setAccessible(true);
        command.put("jobId", "123-456-789");
        String[] ret = (String[]) method.invoke(ps, command);
        String commandStr = ArrayUtils.toString(ret);

        String coreScriptPath = HootProperties.getProperty("coreScriptPath");

        HootProperties.getProperty("coreScriptOutputPath");

        String ETLMakefile = HootProperties.getPropertyOrDefault("ETLMakefile");
        String makePath = coreScriptPath + "/" + ETLMakefile;

        // [make, -f, /project/hoot/scripts/makeetl,
        // translation=/test/file/test.js, INPUT_TYPE=OSM,
        // INPUT=/test/file/INPUT.osm, jobid=123-456-789]
        String expected = "{make,-f," + makePath
                + ",translation=/test/file/test.js,INPUT_TYPE=OSM,INPUT=/test/file/INPUT.osm";
        expected += ",jobid=123-456-789";

        String dbname = HootProperties.getProperty("dbName");
        String userid = HootProperties.getProperty("dbUserId");
        String pwd = HootProperties.getProperty("dbPassword");
        String host = HootProperties.getProperty("dbHost");
        String dbUrl = "hootapidb://" + userid + ":" + pwd + "@" + host + "/" + dbname;
        expected += ",DB_URL=" + dbUrl;
        
        dbname = HootProperties.getProperty("osmApiDbName");
        userid = HootProperties.getProperty("osmApiDbUserId");
        pwd = HootProperties.getProperty("osmApiDbPassword");
        host = HootProperties.getProperty("osmApiDbHost");
        String osmApiDbUrl = "osmapidb://" + userid + ":" + pwd + "@" + host + "/" + dbname;
        expected += ",OSM_API_DB_URL=" + osmApiDbUrl + "}";

        Assert.assertEquals(expected, commandStr);
    }
}
