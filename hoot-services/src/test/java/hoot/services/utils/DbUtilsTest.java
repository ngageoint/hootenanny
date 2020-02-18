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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.utils;

import static hoot.services.utils.MapUtils.insertMap;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import java.sql.SQLException;
import java.sql.Timestamp;
import java.text.DateFormat;
import java.util.Calendar;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.stream.Collectors;

import org.apache.commons.io.IOUtils;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.junit.Before;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.ApplicationContext;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import org.springframework.test.context.support.AnnotationConfigContextLoader;
import org.springframework.transaction.annotation.Transactional;

import hoot.services.ApplicationContextUtils;
import hoot.services.UnitTest;
import hoot.services.jerseyframework.HootServicesSpringTestConfig;
import hoot.services.models.db.Maps;
import hoot.services.models.osm.MapLayer;


@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(classes = HootServicesSpringTestConfig.class, loader = AnnotationConfigContextLoader.class)
public class DbUtilsTest {

    @Autowired
    private JdbcTemplate jdbcTemplate;

    @Autowired
    private ApplicationContext applicationContext;

    @Before
    public void setAppContext() {
        new ApplicationContextUtils().setApplicationContext(applicationContext);
    }

    @Test
    @Category(UnitTest.class)
    @Transactional
    public void testDeleteTables() throws Exception {
        MapUtils.cleanupTestUsers();

        long userId = MapUtils.insertUser();
        long mapId = insertMap(userId);

        assertTrue(DbUtils.userExists(userId));
        assertTrue(DbUtils.mapExists(String.valueOf(mapId)));

        DbUtils.deleteMapRelatedTablesByMapId(mapId);
        DbUtils.deleteMap(mapId);

        assertFalse(DbUtils.mapExists(String.valueOf(mapId)));

        MapUtils.deleteUser(userId);
        assertFalse(DbUtils.userExists(userId));
   }


    public boolean checkForDependents(long mapId) throws SQLException {
        return DbUtils.getMapTableSeqCount(mapId) == 10;
    }

