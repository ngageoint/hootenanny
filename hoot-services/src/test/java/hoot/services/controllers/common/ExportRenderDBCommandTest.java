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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.common;


import static hoot.services.HootProperties.HOOTAPI_DB_URL;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

import java.util.UUID;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import org.springframework.test.context.support.AnnotationConfigContextLoader;
import org.springframework.transaction.annotation.Transactional;

import hoot.services.jerseyframework.HootServicesSpringTestConfig;
import hoot.services.utils.MapUtils;


@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(classes = HootServicesSpringTestConfig.class, loader = AnnotationConfigContextLoader.class)
@Transactional
public class ExportRenderDBCommandTest {

    @Test
    public void testExportRenderDBCommandBasic() throws Exception {
        String jobId = UUID.randomUUID().toString();
        Class<?> caller = this.getClass();

        long userId = MapUtils.insertUser();
        long mapId = MapUtils.insertMap(userId);

        ExportRenderDBCommand exportRenderDBCommand = new ExportRenderDBCommand(jobId, "map-with-id-" + mapId, caller);
        exportRenderDBCommand.execute();

        assertEquals(true, exportRenderDBCommand.getTrackable());
        assertNotNull(exportRenderDBCommand.getSubstitutionMap());
        assertNotNull(exportRenderDBCommand.getWorkDir());
        assertNotNull(exportRenderDBCommand.getCommand());

        String expectedCommand = "${SCRIPT} ${MAP_ID} ${HOOTAPI_DB_URL}";
        assertEquals(expectedCommand, exportRenderDBCommand.getCommand());

        assertTrue(exportRenderDBCommand.getSubstitutionMap().containsKey("SCRIPT"));
        assertEquals(String.valueOf(mapId), exportRenderDBCommand.getSubstitutionMap().get("MAP_ID"));

        assertTrue(exportRenderDBCommand.getSubstitutionMap().containsKey("HOOTAPI_DB_URL"));
        assertEquals(HOOTAPI_DB_URL, exportRenderDBCommand.getSubstitutionMap().get("HOOTAPI_DB_URL"));
    }
}