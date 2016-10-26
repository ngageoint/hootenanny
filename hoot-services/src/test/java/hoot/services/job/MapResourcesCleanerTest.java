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
package hoot.services.job;


import static hoot.services.utils.DbUtils.getConnection;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;

import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.test.context.ActiveProfiles;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import org.springframework.test.context.support.AnnotationConfigContextLoader;
import org.springframework.transaction.annotation.Transactional;

import hoot.services.UnitTest;
import hoot.services.testsupport.HootServicesSpringTestConfig;
import hoot.services.testsupport.MapUtils;


@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(classes = HootServicesSpringTestConfig.class, loader = AnnotationConfigContextLoader.class)
@ActiveProfiles("test")
@Transactional
public class MapResourcesCleanerTest {

    @Autowired
    private MapResourcesCleaner mapResourcesCleaner;

    @Test
    @Category(UnitTest.class)
    public void testDeleteDatabaseByName() throws Exception {
        try (Connection connection = getConnection()) {
            // Enable autoCommit
            connection.setAutoCommit(true);

            String currentCatalog = connection.getCatalog();
            String dbName = currentCatalog + "_renderdb_" + "test9999";

            boolean databaseExists;
            String sql = "SELECT 1 from pg_database WHERE datname='" + dbName + "'";
            try (PreparedStatement stmt = connection.prepareStatement(sql)) {
                try (ResultSet rs = stmt.executeQuery()) {
                    databaseExists = rs.next();
                }
            }

            if (databaseExists) {
                sql = "DROP DATABASE \"" + dbName + "\"";
                try (PreparedStatement stmt = connection.prepareStatement(sql)) {
                    stmt.executeUpdate();
                }
            }

            sql = "CREATE DATABASE \"" + dbName + "\"";
            try (PreparedStatement stmt = connection.prepareStatement(sql)) {
                stmt.executeUpdate();
            }

            mapResourcesCleaner.exec("test9999");

            sql = "SELECT 1 from pg_database WHERE datname='" + dbName + "'";
            try (PreparedStatement stmt = connection.prepareStatement(sql)) {
                try (ResultSet rs = stmt.executeQuery()) {
                    databaseExists = rs.next();
                }
            }

            assertFalse(databaseExists);
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testDeleteDatabaseById() throws Exception {
        Long mapId;

        try (Connection connection = getConnection()) {
            long userId = MapUtils.insertUser();
            mapId = MapUtils.insertMap(userId);

            // Enable autoCommit
            connection.setAutoCommit(true);

            String currentCatalog = connection.getCatalog();
            String dbName = currentCatalog + "_renderdb_" + mapId;

            boolean databaseExists;
            String sql = "SELECT 1 from pg_database WHERE datname='" + dbName + "'";
            try (PreparedStatement stmt = connection.prepareStatement(sql)) {
                try (ResultSet rs = stmt.executeQuery()) {
                    databaseExists = rs.next();
                }
            }

            if (databaseExists) {
                sql = "DROP DATABASE \"" + dbName + "\"";
                try (PreparedStatement stmt = connection.prepareStatement(sql)) {
                    stmt.executeUpdate();
                }
            }

            sql = "CREATE DATABASE \"" + dbName + "\"";
            try (PreparedStatement stmt = connection.prepareStatement(sql)) {
                stmt.executeUpdate();
            }

            mapResourcesCleaner.exec("map-with-id-" + mapId);

            sql = "SELECT 1 from pg_database WHERE datname='" + dbName + "'";
            try (PreparedStatement stmt = connection.prepareStatement(sql)) {
                try (ResultSet rs = stmt.executeQuery()) {
                    databaseExists = rs.next();
                }
            }

            assertFalse(databaseExists);
        }
    }

    @Test(expected = RuntimeException.class)
    @Category(UnitTest.class)
    public void testDeleteRenderDBByWrongNameWithException() throws Exception {
        try {
            mapResourcesCleaner.exec("non-existing-db");
        }
        catch (RuntimeException re) {
            assertTrue(re.getMessage().startsWith("Error deleting layer where mapName ="));
            throw re;
        }
    }
}