    @Test
    @Category(UnitTest.class)
    @Transactional
    public void testUpdateMapsTableTags() throws Exception {
        long userId = MapUtils.insertUser();
        long mapId = insertMap(userId);
        JSONParser parser = new JSONParser();
        Map<String, String> tags = new HashMap<>();
        String k1 = "input1";
        String k2 = "input2";
        String v1 = "layer1";
        String v2 = "layer2";
        tags.put(k1, v1);
        tags.put(k2, v2);
        // Test tag set
        long result = DbUtils.updateMapsTableTags(tags, mapId);
        assertTrue(result > -1);
        Map<String, String> checkTags = DbUtils.getMapsTableTags(mapId);
        assertTrue(checkTags.containsKey(k1));
        assertTrue(checkTags.containsKey(k2));
        assertEquals(v1, checkTags.get(k1));
        assertEquals(v2, checkTags.get(k2));

        // Test tag append
        Map<String, String> tagsAppend = new HashMap<>();
        String k3 = "key";
        String v3 = "value";
        tagsAppend.put(k3, v3);
        result = DbUtils.updateMapsTableTags(tagsAppend, mapId);
        assertTrue(result > -1);
        checkTags = DbUtils.getMapsTableTags(mapId);
        assertTrue(checkTags.containsKey(k1));
        assertTrue(checkTags.containsKey(k2));
        assertTrue(checkTags.containsKey(k3));
        assertEquals(v1, checkTags.get(k1));
        assertEquals(v2, checkTags.get(k2));
        assertEquals(v3, checkTags.get(k3));

        // Test tag update
        Map<String, String> tagsUpdate = new HashMap<>();
        String k4 = "key";
        String v4 = "change";
        tagsUpdate.put(k4, v4);
        result = DbUtils.updateMapsTableTags(tagsUpdate, mapId);
        assertTrue(result > -1);
        checkTags = DbUtils.getMapsTableTags(mapId);
        assertTrue(checkTags.containsKey(k1));
        assertTrue(checkTags.containsKey(k2));
        assertTrue(checkTags.containsKey(k4));
        assertEquals(v1, checkTags.get(k1));
        assertEquals(v2, checkTags.get(k2));
        assertEquals(v4, checkTags.get(k4));

        // Test json tag value
        Map<String, String> tagsJson = new HashMap<>();
        String k5 = "params";
        String v5 = "{\"INPUT1\":\"4835\",\"INPUT2\":\"4836\",\"OUTPUT_NAME\":\"Merged_525_stats\",\"CONFLATION_TYPE\":\"Reference\",\"GENERATE_REPORT\":\"false\",\"TIME_STAMP\":\"1453777469448\",\"REFERENCE_LAYER\":\"1\",\"AUTO_TUNNING\":\"false\",\"ADV_OPTIONS\":\"-D \\\"map.cleaner.transforms=hoot::ReprojectToPlanarOp;hoot::DuplicateWayRemover;hoot::SuperfluousWayRemover;hoot::IntersectionSplitter;hoot::UnlikelyIntersectionRemover;hoot::DualHighwaySplitter;hoot::ImpliedDividedMarker;hoot::DuplicateNameRemover;hoot::SmallHighwayMerger;hoot::RemoveEmptyAreasVisitor;hoot::RemoveDuplicateAreaVisitor;hoot::NoInformationElementRemover\\\" -D \\\"small.way.merger.threshold=15\\\" -D \\\"unify.optimizer.time.limit=30\\\" -D \\\"ogr.split.o2s=false\\\" -D \\\"ogr.tds.add.fcsubtype=true\\\" -D \\\"ogr.tds.structure=true\\\" -D \\\"duplicate.name.case.sensitive=true\\\" -D \\\"conflate.match.highway.classifier=hoot::HighwayRfClassifier\\\" -D \\\"match.creators=hoot::HighwayMatchCreator;hoot::BuildingMatchCreator;hoot::ScriptMatchCreator,PoiGeneric.js;hoot::ScriptMatchCreator,LinearWaterway.js\\\" -D \\\"merger.creators=hoot::HighwaySnapMergerCreator;hoot::BuildingMergerCreator;hoot::ScriptMergerCreator\\\" -D \\\"search.radius.highway=-1\\\" -D \\\"highway.matcher.heading.delta=5.0\\\" -D \\\"highway.matcher.max.angle=60\\\" -D \\\"way.merger.min.split.size=5\\\" -D \\\"conflate.enable.old.roads=false\\\" -D \\\"way.subline.matcher=hoot::MaximalNearestSublineMatcher\\\" -D \\\"waterway.angle.sample.distance=20.0\\\" -D \\\"waterway.matcher.heading.delta=150.0\\\" -D \\\"waterway.auto.calc.search.radius=true\\\" -D \\\"search.radius.waterway=-1\\\" -D \\\"waterway.rubber.sheet.minimum.ties=5\\\" -D \\\"waterway.rubber.sheet.ref=true\\\" -D \\\"writer.include.debug.tags=false\\\"\",\"INPUT1_TYPE\":\"DB\",\"INPUT2_TYPE\":\"DB\",\"USER_EMAIL\":\"test@test.com\"}";
        tagsJson.put(k5, JsonUtils.escapeJson(v5));
        result = DbUtils.updateMapsTableTags(tagsJson, mapId);
        assertTrue(result > -1);
        checkTags = DbUtils.getMapsTableTags(mapId);
        assertTrue(checkTags.containsKey(k1));
        assertTrue(checkTags.containsKey(k2));
        assertTrue(checkTags.containsKey(k4));
        assertTrue(checkTags.containsKey(k5));
        assertEquals(v1, checkTags.get(k1));
        assertEquals(v2, checkTags.get(k2));
        assertEquals(v4, checkTags.get(k4));
        assertEquals(parser.parse(JsonUtils.escapeJson(v5).replaceAll("\\\\\"", "\"")),
                parser.parse(checkTags.get(k5).replaceAll("\\\\\"", "\"")));

        // Test that we can parse back into json
        JSONObject oParams = (JSONObject) parser.parse(checkTags.get(k5).replaceAll("\\\\\"", "\""));
        assertEquals("4835", oParams.get("INPUT1").toString());
        assertEquals("15", ((JSONObject) oParams.get("ADV_OPTIONS")).get("small.way.merger.threshold").toString());

        // Test stats tag value
        Map<String, String> tagsStats = new HashMap<>();
        String k6 = "stats";
        String v6 = IOUtils.toString(this.getClass().getResourceAsStream("conflation-stats.csv"), "UTF-8");
        tagsStats.put(k6, v6);
        result = DbUtils.updateMapsTableTags(tagsStats, mapId);
        assertTrue(result > -1);
        checkTags = DbUtils.getMapsTableTags(mapId);
        assertTrue(checkTags.containsKey(k1));
        assertTrue(checkTags.containsKey(k2));
        assertTrue(checkTags.containsKey(k4));
        assertTrue(checkTags.containsKey(k6));
        assertEquals(v1, checkTags.get(k1));
        assertEquals(v2, checkTags.get(k2));
        assertEquals(v4, checkTags.get(k4));
        assertEquals(v6, checkTags.get(k6));

        DbUtils.deleteMapRelatedTablesByMapId(mapId);
        DbUtils.deleteMap(mapId);
        MapUtils.deleteUser(userId);
    }

