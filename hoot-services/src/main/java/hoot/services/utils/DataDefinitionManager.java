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
package hoot.services.utils;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;


public final class DataDefinitionManager {
    private static final Logger logger = LoggerFactory.getLogger(DataDefinitionManager.class);

    private DataDefinitionManager() {
    }

    static boolean checkDbExists(String dbName) throws SQLException {
        try (Connection conn = DbUtils.createConnection()) {
            String sql = "SELECT 1 FROM pg_database WHERE datname = ?";
            try (PreparedStatement stmt = conn.prepareStatement(sql)) {
                stmt.setString(1, dbName);
                try (ResultSet rs = stmt.executeQuery()) {
                    return rs.next();
                }
            }
        }
    }

    static void createDb(String dbname) throws SQLException {
        try (Connection conn = DbUtils.createConnection()) {
            String sql = "CREATE DATABASE \"" + dbname + "\"";
            try (PreparedStatement stmt = conn.prepareStatement(sql)) {
                stmt.execute();
            }
        }
    }

    public static void deleteTables(List<String> tables, String dbname) throws SQLException {
        try (Connection conn = DbUtils.createConnection()) {
            for (String tblName : tables) {
                String sql = "DROP TABLE \"" + tblName + "\"";
                try (PreparedStatement stmt = conn.prepareStatement(sql)) {
                    stmt.execute();
                }
            }
        }
    }

    static void deleteDb(String dbname, boolean force) throws SQLException {
        try (Connection conn = DbUtils.createConnection()) {
            // TODO: re-evaluate what this if block is supposed to to.
            if (force) {
                String columnName;
                String sql = "SELECT column_name " + "FROM information_schema.columns "
                        + "WHERE table_name='pg_stat_activity' AND column_name LIKE '%pid'";

                try (PreparedStatement stmt = conn.prepareStatement(sql)) {
                    // Get the column name from the db as it's version dependent
                    try (ResultSet rs = stmt.executeQuery()) {
                        rs.next();
                        columnName = rs.getString("column_name");
                    }
                }

                String forceSql = "SELECT pg_terminate_backend(" + columnName + ") " + "FROM pg_stat_activity "
                        + "WHERE datname = ?";
                try (PreparedStatement stmt = conn.prepareStatement(forceSql)) {
                    stmt.setString(1, dbname);
                    // Get the column name from the db as it's version dependent
                    try (ResultSet rs = stmt.executeQuery()) {
                    }
                }
            }

            String sql = "DROP DATABASE \"" + dbname + "\"";
            try (PreparedStatement stmt = conn.prepareStatement(sql)) {
                stmt.executeUpdate();
            }
        }
    }

    public static List<String> getTablesList(String dbName, String filter_prefix) throws SQLException {
        List<String> tblList = new ArrayList<>();
        try (Connection conn = DbUtils.createConnection()) {
            String sql = "SELECT table_name " + "FROM information_schema.tables "
                    + "WHERE table_schema='public' AND table_name LIKE " + "'" + filter_prefix.replace('-', '_')
                    + "_%'";

            try (PreparedStatement stmt = conn.prepareStatement(sql)) {
                try (ResultSet rs = stmt.executeQuery()) {
                    while (rs.next()) {
                        // Retrieve by column name
                        tblList.add(rs.getString("table_name"));
                    }
                }
            }
        }

        return tblList;
    }

    static void createTable(String createTblSql, String dbname) throws SQLException {
        try (Connection conn = DbUtils.createConnection()) {
            try (PreparedStatement stmt = conn.prepareStatement(createTblSql)) {
                stmt.executeUpdate();
            }
        }
    }
}
