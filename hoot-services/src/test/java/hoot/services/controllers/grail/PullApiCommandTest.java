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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2021, 2022 Maxar (http://www.maxar.com/)
 */
package hoot.services.controllers.grail;

import static hoot.services.HootProperties.OVERPASS_QUERY_MAXSIZE;
import static hoot.services.HootProperties.OVERPASS_QUERY_TIMEOUT;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.fail;

import java.io.IOException;
import java.util.regex.Matcher;

import org.junit.Test;

public class PullApiCommandTest {

    @Test
    public void testGetFilterPatternDefault() throws IOException {
        String overpassql =
                "[out:json];\n" +
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
        Matcher matcher = PullApiCommand.overpassqlFilterPattern.matcher(overpassql);
        if (matcher.find()) {
            filter = matcher.group(1);
        }
        assertEquals("natural=coastline", filter);

    }

    @Test
    public void testGetFilterPatternHighway() throws IOException {
        String overpassql =
                "[out:json];\n" +
                "(\n" +
                "nwr[\"highway\"]({{bbox}});\n" +
                ");\n" +
                "(._;>;);\n" +
                "out meta;";
        String filter = null;
        Matcher matcher = PullApiCommand.overpassqlFilterPattern.matcher(overpassql);
        if (matcher.find()) {
            filter = matcher.group(1);
        }
        assertEquals("\"highway\"", filter);

    }

    @Test
    public void testGetFilterPatternHighwayBuilding() throws IOException {
        String overpassql =
                "[out:json];\n" +
                "(\n" +
                "nwr[~\"^(highway|building)$\"~\".\"]({{bbox}})\n" +
                ");\n" +
                "(._;>;);\n" +
                "out meta;";
        String filter = null;
        Matcher matcher = PullApiCommand.overpassqlFilterPattern.matcher(overpassql);
        if (matcher.find()) {
            filter = matcher.group(1);
        }
        assertEquals("~\"^(highway|building)$\"~\".\"", filter);

    }

    @Test
    public void testGetFilterPatternTimeout() throws IOException {
        String overpassql =
                "[out:json][timeout:25];\n" +
                "(\n" +
                "node[amenity]({{bbox}})\n" +
                ");\n" +
                "(._;>;);\n" +
                "out meta;";
        String filter = null;
        Matcher matcher = PullApiCommand.overpassqlFilterPattern.matcher(overpassql);
        if (matcher.find()) {
            filter = matcher.group(1);
        }
        assertEquals("amenity", filter);

    }

    @Test
    public void testGetFilterPatternFence() throws IOException {
        String overpassql =
                "[out:json][timeout:25];\n" +
                "(\n" +
                "nwr[\"barrier\"=\"fence\"]({{bbox}})\n" +
                ");\n" +
                "(._;>;);\n" +
                "out meta;";
        String filter = null;
        Matcher matcher = PullApiCommand.overpassqlFilterPattern.matcher(overpassql);
        if (matcher.find()) {
            filter = matcher.group(1);
        }
        assertEquals("\"barrier\"=\"fence\"", filter);

    }

    @Test
    public void testReplaceFormatTimeoutMaxsize() throws IOException {
        String remainder = "\n" +
                "(\n" +
                "nwr[\"barrier\"=\"fence\"]({{bbox}})\n" +
                ");\n" +
                "(._;>;);\n" +
                "out meta;";
        String expected = "[out:csv(::count, ::\"count:nodes\", ::\"count:ways\", ::\"count:relations\")][maxsize:536870912][timeout:180];" + remainder;
        String replacer = String.format("[out:csv(::count, ::\"count:nodes\", ::\"count:ways\", ::\"count:relations\")][maxsize:%s][timeout:%s];",
                OVERPASS_QUERY_MAXSIZE, OVERPASS_QUERY_TIMEOUT);
        String overpassql1 = "[out:json][timeout:25];" + remainder;
        String overpassql2 = "[out:json][mazsize:2048];" + remainder;
        String overpassql3 = "[out:json][mazsize:2048][timeout:25];" + remainder;
        String overpassql4 = "[out:json][timeout:25][mazsize:2048];" + remainder;
        String overpassql5 = "[out:json];" + remainder;

        assertEquals(expected, overpassql5.replaceFirst(PullApiCommand.overpassqlFormatPattern, replacer));
        assertEquals(expected, overpassql1.replaceFirst(PullApiCommand.overpassqlFormatPattern, replacer));
        assertEquals(expected, overpassql2.replaceFirst(PullApiCommand.overpassqlFormatPattern, replacer));
        assertEquals(expected, overpassql3.replaceFirst(PullApiCommand.overpassqlFormatPattern, replacer));
        assertEquals(expected, overpassql4.replaceFirst(PullApiCommand.overpassqlFormatPattern, replacer));

    }

}
