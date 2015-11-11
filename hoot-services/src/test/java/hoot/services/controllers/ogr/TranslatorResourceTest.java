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
package hoot.services.controllers.ogr;

import hoot.services.HootProperties;
import hoot.services.UnitTest;

import org.junit.Ignore;
import org.junit.Test;
import org.junit.experimental.categories.Category;

public class TranslatorResourceTest {
	
	@Ignore
	@Test
	@Category(UnitTest.class)
	public void testParseOsm() throws Exception
	{

	}
	
	@Ignore
	@Test
	@Category(UnitTest.class)
	public void testExecOsmConversion() throws Exception
	{
		String homeFolder = HootProperties.getInstance().getProperty("homeFolder", "");
		/*Translator translator = new Translator();
		translator.execOsmConversion(homeFolder + "/test-files/conflate/unified/AllDataTypesA.osm", 
				homeFolder + "/test-output/OsmConversion.osm");*/
	}
	
	@Ignore
	@Test
	@Category(UnitTest.class)
	public void testTranslateToOgr() throws Exception
	{

	}
}
