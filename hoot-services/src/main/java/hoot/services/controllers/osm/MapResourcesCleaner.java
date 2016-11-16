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
package hoot.services.controllers.osm;


import static hoot.services.models.db.QMaps.maps;
import static hoot.services.models.db.QReviewBookmarks.reviewBookmarks;
import static hoot.services.utils.DbUtils.*;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.UUID;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import hoot.services.controllers.job.JobStatusManager;


@Service
@Transactional
public class MapResourcesCleaner {
    private static final Logger logger = LoggerFactory.getLogger(MapResourcesCleaner.class);

    @Autowired
    private JobStatusManager jobStatusManager;

    public MapResourcesCleaner() {}

    public String exec(String mapName) {
        String jobId = UUID.randomUUID().toString();

        try {
            jobStatusManager.addJob(jobId);

            deleteLayerBy(mapName);

            jobStatusManager.setComplete(jobId);
        }
        catch (Exception e) {
            jobStatusManager.setFailed(jobId, e.getMessage());
            String msg = "Error deleting layer where mapName = " +  mapName;
            throw new RuntimeException(msg, e);
        }

        return jobId;
    }

    private static void deleteLayerBy(String mapName) {
        Long mapId = getMapIdByName(mapName);

        if (mapId == null) {
            throw new IllegalArgumentException(mapName + " doesn't have a corresponding map ID associated with it!");
        }

        deleteBookmarksBy(mapId);
        deleteRenderDBBy(mapName, mapId);
        deleteOSMRecordByName(mapId);
    }

    /**
     * Drops the postgis render db created for hoot map dataset
     */
    private static void deleteRenderDBBy(String mapName, Long mapId) {

        try (Connection connection = getConnection()) {
            String catalog = connection.getCatalog();

            String dbNameByMapId = catalog + "_renderdb_" + mapId;

            // First, try to delete by mapId
            boolean deleted = deletePostgresqlDBBy(dbNameByMapId, connection);

            if (!deleted) {
                String dbNameByMapName = catalog + "_renderdb_" + mapName;

                // dbNameByMapId doesn't appear to exist.  Try deleting by mapName
                deleted = deletePostgresqlDBBy(dbNameByMapName, connection);

                if (!deleted) {
                    logger.debug("Neither {} nor {} appear to present to be deleted!", dbNameByMapId, dbNameByMapName);
                }
            }
        }
        catch (SQLException e) {
            throw new RuntimeException("Error deleting renderdb for map with name = " + mapName, e);
        }
    }

    private static boolean deletePostgresqlDBBy(String dbName, Connection connection) throws SQLException {
        // Straight SQL below. No DDL support in QueryDSL anymore. Have to do it the old-fashioned way.

        boolean deleted = false;

        try {
            // NOTE: DROP DATABASE sql call cannot be run inside of a transaction.  That's why
            // for the duration of this method we set auto commit to true to enable
            // short running transactions - one transaction per execute* call.

            if (!connection.getAutoCommit()) {
                // Enable autoCommit
                connection.setAutoCommit(true);
            }

            boolean databaseExists;

            String sql = "SELECT 1 from pg_database WHERE datname = ?";
            try (PreparedStatement stmt = connection.prepareStatement(sql)) {
                stmt.setString(1, dbName);
                try (ResultSet rs = stmt.executeQuery()) {
                    databaseExists = rs.next();
                }
            }

            if (databaseExists) {
                // 1) Make sure no one can connect to 'dbName' database.  Requires db owner privileges to execute.

                sql = "UPDATE pg_database SET datallowconn = 'false' WHERE datname = ?";
                try (PreparedStatement stmt = connection.prepareStatement(sql)) {
                    stmt.setString(1, dbName);
                    stmt.executeUpdate();
                }

                // 2) Force disconnect of all clients connected to the database, using pg_terminate_backend.
                //    Requires superuser privileges.

                String postgresqlDBVersion;  //Example: "PostgreSQL 9.2.1"

                sql = "SELECT version()";
                try (PreparedStatement stmt = connection.prepareStatement(sql)) {
                    try (ResultSet rs = stmt.executeQuery()) {
                        rs.next();
                        postgresqlDBVersion = rs.getString(1);
                    }
                }

                if (postgresqlDBVersion.substring(11, 14).compareTo("9.2") < 0) {
                    // For Postgresql < 9.2 use:
                    sql = "SELECT pg_terminate_backend(pg_stat_activity.procpid) " +
                          "FROM pg_stat_activity " +
                          "WHERE pg_stat_activity.datname = ? AND procpid <> pg_backend_pid()";
                    try (PreparedStatement stmt = connection.prepareStatement(sql)) {
                        stmt.setString(1, dbName);
                        try (ResultSet rs = stmt.executeQuery()) {
                        }
                    }
                }
                else {
                    // For Postgresql >= 9.2 use:
                    sql = "SELECT pg_terminate_backend(pg_stat_activity.pid) " +
                          "FROM pg_stat_activity " +
                          "WHERE pg_stat_activity.datname = ? AND pid <> pg_backend_pid()";
                    try (PreparedStatement stmt = connection.prepareStatement(sql)) {
                        stmt.setString(1, dbName);
                        try (ResultSet rs = stmt.executeQuery()) {
                        }
                    }
                }

                // 3) Drop the database as the last step.  Requires database owner privilege.

                sql = "DROP DATABASE \"" + dbName + "\"";
                try (PreparedStatement stmt = connection.prepareStatement(sql)) {
                    stmt.executeUpdate();
                }

                deleted = true;
            }
        }
        finally {
            // Disable auto commit
            connection.setAutoCommit(false);
        }

        return deleted;
    }

    private static void deleteOSMRecordByName(Long mapId) {
        deleteMapRelatedTablesByMapId(mapId);
        createQuery().delete(maps).where(maps.id.eq(mapId)).execute();
    }

    private static void deleteBookmarksBy(Long mapId) {
        createQuery().delete(reviewBookmarks).where(reviewBookmarks.mapId.eq(mapId)).execute();
    }
}