    @Test
    @Category(UnitTest.class)
    public void testEscapeJson() throws Exception {
        String expected = "{'INPUT1':'4835','INPUT2':'4836','OUTPUT_NAME':'Merged_525_stats'," +
                "'CONFLATION_TYPE':'Reference','GENERATE_REPORT':'false','TIME_STAMP':'1453777469448'," +
                "'REFERENCE_LAYER':'1','AUTO_TUNNING':'false','ADV_OPTIONS': {'map.cleaner.transforms': " +
                "'hoot::ReprojectToPlanarOp;hoot::DuplicateWayRemover;hoot::SuperfluousWayRemover;" +
                "hoot::IntersectionSplitter;hoot::UnlikelyIntersectionRemover;hoot::DualHighwaySplitter;" +
                "hoot::HighwayImpliedDividedMarker;hoot::DuplicateNameRemover;hoot::SmallHighwayMerger;" +
                "hoot::RemoveEmptyAreasVisitor;hoot::RemoveDuplicateAreaVisitor;" +
                "hoot::NoInformationElementRemover', 'small.way.merger.threshold': '15', " +
                "'unify.optimizer.time.limit': '30', 'ogr.split.o2s': 'false', 'ogr.tds.add.fcsubtype': 'true', " +
                "'ogr.tds.structure': 'true', 'duplicate.name.case.sensitive': 'true', " +
                "'conflate.match.highway.classifier': 'hoot::HighwayRfClassifier', 'match.creators': " +
                "'hoot::HighwayMatchCreator;hoot::BuildingMatchCreator;hoot::ScriptMatchCreator,PoiGeneric.js;" +
                "hoot::ScriptMatchCreator,LinearWaterway.js', 'merger.creators': 'hoot::HighwaySnapMergerCreator;" +
                "hoot::BuildingMergerCreator;hoot::ScriptMergerCreator', 'search.radius.highway': '-1', " +
                "'highway.matcher.heading.delta': '5.0', 'highway.matcher.max.angle': '60', " +
                "'way.merger.min.split.size': '5', 'conflate.enable.old.roads': 'false', " +
                "'way.subline.matcher': 'hoot::MaximalNearestSublineMatcher', 'waterway.angle.sample.distance': " +
                "'20.0', 'waterway.matcher.heading.delta': '150.0', 'waterway.auto.calc.search.radius': 'true', " +
                "'search.radius.waterway': '-1', 'waterway.rubber.sheet.minimum.ties': '5', " +
                "'waterway.rubber.sheet.ref': 'true', 'writer.include.debug': 'false'}," +
                "'INPUT1_TYPE':'DB','INPUT2_TYPE':'DB','USER_EMAIL':'test@test.com'}";

        String input = "{\"INPUT1\":\"4835\",\"INPUT2\":\"4836\",\"OUTPUT_NAME\":\"Merged_525_stats\"," +
                "\"CONFLATION_TYPE\":\"Reference\",\"GENERATE_REPORT\":\"false\",\"TIME_STAMP\":\"1453777469448\"," +
                "\"REFERENCE_LAYER\":\"1\",\"AUTO_TUNNING\":\"false\"," +
                "\"ADV_OPTIONS\":\"-D \\\"map.cleaner.transforms=hoot::ReprojectToPlanarOp;hoot::DuplicateWayRemover;" +
                "hoot::SuperfluousWayRemover;hoot::IntersectionSplitter;hoot::UnlikelyIntersectionRemover;" +
                "hoot::DualHighwaySplitter;hoot::HighwayImpliedDividedMarker;hoot::DuplicateNameRemover;hoot::SmallHighwayMerger;" +
                "hoot::RemoveEmptyAreasVisitor;hoot::RemoveDuplicateAreaVisitor;hoot::NoInformationElementRemover\\\" " +
                "-D \\\"small.way.merger.threshold=15\\\" -D \\\"unify.optimizer.time.limit=30\\\" -D " +
                "\\\"ogr.split.o2s=false\\\" -D \\\"ogr.tds.add.fcsubtype=true\\\" -D \\\"ogr.tds.structure=true\\\" " +
                "-D \\\"duplicate.name.case.sensitive=true\\\" -D \\\"conflate.match.highway.classifier=" +
                "hoot::HighwayRfClassifier\\\" -D \\\"match.creators=hoot::HighwayMatchCreator;hoot::" +
                "BuildingMatchCreator;hoot::ScriptMatchCreator,PoiGeneric.js;hoot::ScriptMatchCreator," +
                "LinearWaterway.js\\\" -D \\\"merger.creators=hoot::HighwaySnapMergerCreator;" +
                "hoot::BuildingMergerCreator;hoot::ScriptMergerCreator\\\" -D \\\"search.radius.highway=-1\\\" " +
                "-D \\\"highway.matcher.heading.delta=5.0\\\" -D \\\"highway.matcher.max.angle=60\\\" " +
                "-D \\\"way.merger.min.split.size=5\\\" -D \\\"conflate.enable.old.roads=false\\\" " +
                "-D \\\"way.subline.matcher=hoot::MaximalNearestSublineMatcher\\\" " +
                "-D \\\"waterway.angle.sample.distance=20.0\\\" -D \\\"waterway.matcher.heading.delta=150.0\\\" " +
                "-D \\\"waterway.auto.calc.search.radius=true\\\" -D \\\"search.radius.waterway=-1\\\" " +
                "-D \\\"waterway.rubber.sheet.minimum.ties=5\\\" -D \\\"waterway.rubber.sheet.ref=true\\\" " +
                "-D \\\"writer.include.debug=false\\\"\",\"INPUT1_TYPE\":\"DB\",\"INPUT2_TYPE\":\"DB\"," +
                "\"USER_EMAIL\":\"test@test.com\"}";

        String output = JsonUtils.escapeJson(input);

        JSONParser parser = new JSONParser();
        JSONObject exJson = (JSONObject) parser.parse(expected.replaceAll("'", "\""));
        JSONObject outJson = (JSONObject) parser.parse(output.replaceAll("\\\\\"", "\""));
        assertEquals(exJson, outJson);
    }


