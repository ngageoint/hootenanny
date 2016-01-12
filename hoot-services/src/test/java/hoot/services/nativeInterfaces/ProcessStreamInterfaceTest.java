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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.nativeInterfaces;

import java.io.IOException;
import java.lang.reflect.Method;

import hoot.services.HootProperties;
import hoot.services.UnitTest;

import org.apache.commons.lang3.ArrayUtils;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;

public class ProcessStreamInterfaceTest {


	@Test
	@Category(UnitTest.class)
	public void testcreateCmd() throws Exception
	{
		java.util.UUID.randomUUID().toString();
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
		String[] ret = (String[])method.invoke(ps, command);
		String commandStr = ArrayUtils.toString(ret);

		String expected = "{hoot,--ogr2osm,/test/loc/translation.js,/test/loc/out.osm,/test/loc/out.osm}";
		Assert.assertEquals(expected, commandStr);

	}

	@SuppressWarnings("unchecked")
  @Test
	@Category(UnitTest.class)
	public void testcreateScriptCmd() throws Exception
	{
		hoot.services.controllers.wps.ETLProcessletTest etlTest = new hoot.services.controllers.wps.ETLProcessletTest();
		String sParam = etlTest.generateJobParam();
		JSONParser parser=new JSONParser();
		JSONObject command = (JSONObject)parser.parse(sParam);

		ProcessStreamInterface ps = new ProcessStreamInterface();

		Class<?>[] cArg = new Class[1];
		cArg[0] = JSONObject.class;
		Method method = ProcessStreamInterface.class.getDeclaredMethod("createScriptCmdArray", cArg);
		method.setAccessible(true);
		command.put("jobId", "123-456-789");
		String[] ret = (String[])method.invoke(ps, command);
		String commandStr = ArrayUtils.toString(ret);

		String expected = "";

		try {
	    	String coreScriptPath = HootProperties.getProperty("coreScriptPath");

	    	HootProperties.getProperty("coreScriptOutputPath");

	    	String ETLMakefile = HootProperties.getInstance().getProperty("ETLMakefile",
			          HootProperties.getDefault("ETLMakefile"));
	    	String makePath = coreScriptPath + "/" + ETLMakefile;

	    	//[make, -f, /project/hoot/scripts/makeetl, translation=/test/file/test.js, INPUT_TYPE=OSM, INPUT=/test/file/INPUT.osm,  jobid=123-456-789]
	    	expected = "{make,-f," + makePath + ",translation=/test/file/test.js,INPUT_TYPE=OSM,INPUT=/test/file/INPUT.osm";
	    	expected += ",jobid=123-456-789";

	    	String dbname = HootProperties.getProperty("dbName");
  	  	String userid = HootProperties.getProperty("dbUserId");
  	  	String pwd = HootProperties.getProperty("dbPassword");
  	  	String host = HootProperties.getProperty("dbHost");
  	  	String dbUrl = "postgresql://" + userid + ":" + pwd + "@" + host + "/" + dbname;
  	  	expected += ",DB_URL=" + dbUrl + "}";

		} catch (IOException e) {

		}

		Assert.assertEquals(expected, commandStr);

	}
}
