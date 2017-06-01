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
 * @copyright Copyright (C) 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.utils;


import static hoot.services.models.db.QMaps.maps;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.inject.Provider;

import org.apache.commons.dbcp.BasicDataSource;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.ApplicationContext;

import com.querydsl.core.types.dsl.Expressions;
import com.querydsl.core.types.dsl.NumberPath;
import com.querydsl.core.types.dsl.StringPath;
import com.querydsl.sql.Configuration;
import com.querydsl.sql.PostgreSQLTemplates;
import com.querydsl.sql.RelationalPathBase;
import com.querydsl.sql.SQLQueryFactory;
import com.querydsl.sql.SQLTemplates;
import com.querydsl.sql.spring.SpringConnectionProvider;
import com.querydsl.sql.spring.SpringExceptionTranslator;
import com.querydsl.sql.types.EnumAsObjectType;

import hoot.services.ApplicationContextUtils;
import hoot.services.models.db.QUsers;


/**
 * General Hoot services database utilities
 */
public final class DbUtils {
    private static final Logger logger = LoggerFactory.getLogger(DbUtils.class);

    private static final SQLTemplates templates = PostgreSQLTemplates.builder().quote().build();


    private DbUtils() {}

    /**
     * The types of operations that can be performed on an OSM element from a
     * changeset upload request
     */
    public enum EntityChangeType {
        CREATE, MODIFY, DELETE
    }

    /**
     * The types of operations that can be performed when writing OSM data to
     * the services database
     */
    public enum RecordBatchType {
        INSERT, UPDATE, DELETE
    }

    public enum nwr_enum {
        node, way, relation
    }

    public static Configuration getConfiguration() {
        return getConfiguration("");
    }

    public static Configuration getConfiguration(long mapId) {
        return getConfiguration(String.valueOf(mapId));
    }

    private static Configuration getConfiguration(String mapId) {
        Configuration configuration = new Configuration(templates);
        configuration.register("current_relation_members", "member_type", new EnumAsObjectType<>(nwr_enum.class));
        configuration.setExceptionTranslator(new SpringExceptionTranslator());

        if ((mapId != null) && (!mapId.isEmpty())) {
            overrideTable(mapId, configuration);
        }

        return configuration;
    }

    private static void overrideTable(String mapId, Configuration config) {
        if (config != null) {
            config.registerTableOverride("current_relation_members", "current_relation_members_" + mapId);
            config.registerTableOverride("current_relations", "current_relations_" + mapId);
            config.registerTableOverride("current_way_nodes", "current_way_nodes_" + mapId);
            config.registerTableOverride("current_ways", "current_ways_" + mapId);
            config.registerTableOverride("current_nodes", "current_nodes_" + mapId);
            config.registerTableOverride("changesets", "changesets_" + mapId);
        }
    }

    public static Connection getConnection() throws SQLException {
        ApplicationContext applicationContext = ApplicationContextUtils.getApplicationContext();
        BasicDataSource dbcpDatasource = applicationContext.getBean("dataSource", BasicDataSource.class);
        return dbcpDatasource.getConnection();
    }

    public static SQLQueryFactory createQuery(String mapId) {
        ApplicationContext applicationContext = ApplicationContextUtils.getApplicationContext();
        BasicDataSource datasource = applicationContext.getBean("dataSource", BasicDataSource.class);
        Provider<Connection> provider = new SpringConnectionProvider(datasource);
        return new SQLQueryFactory(getConfiguration(mapId), provider);
    }

    public static SQLQueryFactory createQuery() {
        return createQuery(null);
    }

    public static SQLQueryFactory createQuery(long mapId) {
        return createQuery(String.valueOf(mapId));
    }

    /**
     * Gets the map id from map name
     *
     * @param mapName map name
     * @return map ID
     */
    public static Long getMapIdByName(String mapName) {
        return createQuery().select(maps.id).from(maps).where(maps.displayName.eq(mapName)).fetchOne();
    }

    public static String getDisplayNameById(long mapId) {
        return createQuery().select(maps.displayName).from(maps).where(maps.id.eq(mapId)).fetchOne();
    }

    /**
     *  Delete map related tables by map ID
     *
     * @param mapId map ID
     */
    public static void deleteMapRelatedTablesByMapId(long mapId) {
        List<String> tables = new ArrayList<>();

        tables.add("current_way_nodes_" + mapId);
        tables.add("current_relation_members_" + mapId);
        tables.add("current_nodes_" + mapId);
        tables.add("current_ways_" + mapId);
        tables.add("current_relations_" + mapId);
        tables.add("changesets_" + mapId);

        try {
            deleteTables(tables);
        }
        catch (SQLException e) {
            throw new RuntimeException("Error deleting map related tables by map id.  mapId = " + mapId, e);
        }
    }

    public static long getTestUserId() {
        // there is only ever one test user
        return createQuery().select(QUsers.users.id).from(QUsers.users).fetchFirst();
    }

