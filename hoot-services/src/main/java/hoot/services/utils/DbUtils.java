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


import static hoot.services.models.db.QCurrentNodes.currentNodes;
import static hoot.services.models.db.QCurrentRelations.currentRelations;
import static hoot.services.models.db.QCurrentWays.currentWays;
import static hoot.services.models.db.QMaps.maps;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import javax.inject.Provider;

import org.apache.commons.dbcp.BasicDataSource;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.ApplicationContext;

import com.querydsl.core.types.Predicate;
import com.querydsl.core.types.dsl.BooleanExpression;
import com.querydsl.core.types.dsl.Expressions;
import com.querydsl.sql.Configuration;
import com.querydsl.sql.PostgreSQLTemplates;
import com.querydsl.sql.RelationalPathBase;
import com.querydsl.sql.SQLQueryFactory;
import com.querydsl.sql.SQLTemplates;
import com.querydsl.sql.dml.SQLDeleteClause;
import com.querydsl.sql.dml.SQLInsertClause;
import com.querydsl.sql.dml.SQLUpdateClause;
import com.querydsl.sql.spring.SpringConnectionProvider;
import com.querydsl.sql.spring.SpringExceptionTranslator;
import com.querydsl.sql.types.EnumAsObjectType;

import hoot.services.ApplicationContextUtils;
import hoot.services.models.db.CurrentNodes;
import hoot.services.models.db.CurrentRelations;
import hoot.services.models.db.CurrentWays;
import hoot.services.models.db.QReviewBookmarks;
import hoot.services.models.db.QUsers;


/**
 * General Hoot services database utilities
 */
public final class DbUtils {
    private static final Logger logger = LoggerFactory.getLogger(DbUtils.class);

    private static final SQLTemplates templates = PostgreSQLTemplates.builder().quote().build();

    public static final String TIMESTAMP_DATE_FORMAT = "YYYY-MM-dd HH:mm:ss";
    public static final String OSM_API_TIMESTAMP_FORMAT = "YYYY-MM-dd HH:mm:ss.SSS";
    public static final String TIME_STAMP_REGEX = "\\d{4}-\\d{2}-\\d{2}\\s\\d{2}:\\d{2}:\\d{2}\\.\\d+";


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

