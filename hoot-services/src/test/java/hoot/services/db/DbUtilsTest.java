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
package hoot.services.db;

import hoot.services.db.DbUtils;
import hoot.services.UnitTest;

import java.sql.Connection;
import java.util.HashMap;
import java.util.Map;

import org.apache.commons.io.IOUtils;

import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;

import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;

public class DbUtilsTest {

    @Test
    @Category(UnitTest.class)
    public void testUpdateMapsTableTags() throws Exception
    {
        Connection conn = DbUtils.createConnection();
        long userId = DbUtils.insertUser(conn);
        long mapId = DbUtils.insertMap(userId, conn);
        JSONParser parser = new JSONParser();
        try {
            Map<String, String> tags = new HashMap<String, String>();
            String k1 = "input1";
            String k2 = "input2";
            String v1 = "layer1";
            String v2 = "layer2";
            tags.put(k1, v1);
            tags.put(k2, v2);
            // Test tag set
            long result = DbUtils.updateMapsTableTags(tags, mapId, conn);
            Assert.assertTrue(result > -1);
            Map<String, String> checkTags = DbUtils.getMapsTableTags(mapId, conn);
            Assert.assertTrue(checkTags.containsKey(k1));
            Assert.assertTrue(checkTags.containsKey(k2));
            Assert.assertEquals(v1, checkTags.get(k1));
            Assert.assertEquals(v2, checkTags.get(k2));

            // Test tag append
            Map<String, String> tagsAppend = new HashMap<String, String>();
            String k3 = "key";
            String v3 = "value";
            tagsAppend.put(k3, v3);
            result = DbUtils.updateMapsTableTags(tagsAppend, mapId, conn);
            Assert.assertTrue(result > -1);
            checkTags = DbUtils.getMapsTableTags(mapId, conn);
            Assert.assertTrue(checkTags.containsKey(k1));
            Assert.assertTrue(checkTags.containsKey(k2));
            Assert.assertTrue(checkTags.containsKey(k3));
            Assert.assertEquals(v1, checkTags.get(k1));
            Assert.assertEquals(v2, checkTags.get(k2));
            Assert.assertEquals(v3, checkTags.get(k3));

            // Test tag update
            Map<String, String> tagsUpdate = new HashMap<String, String>();
            String k4 = "key";
            String v4 = "change";
            tagsUpdate.put(k4, v4);
            result = DbUtils.updateMapsTableTags(tagsUpdate, mapId, conn);
            Assert.assertTrue(result > -1);
            checkTags = DbUtils.getMapsTableTags(mapId, conn);
            Assert.assertTrue(checkTags.containsKey(k1));
            Assert.assertTrue(checkTags.containsKey(k2));
            Assert.assertTrue(checkTags.containsKey(k4));
            Assert.assertEquals(v1, checkTags.get(k1));
            Assert.assertEquals(v2, checkTags.get(k2));
            Assert.assertEquals(v4, checkTags.get(k4));

            // Test json tag value
            Map<String, String> tagsJson = new HashMap<String, String>();
            String k5 = "params";
            String v5 = "{\"INPUT1\":\"4835\",\"INPUT2\":\"4836\",\"OUTPUT_NAME\":\"Merged_525_stats\",\"CONFLATION_TYPE\":\"Reference\",\"GENERATE_REPORT\":\"false\",\"TIME_STAMP\":\"1453777469448\",\"REFERENCE_LAYER\":\"1\",\"AUTO_TUNNING\":\"false\",\"ADV_OPTIONS\":\"-D \\\"map.cleaner.transforms=hoot::ReprojectToPlanarOp;hoot::DuplicateWayRemover;hoot::SuperfluousWayRemover;hoot::IntersectionSplitter;hoot::UnlikelyIntersectionRemover;hoot::DualWaySplitter;hoot::ImpliedDividedMarker;hoot::DuplicateNameRemover;hoot::SmallWayMerger;hoot::RemoveEmptyAreasVisitor;hoot::RemoveDuplicateAreaVisitor;hoot::NoInformationElementRemover\\\" -D \\\"small.way.merger.threshold=15\\\" -D \\\"unify.optimizer.time.limit=30\\\" -D \\\"ogr.split.o2s=false\\\" -D \\\"ogr.tds.add.fcsubtype=true\\\" -D \\\"ogr.tds.structure=true\\\" -D \\\"duplicate.name.case.sensitive=true\\\" -D \\\"conflate.match.highway.classifier=hoot::HighwayRfClassifier\\\" -D \\\"match.creators=hoot::HighwayMatchCreator;hoot::BuildingMatchCreator;hoot::ScriptMatchCreator,PoiGeneric.js;hoot::ScriptMatchCreator,LinearWaterway.js\\\" -D \\\"merger.creators=hoot::HighwaySnapMergerCreator;hoot::BuildingMergerCreator;hoot::ScriptMergerCreator\\\" -D \\\"search.radius.highway=-1\\\" -D \\\"highway.matcher.heading.delta=5.0\\\" -D \\\"highway.matcher.max.angle=60\\\" -D \\\"way.merger.min.split.size=5\\\" -D \\\"conflate.enable.old.roads=false\\\" -D \\\"way.subline.matcher=hoot::MaximalNearestSublineMatcher\\\" -D \\\"waterway.angle.sample.distance=20.0\\\" -D \\\"waterway.matcher.heading.delta=150.0\\\" -D \\\"waterway.auto.calc.search.radius=true\\\" -D \\\"search.radius.waterway=-1\\\" -D \\\"waterway.rubber.sheet.minimum.ties=5\\\" -D \\\"waterway.rubber.sheet.ref=true\\\" -D \\\"osm.writer.include.debug=false\\\"\",\"INPUT1_TYPE\":\"DB\",\"INPUT2_TYPE\":\"DB\",\"USER_EMAIL\":\"test@test.com\"}";
            tagsJson.put(k5, DbUtils.escapeJson(v5));
            result = DbUtils.updateMapsTableTags(tagsJson, mapId, conn);
            Assert.assertTrue(result > -1);
            checkTags = DbUtils.getMapsTableTags(mapId, conn);
            Assert.assertTrue(checkTags.containsKey(k1));
            Assert.assertTrue(checkTags.containsKey(k2));
            Assert.assertTrue(checkTags.containsKey(k4));
            Assert.assertTrue(checkTags.containsKey(k5));
            Assert.assertEquals(v1, checkTags.get(k1));
            Assert.assertEquals(v2, checkTags.get(k2));
            Assert.assertEquals(v4, checkTags.get(k4));
            Assert.assertEquals(
              parser.parse(DbUtils.escapeJson(v5).replaceAll("\\\\\"", "\"")), 
              parser.parse(checkTags.get(k5).replaceAll("\\\\\"", "\"")));

            //Test that we can parse back into json
            JSONObject oParams = (JSONObject)parser.parse(checkTags.get(k5).replaceAll("\\\\\"", "\""));
            Assert.assertEquals("4835", oParams.get("INPUT1").toString());
            Assert.assertEquals("15", ((JSONObject)oParams.get("ADV_OPTIONS")).get("small.way.merger.threshold").toString());


            //Test stats tag value
            Map<String, String> tagsStats = new HashMap<String, String>();
            String k6 = "stats";
            String v6 = IOUtils.toString(this.getClass().getResourceAsStream("conflation-stats.csv"), "UTF-8");
            tagsStats.put(k6, v6);
            result = DbUtils.updateMapsTableTags(tagsStats, mapId, conn);
            Assert.assertTrue(result > -1);
            checkTags = DbUtils.getMapsTableTags(mapId, conn);
            Assert.assertTrue(checkTags.containsKey(k1));
            Assert.assertTrue(checkTags.containsKey(k2));
            Assert.assertTrue(checkTags.containsKey(k4));
            Assert.assertTrue(checkTags.containsKey(k6));
            Assert.assertEquals(v1, checkTags.get(k1));
            Assert.assertEquals(v2, checkTags.get(k2));
            Assert.assertEquals(v4, checkTags.get(k4));
            Assert.assertEquals(v6, checkTags.get(k6));

        } finally {
            DbUtils.deleteOSMRecord(conn, mapId);
            DbUtils.deleteUser(conn, userId);
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testEscapeJson() throws Exception
    {
        String expected = "{'INPUT1':'4835','INPUT2':'4836','OUTPUT_NAME':'Merged_525_stats','CONFLATION_TYPE':'Reference','GENERATE_REPORT':'false','TIME_STAMP':'1453777469448','REFERENCE_LAYER':'1','AUTO_TUNNING':'false','ADV_OPTIONS': {'map.cleaner.transforms': 'hoot::ReprojectToPlanarOp;hoot::DuplicateWayRemover;hoot::SuperfluousWayRemover;hoot::IntersectionSplitter;hoot::UnlikelyIntersectionRemover;hoot::DualWaySplitter;hoot::ImpliedDividedMarker;hoot::DuplicateNameRemover;hoot::SmallWayMerger;hoot::RemoveEmptyAreasVisitor;hoot::RemoveDuplicateAreaVisitor;hoot::NoInformationElementRemover', 'small.way.merger.threshold': '15', 'unify.optimizer.time.limit': '30', 'ogr.split.o2s': 'false', 'ogr.tds.add.fcsubtype': 'true', 'ogr.tds.structure': 'true', 'duplicate.name.case.sensitive': 'true', 'conflate.match.highway.classifier': 'hoot::HighwayRfClassifier', 'match.creators': 'hoot::HighwayMatchCreator;hoot::BuildingMatchCreator;hoot::ScriptMatchCreator,PoiGeneric.js;hoot::ScriptMatchCreator,LinearWaterway.js', 'merger.creators': 'hoot::HighwaySnapMergerCreator;hoot::BuildingMergerCreator;hoot::ScriptMergerCreator', 'search.radius.highway': '-1', 'highway.matcher.heading.delta': '5.0', 'highway.matcher.max.angle': '60', 'way.merger.min.split.size': '5', 'conflate.enable.old.roads': 'false', 'way.subline.matcher': 'hoot::MaximalNearestSublineMatcher', 'waterway.angle.sample.distance': '20.0', 'waterway.matcher.heading.delta': '150.0', 'waterway.auto.calc.search.radius': 'true', 'search.radius.waterway': '-1', 'waterway.rubber.sheet.minimum.ties': '5', 'waterway.rubber.sheet.ref': 'true', 'osm.writer.include.debug': 'false'},'INPUT1_TYPE':'DB','INPUT2_TYPE':'DB','USER_EMAIL':'test@test.com'}";

        String input = "{\"INPUT1\":\"4835\",\"INPUT2\":\"4836\",\"OUTPUT_NAME\":\"Merged_525_stats\",\"CONFLATION_TYPE\":\"Reference\",\"GENERATE_REPORT\":\"false\",\"TIME_STAMP\":\"1453777469448\",\"REFERENCE_LAYER\":\"1\",\"AUTO_TUNNING\":\"false\",\"ADV_OPTIONS\":\"-D \\\"map.cleaner.transforms=hoot::ReprojectToPlanarOp;hoot::DuplicateWayRemover;hoot::SuperfluousWayRemover;hoot::IntersectionSplitter;hoot::UnlikelyIntersectionRemover;hoot::DualWaySplitter;hoot::ImpliedDividedMarker;hoot::DuplicateNameRemover;hoot::SmallWayMerger;hoot::RemoveEmptyAreasVisitor;hoot::RemoveDuplicateAreaVisitor;hoot::NoInformationElementRemover\\\" -D \\\"small.way.merger.threshold=15\\\" -D \\\"unify.optimizer.time.limit=30\\\" -D \\\"ogr.split.o2s=false\\\" -D \\\"ogr.tds.add.fcsubtype=true\\\" -D \\\"ogr.tds.structure=true\\\" -D \\\"duplicate.name.case.sensitive=true\\\" -D \\\"conflate.match.highway.classifier=hoot::HighwayRfClassifier\\\" -D \\\"match.creators=hoot::HighwayMatchCreator;hoot::BuildingMatchCreator;hoot::ScriptMatchCreator,PoiGeneric.js;hoot::ScriptMatchCreator,LinearWaterway.js\\\" -D \\\"merger.creators=hoot::HighwaySnapMergerCreator;hoot::BuildingMergerCreator;hoot::ScriptMergerCreator\\\" -D \\\"search.radius.highway=-1\\\" -D \\\"highway.matcher.heading.delta=5.0\\\" -D \\\"highway.matcher.max.angle=60\\\" -D \\\"way.merger.min.split.size=5\\\" -D \\\"conflate.enable.old.roads=false\\\" -D \\\"way.subline.matcher=hoot::MaximalNearestSublineMatcher\\\" -D \\\"waterway.angle.sample.distance=20.0\\\" -D \\\"waterway.matcher.heading.delta=150.0\\\" -D \\\"waterway.auto.calc.search.radius=true\\\" -D \\\"search.radius.waterway=-1\\\" -D \\\"waterway.rubber.sheet.minimum.ties=5\\\" -D \\\"waterway.rubber.sheet.ref=true\\\" -D \\\"osm.writer.include.debug=false\\\"\",\"INPUT1_TYPE\":\"DB\",\"INPUT2_TYPE\":\"DB\",\"USER_EMAIL\":\"test@test.com\"}";

        String output = DbUtils.escapeJson(input);

        JSONParser parser = new JSONParser();
        JSONObject exJson = (JSONObject)parser.parse(expected.replaceAll("'","\""));
        JSONObject outJson = (JSONObject)parser.parse(output.replaceAll("\\\\\"", "\""));
        Assert.assertEquals(exJson, outJson);

    }
}
