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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.utils;

import java.util.Map;

import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import hoot.services.UnitTest;


public class PostgresUtilsTest {

    @Test
    @Category(UnitTest.class)
    public void TestPostgresObjToHStore() throws Exception {

        String tagsStr = "\"test1\"=>\"val1\", \"test2\"=>\"{\\\"label\\\":\\\"Confirmed\\\","
                + "\\\"description\\\":\\\"You can look at the point and tell what it is (e.g. mosque or airport)\\\","
                + "\\\"changes\\\":{\\\"replaceTags\\\":{\\\"hgis:imagery_confirmed\\\":\\\"confirmed\\\",\\\"hgis:accuracy\\\":"
                + "\\\"high\\\"},\\\"appendTags\\\":{\\\"source:geometry\\\":\\\"${BASEMAP_IMAGE_SOURCE}\\\",\\\"source:geometry:"
                + "sensor\\\":\\\"${BASEMAP_IMAGE_SENSOR}\\\",\\\"source:geometry:date\\\":\\\"${BASEMAP_IMAGE_DATETIME}\\\","
                + "\\\"source:geometry:id\\\":\\\"${BASEMAP_IMAGE_ID}\\\"}}}\"";
        Map<String, String> ret = PostgresUtils.parseTags(tagsStr);

        Assert.assertEquals(2, ret.size());
        Assert.assertEquals("val1", ret.get("test1"));
        Assert.assertEquals("{\\\"label\\\":\\\"Confirmed\\\",\\\"description\\\":"
                + "\\\"You can look at the point and tell what it is (e.g. mosque or airport)\\\","
                + "\\\"changes\\\":{\\\"replaceTags\\\":{\\\"hgis:imagery_confirmed\\\":\\\"confirmed\\\","
                + "\\\"hgis:accuracy\\\":\\\"high\\\"},\\\"appendTags\\\":{\\\"source:geometry\\\":"
                + "\\\"${BASEMAP_IMAGE_SOURCE}\\\",\\\"source:geometry:sensor\\\":\\\"${BASEMAP_IMAGE_SENSOR}\\\","
                + "\\\"source:geometry:date\\\":\\\"${BASEMAP_IMAGE_DATETIME}\\\",\\\"source:geometry:id\\\":"
                + "\\\"${BASEMAP_IMAGE_ID}\\\"}}}", ret.get("test2"));

        String fullExample = "\"poi\"=>\"yes\", \"name\"=>\"Garden of the Gods\", \"uuid\"=>\"{82e7efa6-1295-44c7-b79c-5ae022d7b3a1}\", "
                + "\"leisure\"=>\"park\", \"hoot:status\"=>\"1\", \"error:circular\"=>\"1000\", \"hoot:review:note\"=>\"Flagged for imagery validation\", \"hoot:review:needs\"=>\"yes\", "
                + "\"hoot:review:choices:1\"=>\"{\\\"label\\\":\\\"Confirmed\\\",\\\"description\\\":\\\"You can look at the point and tell what it is (e.g. mosque or airport)\\\",\\\"changes\\\":{\\\"replaceTags\\\":{\\\"hgis:imagery_confirmed\\\":\\\"confirmed\\\",\\\"hgis:accuracy\\\":\\\"high\\\"},\\\"appendTags\\\":{\\\"source:geometry\\\":\\\"${BASEMAP_IMAGE_SOURCE}\\\",\\\"source:geometry:sensor\\\":\\\"${BASEMAP_IMAGE_SENSOR}\\\",\\\"source:geometry:date\\\":\\\"${BASEMAP_IMAGE_DATETIME}\\\",\\\"source:geometry:id\\\":\\\"${BASEMAP_IMAGE_ID}\\\"}}}\", "
                + "\"hoot:review:choices:2\"=>\"{\\\"label\\\":\\\"Assessed\\\",\\\"description\\\":\\\"The point is on a building, but you can't verify its type (e.g. hair salon).\\\",\\\"changes\\\":{\\\"replaceTags\\\":{\\\"hgis:imagery_confirmed\\\":\\\"assessed\\\",\\\"hgis:accuracy\\\":\\\"high\\\"},\\\"appendTags\\\":{\\\"source:geometry\\\":\\\"${BASEMAP_IMAGE_SOURCE}\\\",\\\"source:geometry:sensor\\\":\\\"${BASEMAP_IMAGE_SENSOR}\\\",\\\"source:geometry:date\\\":\\\"${BASEMAP_IMAGE_DATETIME}\\\",\\\"source:geometry:id\\\":\\\"${BASEMAP_IMAGE_ID}\\\"}}}\", "
                + "\"hoot:review:choices:3\"=>\"{\\\"label\\\":\\\"Reported\\\",\\\"description\\\":\\\"Imagery is pixelated or cloudy -- can not be assessed.\\\",\\\"changes\\\":{\\\"replaceTags\\\":{\\\"hgis:imagery_confirmed\\\":\\\"reported\\\"},\\\"appendTags\\\":{\\\"source:geometry\\\":\\\"${BASEMAP_IMAGE_SOURCE}\\\",\\\"source:geometry:sensor\\\":\\\"${BASEMAP_IMAGE_SENSOR}\\\",\\\"source:geometry:date\\\":\\\"${BASEMAP_IMAGE_DATETIME}\\\",\\\"source:geometry:id\\\":\\\"${BASEMAP_IMAGE_ID}\\\"}}}\"";

        Map<String, String> tags = PostgresUtils.parseTags(fullExample);
        Assert.assertNotNull(tags);
        Assert.assertEquals(11, tags.size());
        Assert.assertEquals("yes", tags.get("poi"));
        Assert.assertEquals("Flagged for imagery validation", tags.get("hoot:review:note"));

        String expected = "{\\\"label\\\":\\\"Assessed\\\",\\\"description\\\":\\\"The point is on a building, but you can't verify its type (e.g. hair salon).\\\",\\\"changes\\\":{\\\"replaceTags\\\":{\\\"hgis:imagery_confirmed\\\":\\\"assessed\\\",\\\"hgis:accuracy\\\":\\\"high\\\"},\\\"appendTags\\\":{\\\"source:geometry\\\":\\\"${BASEMAP_IMAGE_SOURCE}\\\",\\\"source:geometry:sensor\\\":\\\"${BASEMAP_IMAGE_SENSOR}\\\",\\\"source:geometry:date\\\":\\\"${BASEMAP_IMAGE_DATETIME}\\\",\\\"source:geometry:id\\\":\\\"${BASEMAP_IMAGE_ID}\\\"}}}";

        Assert.assertEquals(expected, tags.get("hoot:review:choices:2"));
    }
}
