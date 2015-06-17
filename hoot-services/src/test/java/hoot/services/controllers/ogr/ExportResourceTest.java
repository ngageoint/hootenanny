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
 * @copyright Copyright (C) 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.ogr;

import java.lang.reflect.Method;

import hoot.services.UnitTest;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;

public class ExportResourceTest {

	@Test
	@Category(UnitTest.class)
	public void testCreateCommand() throws Exception
	{
		ExportResource res = new ExportResource();
		
		Class[] cArg = new Class[5];
		cArg[0] = String.class;
		cArg[1] = String.class;
		cArg[2] = String.class;
		cArg[3] = String.class;
		cArg[4] = String.class;
		Method method = ExportResource.class.getDeclaredMethod("createCommand", cArg);
		method.setAccessible(true);
		JSONObject param = (JSONObject)method.invoke(res, "123-456-789", "FGDB", "TDS.js", "123-456-789", "db");
		JSONArray arr = (JSONArray)param.get("params");
		
		for(int i=0; i<arr.size(); i++)
		{
			JSONObject p = (JSONObject)arr.get(i);
			if(p.containsKey("translation"))
			{
				Assert.assertEquals("TDS.js", p.get("translation"));
			}
			
			if(p.containsKey("input"))
			{
				Assert.assertEquals("123-456-789", p.get("input"));
			}
			
			if(p.containsKey("output"))
			{
				Assert.assertEquals("123-456-789", p.get("output"));
			}
		}
		
	}
}