    @Test
    @Category(UnitTest.class)
    @Transactional
    public void testDeleteEmptyFolders() throws Exception {
        //Create a user, some folders and a map in one of the folders
        long userId = MapUtils.insertUser();
        long folderId1 = DbUtils.createFolder("empty1", 0L, userId, false);
        long folderId2 = DbUtils.createFolder("empty2", folderId1, userId, false);
        long folderId3 = DbUtils.createFolder("full3", 0L, userId, false);
        long mapId = insertMap(userId);
        DbUtils.updateFolderMapping(mapId, folderId3);

        //Confirm the map and folders were all created
        assertTrue(DbUtils.userExists(userId));
        assertTrue(DbUtils.mapExists(String.valueOf(mapId)));
        Set<Long> fids = DbUtils.getFolderIdsForUser(Long.valueOf(userId));
        assertTrue(fids.contains(folderId1));
        assertTrue(fids.contains(folderId2));
        assertTrue(fids.contains(folderId3));

        //Delete the empty folders
        DbUtils.deleteEmptyFolders();

        //Confirm emppty folders were deleted
        fids = DbUtils.getFolderIdsForUser(Long.valueOf(userId));
        assertFalse(fids.contains(folderId1));
        assertFalse(fids.contains(folderId2));
        assertTrue(fids.contains(folderId3));

        //Delete the map and resulting empty folders and user
        DbUtils.deleteMapRelatedTablesByMapId(mapId);
        DbUtils.deleteMap(mapId);
        DbUtils.deleteEmptyFolders();
        MapUtils.deleteUser(userId);

        //Confirm map and all folders (now empty) are gone
        fids = DbUtils.getFolderIdsForUser(Long.valueOf(userId));
        assertFalse(DbUtils.userExists(userId));
        assertFalse(DbUtils.mapExists(String.valueOf(mapId)));
        assertFalse(fids.contains(folderId1));
        assertFalse(fids.contains(folderId2));
        assertFalse(fids.contains(folderId3));

    }