    public static long updateMapsTableTags(Map<String, String> tags, long mapId) {
        return createQuery(mapId).update(maps)
                .where(maps.id.eq(mapId))
                .set(Collections.singletonList(maps.tags),
                     Collections.singletonList(Expressions.stringTemplate("COALESCE(tags, '') || {0}::hstore", tags)))
                .execute();
    }

    public static Map<String, String> getMapsTableTags(long mapId) {
        Map<String, String> tags = new HashMap<>();

        List<Object> results = createQuery(mapId).select(maps.tags).from(maps).where(maps.id.eq(mapId)).fetch();

        if (!results.isEmpty()) {
            Object oTag = results.get(0);
            tags = PostgresUtils.postgresObjToHStore(oTag);
        }

        return tags;
    }

    public static void deleteTables(List<String> tables) throws SQLException {
        try (Connection conn = getConnection()) {
            for (String table : tables) {
                // DDL Statement. No support in QueryDSL anymore. Have to do it the old-fashioned way.
                String sql = "DROP TABLE IF EXISTS \"" + table + "\"";
                try (PreparedStatement stmt = conn.prepareStatement(sql)) {
                    stmt.execute();
                }
            }
        }
    }

    public static List<String> getTablesList(String tablePrefix) throws SQLException {
        List<String> tables = new ArrayList<>();

        try (Connection conn = getConnection()) {
            String sql = "SELECT table_name " +
                    "FROM information_schema.tables " +
                    "WHERE table_schema='public' AND table_name LIKE " + "'" + tablePrefix.replace('-', '_') + "_%'";

            try (PreparedStatement stmt = conn.prepareStatement(sql)) {
                try (ResultSet rs = stmt.executeQuery()) {
                    while (rs.next()) {
                        // Retrieve by column name
                        tables.add(rs.getString("table_name"));
                    }
                }
            }
        }

        return tables;
    }

    /**
     * Returns table size in bytes
     */
    public static long getTableSizeInBytes(String tableName) {
        return createQuery()
                .select(Expressions.numberTemplate(Long.class, "pg_total_relation_size('" + tableName + "')"))
                .from()
                .fetchOne();
    }

    /**
     * Checks for the existence of map
     *
     * @param mapName
     * @return returns true when exists else false
     */
    public static boolean mapExists(String mapName) {
        long id = getRecordIdForInputString(mapName, maps, maps.id, maps.displayName);
        return (id > -1);
    }

    /**
     * Returns the record ID associated with the record request input string for
     * the given DAO type. First attempts to parse the request string as a
     * record ID. If that is unsuccessful, it treats the request string as a
     * record display name. This currently only supports Map and User types.
     *
     * @param input
     *            can be either a map ID or a map name
     * @return if a record ID string is passed in, it is verified and returned;
     *         if a record name string is passed in, it is verified that only
     *         one record of the requested type exists with the given name, and
     *         its ID is returned
     */
    public static long getRecordIdForInputString(String input, RelationalPathBase<?> table,
            NumberPath<Long> idField, StringPath nameField) {

        if (org.apache.commons.lang3.StringUtils.isEmpty(input)) {
            throw new IllegalArgumentException("No record exists with ID: " + input
                    + ".  Please specify a valid record.");
        }

        // Check if we can compare by ID
        if (org.apache.commons.lang3.StringUtils.isNumeric(input)) {
            logger.debug("Verifying that record with ID = {} in '{}' table has previously been created ...",
                    input, table.getTableName());

            long recordCount = createQuery()
                    .from(table)
                    .where(idField.eq(Long.valueOf(input)))
                    .fetchCount();

            if (recordCount == 0) {
                throw new IllegalArgumentException("No record exists with ID = " + input +
                        " in '" + table + "' table.  Please specify a valid record.");
            }

            if (recordCount == 1) {
                return Long.valueOf(input);
            }

            if (recordCount > 1) {
                throw new IllegalArgumentException("Multiple records exist with ID " + " = " + input
                        + " in '" + table + "' table.  Please specify a single, valid record.");
            }
        }
        else { // input wasn't parsed as a numeric ID, so let's try it as a name
            logger.debug("Verifying that record with NAME = {} in '{}' table has previously been created ...",
                    input, table.getTableName());

            // there has to be a better way to do this against the generated
            // code but haven't been able to get it to work yet
            List<Long> records = createQuery()
                    .select(idField)
                    .from(table)
                    .where(nameField.eq(input))
                    .fetch();

            if (records.isEmpty()) {
                throw new IllegalArgumentException("No record exists with NAME = " + input +
                        " in '" + table + "' table.  Please specify a valid record.");
            }

            if (records.size() == 1) {
                return records.get(0);
            }

            if (records.size() > 1) {
                throw new IllegalArgumentException("Multiple records exist with NAME " + " = " + input
                        + " in '" + table + "' table.  Please specify a single, valid record.");
            }
        }

        return -1;
    }
}
