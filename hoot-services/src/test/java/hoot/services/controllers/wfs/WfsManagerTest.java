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
package hoot.services.controllers.wfs;

import static org.mockito.Matchers.anyString;
import static org.mockito.Mockito.verify;

import java.util.ArrayList;
import java.util.List;

import hoot.services.UnitTest;

import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.mockito.Matchers;
import org.mockito.Mockito;

public class WfsManagerTest 
{
	@Test
	@Category(UnitTest.class)
	public void createWFSDatasourceFeatureTest() throws Exception
	{
		WfsManager spy = Mockito.spy(new WfsManager());
		Mockito.doReturn(null).when(spy)._createFeatureStore(anyString(), anyString());
		Mockito.doNothing().when(spy)._addToWfsStore(null);
		List<String> features = new ArrayList<String>();
		features.add("feat1");
		features.add("feat2");
		spy._createWFSDatasourceFeature("testdb", "testconn", features);
		String content = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><SQLFeatureStore xmlns=\"http://www.deegree.org/datasource/feature/sql\" " +
				"xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" configVersion=\"3.2.0\" " +
				"xsi:schemaLocation=\"http://www.deegree.org/datasource/feature/sql  " +
				"http://schemas.deegree.org/datasource/feature/sql/3.2.0/sql.xsd\">" +
				"<JDBCConnId>testconn</JDBCConnId><FeatureTypeMapping table=\"feat1\"/>" +
				"<FeatureTypeMapping table=\"feat2\"/></SQLFeatureStore>";
		verify(spy)._createFeatureStore(Matchers.matches("testdb"), Matchers.startsWith(content));
	}
}