    @Test
    @Category(UnitTest.class)
    @Transactional
    public void testGetOldMaps() throws Exception {
        //Create a user, some folders and a map in one of the folders
        long userId = MapUtils.insertUser();
        long mapIdOld = insertMap(userId);
        long mapIdNew = insertMap(userId);
        long mapIdNoLastAccessed = insertMap(userId, new Timestamp(1571457600000L));

        Map<String, String> tagsOld = new HashMap<>();
        tagsOld.put("lastAccessed", "2019-10-24T17:03:51.125Z");

        Map<String, String> tagsNew = new HashMap<>();
        DateFormat dateFormat = MapLayer.format;
        Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
        tagsNew.put("lastAccessed", dateFormat.format(now));


        DbUtils.updateMapsTableTags(tagsOld, mapIdOld);
        DbUtils.updateMapsTableTags(tagsNew, mapIdNew);


        Calendar cal = Calendar.getInstance();
        cal.add(Calendar.MONTH, -1);
        Timestamp ts = new Timestamp(cal.getTime().getTime());
        List<Maps> oldMaps = DbUtils.getStaleMaps(ts);

        List<Long> oldMapIds = oldMaps.stream().map(m -> m.getId()).collect(Collectors.toList());
        assertTrue(oldMapIds.contains(mapIdOld));
        assertTrue(oldMapIds.contains(mapIdNoLastAccessed));
        assertFalse(oldMapIds.contains(mapIdNew));

        Map<String, Long> staleMapSummary = DbUtils.getStaleMapsSummary(ts);
        assertTrue(staleMapSummary.get(userId + "::MapUtils::insertUser()") == 2);

        DbUtils.deleteMapRelatedTablesByMapId(mapIdOld);
        DbUtils.deleteMap(mapIdOld);
        DbUtils.deleteMapRelatedTablesByMapId(mapIdNew);
        DbUtils.deleteMap(mapIdNew);
        DbUtils.deleteMapRelatedTablesByMapId(mapIdNoLastAccessed);
        DbUtils.deleteMap(mapIdNoLastAccessed);
        MapUtils.deleteUser(userId);
    }
}
