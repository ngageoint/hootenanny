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
package hoot.services.controllers.wps;

import java.util.LinkedList;

import org.json.simple.JSONArray;
import org.junit.Assert;
import org.deegree.services.wps.ProcessletInputs;
import org.deegree.services.wps.input.ProcessletInput;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import hoot.services.HootProperties;
import hoot.services.IntegrationTest;
import hoot.services.wps.WpsUtils;

import java.io.IOException;
import java.lang.reflect.*;


public class CookieCutterProcessletTest {


	  @Test
	  @Category(IntegrationTest.class)
	  public void testParseParam() throws Exception
	  {
		String ret = generateJobParam();
		String expected = "";
		try {	    	
	    	String CCMakefile = HootProperties.getProperty("CookieCutterMakefilePath");
	    	
	    	expected = "{\"caller\":\"CookieCutterProcesslet\",\"exec\":\"" + CCMakefile + "\"";
				expected += ",\"params\":[{\"cuttershapetype\":\"DB\"},{\"cuttershape\":\"123456\"},";
				expected += "{\"alpha\":\"5000\"},{\"buffer\":\"0\"},{\"inputtype\":\"DB\"},{\"input\":\"987654\"},";
				expected += "{\"outputbuffer\":\"-500\"},{\"crop\":\"false\"}],\"exectype\":\"make\"}";
			} catch (IOException e) {
				
		}
 
		Assert.assertEquals(expected, ret);
	  }
	  
	  public String generateJobParam() throws Exception
	  {
	  	CookieCutterProcesslet processlet = new CookieCutterProcesslet();
			LinkedList<ProcessletInput> allInputs = new LinkedList<ProcessletInput>();
			allInputs.add(WpsUtils.createLiteralInput("cuttershapetype", "DB"));
			allInputs.add(WpsUtils.createLiteralInput("cuttershape", "123456"));
			allInputs.add(WpsUtils.createLiteralInput("alpha", "5000"));
			allInputs.add(WpsUtils.createLiteralInput("buffer", "0"));
			allInputs.add(WpsUtils.createLiteralInput("inputtype", "DB"));
			allInputs.add(WpsUtils.createLiteralInput("input", "987654"));
			allInputs.add(WpsUtils.createLiteralInput("outputbuffer", "-500"));
			allInputs.add(WpsUtils.createLiteralInput("crop", "false"));
			ProcessletInputs in = new ProcessletInputs(allInputs);
			 
			  
			JSONArray arr = processlet.parseRequestParams(in);
			    
			Class<?>[] cArg = new Class[1];
			cArg[0] = JSONArray.class;
			Method method = JobProcesslet.class.getDeclaredMethod("createPostBody", cArg);
			method.setAccessible(true);
			String ret = (String)method.invoke(processlet, arr);
			return ret;
	  }


}