    public static Configuration getConfiguration(String mapId) {
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
     * Gets the map id list from map name
     *
     * @param mapName map name
     * @return List of map ids
     */
    public static List<Long> getMapIdsByName(String mapName) {
        return createQuery()
                .select(maps.id)
                .from(maps)
                .where(maps.displayName.eq(mapName))
                .orderBy(maps.id.asc())
                .fetch();
    }

    public static String getDisplayNameById(long mapId) {
        return createQuery()
                .select(maps.displayName)
                .from(maps)
                .where(maps.id.eq(mapId))
                .fetchFirst();
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

    // remove this. replace by calling hoot core layer delete native command

    /**
     * Drops the postgis render db created for hoot map dataset
     *
     * @param mapName
     *            map name
     */
    public static void deleteRenderDb(String mapName) {
        List<Long> mapIds = getMapIdsByName(mapName);

        try (Connection connection = getConnection()) {
            if (!mapIds.isEmpty()) {
                long mapId = mapIds.get(0);

                String currentCatalog;
                try {
                    currentCatalog = connection.getCatalog();
                }
                catch (SQLException e) {
                    throw new RuntimeException("Error retrieving current catalog name!", e);
                }

                String dbNameByMapId = currentCatalog + "_renderdb_" + mapId;
                try {
                    deleteDb(dbNameByMapId);
                }
                catch (SQLException e1) {
                    logger.warn("Error deleting {} database!  The database might not exist!  Cause: {}",
                            dbNameByMapId, e1.getMessage());

                    String dbNameByMapName = currentCatalog + "_renderdb_" + mapName;
                    try {
                        deleteDb(dbNameByMapName);
                    }
                    catch (SQLException e2) {
                        logger.warn("Couldn't delete renderdb with name {} or {}.  Cause: {}",
                                dbNameByMapId, dbNameByMapName, e2.getMessage());
                    }
                }
            }
        }
        catch (SQLException e) {
            logger.error("Error deleting renderdb for map = {}", mapName, e);
        }
    }

    // remove this. replace by calling hoot core layer delete native command

    /**
     *
     * @param mapName map name
     */
    public static void deleteOSMRecordByName(String mapName) {
        List<Long> mapIds = createQuery()
                .select(maps.id)
                .from(maps)
                .where(maps.displayName.equalsIgnoreCase(mapName))
                .fetch();

        if (!mapIds.isEmpty()) {
            Long mapId = mapIds.get(0);

            deleteMapRelatedTablesByMapId(mapId);

            createQuery()
                    .delete(maps)
                    .where(maps.displayName.eq(mapName))
                    .execute();
        }
    }
    
    /**
    *
    * @param mapName map name
    */
    public static void deleteBookmarksById(String mapName) {
        List<Long> mapIds = getMapIdsByName(mapName);

        if (!mapIds.isEmpty()) {
            long mapId = mapIds.get(0);
            createQuery()
                    .delete(QReviewBookmarks.reviewBookmarks)
                    .where(QReviewBookmarks.reviewBookmarks.mapId.eq(mapId))
                    .execute();
        }
    }

    public static long getTestUserId() {
        // there is only ever one test user
        return createQuery()
                .select(QUsers.users.id)
                .from(QUsers.users)
                .fetchFirst();
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

        List<Object> results = createQuery(mapId)
                .select(maps.tags)
                .from(maps)
                .where(maps.id.eq(mapId))
                .fetch();

        if (!results.isEmpty()) {
            Object oTag = results.get(0);
            tags = PostgresUtils.postgresObjToHStore(oTag);
        }

        return tags;
    }

    public static long batchRecords(long mapId, List<?> records, RelationalPathBase<?> t,
            List<List<BooleanExpression>> predicateslist, RecordBatchType recordBatchType, int maxRecordBatchSize) {
        logger.debug("Batch element {}...", recordBatchType);

        if (recordBatchType == RecordBatchType.INSERT) {
            SQLInsertClause insert = createQuery(mapId).insert(t);
            long nBatch = 0;
            for (int i = 0; i < records.size(); i++) {
                Object oRec = records.get(i);
                insert.populate(oRec).addBatch();
                nBatch++;

                if ((maxRecordBatchSize > -1) && (i > 0)) {
                    if ((i % maxRecordBatchSize) == 0) {
                        insert.execute();

                        insert = createQuery(mapId).insert(t);
                        nBatch = 0;
                    }
                }
            }

            if (nBatch > 0) {
                return insert.execute();
            }

            return 0;
        }
        else if (recordBatchType == RecordBatchType.UPDATE) {
            SQLUpdateClause update = createQuery(mapId).update(t);
            long nBatchUpdate = 0;
            for (int i = 0; i < records.size(); i++) {
                Object oRec = records.get(i);

                List<BooleanExpression> predicates = predicateslist.get(i);

                BooleanExpression[] params = new BooleanExpression[predicates.size()];

                for (int j = 0; j < predicates.size(); j++) {
                    params[j] = predicates.get(j);
                }

                update.populate(oRec).where((Predicate[]) params).addBatch();
                nBatchUpdate++;

                if ((maxRecordBatchSize > -1) && (i > 0)) {
                    if ((i % maxRecordBatchSize) == 0) {
                        update.execute();

                        update = createQuery(mapId).update(t);
                        nBatchUpdate = 0;
                    }
                }
            }

            if (nBatchUpdate > 0) {
                return update.execute();
            }

            return 0;
        }
        else { //(recordBatchType == RecordBatchType.DELETE)
            SQLDeleteClause delete = createQuery(mapId).delete(t);
            long nBatchDel = 0;
            for (int i = 0; i < records.size(); i++) {
                List<BooleanExpression> predicates = predicateslist.get(i);

                BooleanExpression[] params = new BooleanExpression[predicates.size()];

                for (int j = 0; j < predicates.size(); j++) {
                    params[j] = predicates.get(j);
                }

                delete.where((Predicate[]) params).addBatch();
                nBatchDel++;
                if ((maxRecordBatchSize > -1) && (i > 0)) {
                    if ((i % maxRecordBatchSize) == 0) {
                        delete.execute();

                        delete = createQuery(mapId).delete(t);
                        nBatchDel = 0;
                    }
                }
            }

            if (nBatchDel > 0) {
                return delete.execute();
            }

            return 0;
        }
    }

    public static long batchRecordsDirectWays(long mapId, List<?> records,
            RecordBatchType recordBatchType, int maxRecordBatchSize) {

        logger.debug("Batch way {}...", recordBatchType);

        if (recordBatchType == RecordBatchType.INSERT) {
            return batchRecords(mapId, records, currentWays, null, RecordBatchType.INSERT, maxRecordBatchSize);
        }
        else if (recordBatchType == RecordBatchType.UPDATE) {
            List<List<BooleanExpression>> predicateList = new LinkedList<>();
            for (Object o : records) {
                CurrentWays way = (CurrentWays) o;
                predicateList.add(Collections.singletonList(Expressions.asBoolean(currentWays.id.eq(way.getId()))));
            }

            return batchRecords(mapId, records, currentWays, predicateList, RecordBatchType.UPDATE, maxRecordBatchSize);
        }
        else { //recordBatchType == RecordBatchType.DELETE
            List<List<BooleanExpression>> predicateList = new LinkedList<>();
            for (Object o : records) {
                CurrentWays way = (CurrentWays) o;
                predicateList.add(Collections.singletonList(Expressions.asBoolean(currentWays.id.eq(way.getId()))));
            }

            return batchRecords(mapId, records, currentWays, predicateList, RecordBatchType.DELETE, maxRecordBatchSize);
        }
    }

    public static long batchRecordsDirectNodes(long mapId, List<?> records, RecordBatchType recordBatchType,
            int maxRecordBatchSize) {
        logger.debug("Batch node {}...", recordBatchType);

        if (recordBatchType == RecordBatchType.INSERT) {
            return batchRecords(mapId, records, currentNodes, null, RecordBatchType.INSERT, maxRecordBatchSize);
        }
        else if (recordBatchType == RecordBatchType.UPDATE) {
            List<List<BooleanExpression>> predicateList = new LinkedList<>();
            for (Object o : records) {
                CurrentNodes node = (CurrentNodes) o;
                predicateList.add(Collections.singletonList(Expressions.asBoolean(currentNodes.id.eq(node.getId()))));
            }

            return batchRecords(mapId, records, currentNodes, predicateList, RecordBatchType.UPDATE, maxRecordBatchSize);
        }
        else { //recordBatchType == RecordBatchType.DELETE
            List<List<BooleanExpression>> predicateList = new LinkedList<>();
            for (Object o : records) {
                CurrentNodes node = (CurrentNodes) o;
                predicateList.add(Collections.singletonList(Expressions.asBoolean(currentNodes.id.eq(node.getId()))));
            }

            return batchRecords(mapId, records, currentNodes, predicateList, RecordBatchType.DELETE, maxRecordBatchSize);
        }
    }

    public static long batchRecordsDirectRelations(long mapId, List<?> records, RecordBatchType recordBatchType,
            int maxRecordBatchSize) {
        logger.debug("Batch relation {}...", recordBatchType);

        if (recordBatchType == RecordBatchType.INSERT) {
            return batchRecords(mapId, records, currentRelations, null, RecordBatchType.INSERT, maxRecordBatchSize);
        }
        else if (recordBatchType == RecordBatchType.UPDATE) {
            List<List<BooleanExpression>> predicateList = new LinkedList<>();
            for (Object o : records) {
                CurrentRelations relation = (CurrentRelations) o;
                predicateList.add(Collections.singletonList(Expressions.asBoolean(currentRelations.id.eq(relation.getId()))));
            }

            return batchRecords(mapId, records, currentRelations, predicateList, RecordBatchType.UPDATE, maxRecordBatchSize);
        }
        else { //recordBatchType == RecordBatchType.DELETE
            List<List<BooleanExpression>> predicateList = new LinkedList<>();
            for (Object o : records) {
                CurrentRelations relation = (CurrentRelations) o;
                predicateList.add(Collections.singletonList(Expressions.asBoolean(currentRelations.id.eq(relation.getId()))));
            }

            return batchRecords(mapId, records, currentRelations, predicateList, RecordBatchType.DELETE, maxRecordBatchSize);
        }
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

    static void deleteDb(String dbName) throws SQLException {
        try (Connection conn = getConnection()) {
            // Straight SQL below. No DDL support in QueryDSL anymore. Have to do it the old-fashioned way.

            // 1) Make sure no one can connect to this database.  Requires db owner privileges to execute.

            String sql = "UPDATE pg_database SET datallowconn = 'false' WHERE datname = ?";
            try (PreparedStatement stmt = conn.prepareStatement(sql)) {
                stmt.setString(1, dbName);
                stmt.executeUpdate();
            }

            // 2) Force disconnection of all clients connected database, using pg_terminate_backend.
            //    Requires superuser privileges.

            // For Postgresql < 9.2 use: SELECT pg_terminate_backend(procpid) FROM pg_stat_activity WHERE datname = 'mydb';
            sql = "SELECT pg_terminate_backend(procpid) FROM pg_stat_activity WHERE datname = ?";
            try {
                try (PreparedStatement stmt = conn.prepareStatement(sql)) {
                    stmt.setString(1, dbName);
                    stmt.executeQuery();
                }
            }
            catch (Exception e) {
                logger.debug("Failed to execute: {}", sql, e);
            }

            // For Postgresql >= 9.2 use: SELECT pg_terminate_backend(pid) FROM pg_stat_activity WHERE datname = 'mydb';
            sql = "SELECT pg_terminate_backend(pid) FROM pg_stat_activity WHERE datname = ?";
            try (PreparedStatement stmt = conn.prepareStatement(sql)) {
                stmt.setString(1, dbName);
                stmt.executeQuery();
            }
            catch (Exception e) {
                logger.debug("Failed to execute: {}", sql, e);
            }

            // 3) Drop the database as the last step.  Requires database owner privilege.

            sql = "DROP DATABASE \"" + dbName + "\"";
            try (PreparedStatement stmt = conn.prepareStatement(sql)) {
                stmt.executeUpdate();
            }
        }
    }

    public static List<String> getTablesList(String filterPrefix) throws SQLException {
        List<String> tables = new ArrayList<>();

        try (Connection conn = getConnection()) {
            String sql = "SELECT table_name " + "" +
                    "FROM information_schema.tables " +
                    "WHERE table_schema='public' AND table_name LIKE " + "'" + filterPrefix.replace('-', '_')
                    + "_%'";

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
}
