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
 * @copyright Copyright (C) 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.grail;

import static org.junit.Assert.assertEquals;

import java.io.IOException;
import java.util.regex.Matcher;

import org.junit.Test;

public class PullApiCommandTest {

    @Test
    public void testGetFilterPatternDefault() throws IOException {
        String oql = "[out:json];\n" +
                "(node({{bbox}});<;>;)->.all;\n" +
                "(\n" +
                "   relation({{bbox}})[natural=coastline];>;\n" +
                "   way({{bbox}})[natural=coastline];<;>;\n" +
                "   relation({{bbox}})[boundary=administrative];>;\n" +
                "   way({{bbox}})[boundary=administrative];<;>;\n" +
                "   node({{bbox}})[boundary=administrative];\n" +
                ")->.exclude;\n" +
                "(.all; - .exclude;);\n" +
                "out meta;";
        String filter = null;
        Matcher matcher = PullApiCommand.oqlFilterPattern.matcher(oql);
        if (matcher.find()) {
            filter = matcher.group();
        }
        assertEquals("natural=coastline", filter);

    }

    @Test
    public void testGetFilterPatternHighway() throws IOException {
        String oql = "[out:json][bbox:{{bbox}}];\n" +
                "(\n" +
                "nwr[\"highway\"]({{bbox}});\n" +
                ");\n" +
                "(._;>;);\n" +
                "out meta;";
        String filter = null;
        Matcher matcher = PullApiCommand.oqlFilterPattern.matcher(oql);
        if (matcher.find()) {
            filter = matcher.group();
        }
        assertEquals("highway", filter);

    }

    @Test
    public void testGetFilterPatternHighwayBuilding() throws IOException {
        String oql = "[out:json];\n" +
                "(\n" +
                "nwr[~\"^(highway|building)$\"~\".\"]({{bbox}})\n" +
                ");\n" +
                "(._;>;);\n" +
                "out meta;";
        String filter = null;
        Matcher matcher = PullApiCommand.oqlFilterPattern.matcher(oql);
        if (matcher.find()) {
            filter = matcher.group();
        }
        assertEquals("~\"^(highway|building)$\"~\".\"", filter);

    }

}
