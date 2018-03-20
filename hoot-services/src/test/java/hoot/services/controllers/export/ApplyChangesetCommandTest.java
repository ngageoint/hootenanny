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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.export;


import static hoot.services.HootProperties.OSMAPI_DB_URL;
import static hoot.services.HootProperties.TEMP_OUTPUT_PATH;
import static org.junit.Assert.*;

import java.io.File;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.UUID;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import org.springframework.test.context.support.AnnotationConfigContextLoader;
import org.springframework.transaction.annotation.Transactional;

import hoot.services.jerseyframework.HootServicesSpringTestConfig;
import hoot.services.utils.DbUtils;
import hoot.services.utils.MapUtils;


@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(classes = HootServicesSpringTestConfig.class, loader = AnnotationConfigContextLoader.class)
@Transactional
public class ApplyChangesetCommandTest {

    @Test
    public void testOSMAPIDBApplyChangesetCommand() {
        String jobId = UUID.randomUUID().toString();
        String debugLevel = "error";
        Class<?> caller = this.getClass();

        long userId = MapUtils.insertUser();
        long mapId = MapUtils.insertMap(userId);
        String aoi = "-104.8192,38.8162,-104.6926,38.9181";

        ExportParams exportParams = new ExportParams();
        exportParams.setInput("map-with-id-" + mapId);
        exportParams.setOutputName("output");
        exportParams.setTextStatus(true);
        exportParams.setAppend(false);
        exportParams.setInputType("file");
        exportParams.setOutputType("shp");
        exportParams.setBounds(aoi);

        //timestamp of the form: "yyyy-MM-dd hh:mm:ss.zzz" used to prevent writing conflicted data
        String conflictTimestamp = "2017-04-18 14:00:01.234";
        Map<String, String> tags = new HashMap<>();
        tags.put("osm_api_db_export_time", conflictTimestamp);
        DbUtils.updateMapsTableTags(tags, mapId);

        ApplyChangesetCommand exportCommand = new ApplyChangesetCommand(jobId, exportParams, debugLevel, caller);

        List<String> options = exportCommand.getCommonExportHootOptions();
        List<String> hootOptions = new LinkedList<>();
        options.forEach(option -> { hootOptions.add("-D"); hootOptions.add(option); });

        assertEquals(jobId, exportCommand.getJobId());
        assertEquals(true, exportCommand.getTrackable());
        assertNotNull(exportCommand.getSubstitutionMap());
        assertNotNull(exportCommand.getWorkDir());
        assertNotNull(exportCommand.getCommand());

        String expectedCommand = "hoot changeset-apply --${DEBUG_LEVEL} ${HOOT_OPTIONS} " +
                        "${SQL_CHANGESET_PATH} ${TARGET_DATABASE_URL} ${CONFLICT_AOI} ${CONFLICT_TIMESTAMP}";
        assertEquals(expectedCommand, exportCommand.getCommand());

        assertTrue(exportCommand.getSubstitutionMap().containsKey("DEBUG_LEVEL"));
        assertEquals(debugLevel, exportCommand.getSubstitutionMap().get("DEBUG_LEVEL"));

        assertTrue(exportCommand.getSubstitutionMap().containsKey("HOOT_OPTIONS"));
        assertEquals(hootOptions, exportCommand.getSubstitutionMap().get("HOOT_OPTIONS"));

        String sqlChangesetPath = new File(new File(TEMP_OUTPUT_PATH, jobId),
                "changeset-" + jobId + ".osc.sql").getAbsolutePath();
        assertTrue(exportCommand.getSubstitutionMap().containsKey("SQL_CHANGESET_PATH"));
        assertEquals(sqlChangesetPath, exportCommand.getSubstitutionMap().get("SQL_CHANGESET_PATH"));

        assertTrue(exportCommand.getSubstitutionMap().containsKey("TARGET_DATABASE_URL"));
        assertEquals(OSMAPI_DB_URL, exportCommand.getSubstitutionMap().get("TARGET_DATABASE_URL"));

        assertTrue(exportCommand.getSubstitutionMap().containsKey("CONFLICT_AOI"));
        assertEquals(aoi, exportCommand.getSubstitutionMap().get("CONFLICT_AOI"));

        assertTrue(exportCommand.getSubstitutionMap().containsKey("CONFLICT_TIMESTAMP"));
        assertEquals(conflictTimestamp, exportCommand.getSubstitutionMap().get("CONFLICT_TIMESTAMP"));
    }
}