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

        deleteBookmarksBy(mapId);
        deleteRenderDBBy(mapName, mapId);
        deleteOSMRecordByName(mapId);
    }

    /**
     * Drops the postgis render db created for hoot map dataset
     *
     * @param mapName
     *            map name
     */
    private static void deleteRenderDBBy(String mapName, Long mapId) {
        try (Connection connection = getConnection()) {
            String catalog;
            try {
                catalog = connection.getCatalog();
            }
            catch (SQLException e) {
                throw new RuntimeException("Error retrieving current catalog name!", e);
            }

            if (mapId != null) {
                String dbNameByMapId = catalog + "_renderdb_" + mapId;
                try {
                    deletePostgresqlDB(dbNameByMapId, connection);
                }
                catch (SQLException e1) {
                    logger.warn("Error deleting {} database by map ID!", dbNameByMapId, e1);

                    String dbNameByMapName = catalog + "_renderdb_" + mapName;
                    try {
                        deletePostgresqlDB(dbNameByMapName, connection);
                    }
                    catch (SQLException e2) {
                        logger.warn("Couldn't delete {} by map name either!", dbNameByMapName, e2);
                        throw e2;
                    }
                }
            }
            else {
                String dbNameByMapName = catalog + "_renderdb_" + mapName;
                deletePostgresqlDB(dbNameByMapName, connection);
            }
        }
        catch (SQLException e) {
            String message;
            if (mapId != null) {
                message = "Error deleting renderdb for map with ID = " + mapId;
            }
            else {
                message = "Error deleting renderdb for map with name = " + mapName;
            }

            throw new RuntimeException(message, e);
        }
    }

    private static void deletePostgresqlDB(String dbName, Connection connection) throws SQLException {
        // Straight SQL below. No DDL support in QueryDSL anymore. Have to do it the old-fashioned way.

        try {
            // NOTE: DROP DATABASE sql call cannot be run inside of a transaction.  That's why
            // for the duration of this method we set auto commit to true to enable
            // short running transactions - one transaction per execute* call.

            if (!connection.getAutoCommit()) {
                // Enable autoCommit
                connection.setAutoCommit(true);
            }

            // 1) Make sure no one can connect to 'dbName' database.  Requires db owner privileges to execute.

            String sql = "UPDATE pg_database SET datallowconn = 'false' WHERE datname = ?";
            try (PreparedStatement stmt = connection.prepareStatement(sql)) {
                stmt.setString(1, dbName);
                stmt.executeUpdate();
            }

            // 2) Force disconnection of all clients connected database, using pg_terminate_backend.
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
                        "WHERE pg_stat_activity.datname = ? AND procpid <> pg_backend_pid();";
                try (PreparedStatement stmt = connection.prepareStatement(sql)) {
                    stmt.setString(1, dbName);
                    try (ResultSet rs = stmt.executeQuery()){}
                }
            }
            else {
                // For Postgresql >= 9.2 use:
                sql = "SELECT pg_terminate_backend(pg_stat_activity.pid) " +
                        "FROM pg_stat_activity " +
                        "WHERE pg_stat_activity.datname = ? AND pid <> pg_backend_pid()";
                try (PreparedStatement stmt = connection.prepareStatement(sql)) {
                    stmt.setString(1, dbName);
                    try (ResultSet rs = stmt.executeQuery()){}
                }
            }

            // 3) Drop the database as the last step.  Requires database owner privilege.

            sql = "DROP DATABASE \"" + dbName + "\"";
            try (PreparedStatement stmt = connection.prepareStatement(sql)) {
                stmt.executeUpdate();
            }
        }
        finally {
            // Disable auto commit
            connection.setAutoCommit(false);
        }
    }

    private static void deleteOSMRecordByName(Long mapId) {
        if (mapId != null) {
            deleteMapRelatedTablesByMapId(mapId);
            createQuery().delete(maps).where(maps.id.eq(mapId)).execute();
        }
    }

    private static void deleteBookmarksBy(Long mapId) {
        if (mapId != null) {
            createQuery().delete(reviewBookmarks).where(reviewBookmarks.mapId.eq(mapId)).execute();
        }
    }
}
