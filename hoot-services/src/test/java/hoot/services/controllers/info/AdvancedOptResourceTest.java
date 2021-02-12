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
 * @copyright Copyright (C) 2015, 2016, 2017, 2019, 2021 Maxar (http://www.maxar.com/)
 */
package hoot.services.controllers.info;

import static org.junit.Assert.assertThat;

import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import org.hamcrest.CoreMatchers;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import hoot.services.UnitTest;
import hoot.services.jerseyframework.HootServicesJerseyTestAbstract;


public class AdvancedOptResourceTest extends HootServicesJerseyTestAbstract {

    @Test
    @Category(UnitTest.class)
    public void testGetOptionsWhenConflationOptions() throws Exception {
        Response responseData =
                target("/advancedopts/getoptions")
                    .queryParam("conftype", "conflationOptions")
                    .request(MediaType.APPLICATION_JSON)
                    .get();

        // BuildingKeepMoreComplexGeometryWhenAutoMerging is one of the settings that Attribute Conflation customizes. That's unlikely to change.
        String jsonStr = responseData.readEntity(String.class);
         assertThat(jsonStr, CoreMatchers.containsString("BuildingKeepMoreComplexGeometryWhenAutoMerging"));
    }

    @Test
    @Category(UnitTest.class)
    public void testGetOptionsWhenHoot2() throws Exception {
        Response responseData =
                target("/advancedopts/getoptions")
                    .queryParam("conftype", "hoot2")
                    .request(MediaType.APPLICATION_JSON)
                    .get();

        String jsonStr = responseData.readEntity(String.class);
         assertThat(jsonStr, CoreMatchers.containsString("Roads"));
    }

    @Test
    @Category(UnitTest.class)
    public void testGetOptionsWhenConflationTypeReference() throws Exception {
        Response responseData =
                target("/advancedopts/getoptions")
                        .queryParam("conftype", "reference")
                        .request(MediaType.APPLICATION_JSON)
                        .get();

        String jsonStr = responseData.readEntity(String.class);
        assertThat(jsonStr, CoreMatchers.containsString("reference_conflation_options"));
    }

    @Test
    @Category(UnitTest.class)
    public void testGetOptionsWhenConflationTypeAttribute() throws Exception {
        Response responseData =
                target("/advancedopts/getoptions")
                        .queryParam("conftype", "attribute")
                        .request(MediaType.APPLICATION_JSON)
                        .get();

        String jsonStr = responseData.readEntity(String.class);
        assertThat(jsonStr, CoreMatchers.containsString("attribute_conflation_options"));
    }

    @Test
    @Category(UnitTest.class)
    public void testGetOptionsWhenConflationTypeHorizontal() throws Exception {
        Response responseData =
                target("/advancedopts/getoptions")
                        .queryParam("conftype", "horizontal")
                        .request(MediaType.APPLICATION_JSON)
                        .get();

        String jsonStr = responseData.readEntity(String.class);
        assertThat(jsonStr, CoreMatchers.containsString("horizontal_conflation_options"));
    }

    @Test
    @Category(UnitTest.class)
    public void testGetOptionsWhenConflationTypeAdvanced() throws Exception {
        Response responseData =
                target("/advancedopts/getoptions")
                        .queryParam("conftype", "custom")
                        .request(MediaType.APPLICATION_JSON)
                        .get();

        String jsonStr = responseData.readEntity(String.class);
        assertThat(jsonStr, CoreMatchers.containsString("hoot_cleaning_options"));
    }
}
