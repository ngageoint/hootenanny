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


import static hoot.services.HootProperties.DB_NAME;
import static hoot.services.models.db.QCurrentNodes.currentNodes;
import static hoot.services.models.db.QCurrentRelations.currentRelations;
import static hoot.services.models.db.QCurrentWays.currentWays;
import static hoot.services.models.db.QMaps.maps;

import java.sql.Connection;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import org.apache.commons.dbcp.BasicDataSource;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.querydsl.core.types.Expression;
import com.querydsl.core.types.Predicate;
import com.querydsl.core.types.dsl.BooleanExpression;
import com.querydsl.core.types.dsl.Expressions;
import com.querydsl.sql.Configuration;
import com.querydsl.sql.PostgreSQLTemplates;
import com.querydsl.sql.RelationalPathBase;
import com.querydsl.sql.SQLQuery;
import com.querydsl.sql.SQLTemplates;
import com.querydsl.sql.dml.SQLDeleteClause;
import com.querydsl.sql.dml.SQLInsertClause;
import com.querydsl.sql.dml.SQLUpdateClause;
import com.querydsl.sql.types.EnumAsObjectType;

import hoot.services.HootProperties;
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
    private static final BasicDataSource dbcpDatasource;

    public static final String TIMESTAMP_DATE_FORMAT = "YYYY-MM-dd HH:mm:ss";
    public static final String OSM_API_TIMESTAMP_FORMAT = "YYYY-MM-dd HH:mm:ss.SSS";
    public static final String TIME_STAMP_REGEX = "\\d{4}-\\d{2}-\\d{2}\\s\\d{2}:\\d{2}:\\d{2}\\.\\d+";

    static {
        dbcpDatasource = HootProperties.getSpringContext().getBean("dataSource", BasicDataSource.class);
    }

    private DbUtils() {
    }

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

    public static Connection createConnection() throws SQLException {
        return dbcpDatasource.getConnection();
    }

    /**
     * Gets the map id list from map name
     *
     * @param connection JDBC connection
     * @param mapName map name
     * @return List of map ids
     */
    public static List<Long> getMapIdsByName(Connection connection, String mapName) {
        return new SQLQuery<>(connection, getConfiguration())
                .select(maps.id)
                .from(maps)
                .where(maps.displayName.eq(mapName))
                .orderBy(maps.id.asc())
                .fetch();
    }

    public static String getDisplayNameById(Connection connection, long mapId) {
        return new SQLQuery<>(connection, getConfiguration())
                .select(maps.displayName)
                .from(maps)
                .where(maps.id.eq(mapId))
                .fetchFirst();
    }

    private static long getNodeCountByMapName(Connection connection, String mapName, Expression<?> table) {
        long recordCount = 0;

        List<Long> mapIds = getMapIdsByName(connection, mapName);

        for (Long mapId : mapIds) {
            recordCount += new SQLQuery<>(connection, getConfiguration(mapId.toString()))
                    .from(table)
                    .fetchCount();
        }

        return recordCount;
    }

    /**
     * Get current_nodes record count by map name
     *
     * @param connection JDBC connection
     * @param mapName map name
     * @return count of nodes record
     */
    public static long getNodesCountByName(Connection connection, String mapName) {
        return getNodeCountByMapName(connection, mapName, currentNodes);
    }

    /**
     * Get current_ways record count by map name
     *
     * @param connection JDBC connection
     * @param mapName map name
     * @return current_ways record count
     */
    public static long getWayCountByName(Connection connection, String mapName) {
        return getNodeCountByMapName(connection, mapName, currentWays);
    }

    /**
     * Get current_relations record count by map name
     *
     * @param connection JDBC connection
     * @param mapName map name
     * @return current_relations record count
     */
    public static long getRelationCountByName(Connection connection, String mapName) {
        return getNodeCountByMapName(connection, mapName, currentRelations);
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
            DataDefinitionManager.deleteTables(tables, DB_NAME);
        }
        catch (SQLException e) {
            throw new RuntimeException("Error deleting map related tables by map id.  mapId = " + mapId, e);
        }
    }

    // remove this. replace by calling hoot core layer delete native command

    /**
     * Drops the postgis render db created for hoot map dataset
     *
     * @param connection
     *            JDBC Connection
     * @param mapName
     *            map name
     */
    public static void deleteRenderDb(Connection connection, String mapName) {
        List<Long> mapIds = getMapIdsByName(connection, mapName);

        if (!mapIds.isEmpty()) {
            long mapId = mapIds.get(0);
            String dbname = null;

            try {
                dbname = connection.getCatalog() + "_renderdb_" + mapId;
            }
            catch (SQLException e) {
                throw new RuntimeException("Error deleting renderdb for map with id = " + mapId, e);
            }

            try {
                DataDefinitionManager.deleteDb(dbname, false);
            }
            catch (SQLException e1) {
                logger.warn("Error deleting {} database!", dbname, e1);

                try {
                    DataDefinitionManager.deleteDb(connection.getCatalog() + "_renderdb_" + mapName, false);
                }
                catch (SQLException e2) {
                    logger.warn("No renderdb present to delete for {} or map id {}", mapName, mapId, e2);
                }
            }
        }
    }

    // remove this. replace by calling hoot core layer delete native command

    /**
     *
     *
     * @param connection JDBC connection
     * @param mapName map name
     */
    public static void deleteOSMRecordByName(Connection connection, String mapName) {
        Configuration configuration = getConfiguration();

        List<Long> mapIds = new SQLQuery<>(connection, configuration)
                .select(maps.id)
                .from(maps)
                .where(maps.displayName.equalsIgnoreCase(mapName))
                .fetch();

        if (!mapIds.isEmpty()) {
            Long mapId = mapIds.get(0);

            deleteMapRelatedTablesByMapId(mapId);

            new SQLDeleteClause(connection, configuration, maps)
                    .where(maps.displayName.eq(mapName))
                    .execute();
        }
    }
    
    /**
    *
    * @param connection JDBC connection
    * @param mapName map name
    */
    public static void deleteBookmarksById(Connection connection, String mapName) {
        List<Long> mapIds = getMapIdsByName(connection, mapName);

        if (!mapIds.isEmpty()) {
            long mapId = mapIds.get(0);
            new SQLDeleteClause(connection, getConfiguration(), QReviewBookmarks.reviewBookmarks)
                    .where(QReviewBookmarks.reviewBookmarks.mapId.eq(mapId))
                    .execute();
        }
    }

    public static long getTestUserId(Connection conn) {
        // there is only ever one test user
        return new SQLQuery<Long>(conn, getConfiguration())
                .select(QUsers.users.id)
                .from(QUsers.users)
                .fetchFirst();
    }

    public static long updateMapsTableTags(Map<String, String> tags, long mapId, Connection connection) {
        return new SQLUpdateClause(connection, getConfiguration(mapId), maps)
                .where(maps.id.eq(mapId))
                .set(Collections.singletonList(maps.tags),
                     Collections.singletonList(Expressions.stringTemplate("COALESCE(tags, '') || {0}::hstore", tags)))
                .execute();
    }

    public static Map<String, String> getMapsTableTags(long mapId, Connection connection) {
        Map<String, String> tags = new HashMap<>();

        List<Object> results = new SQLQuery<>(connection, getConfiguration(mapId))
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
            List<List<BooleanExpression>> predicateslist, RecordBatchType recordBatchType,
            Connection conn, int maxRecordBatchSize) {
        logger.debug("Batch element {}...", recordBatchType);

        Configuration configuration = getConfiguration(mapId);

        if (recordBatchType == RecordBatchType.INSERT) {
            SQLInsertClause insert = new SQLInsertClause(conn, configuration, t);
            long nBatch = 0;
            for (int i = 0; i < records.size(); i++) {
                Object oRec = records.get(i);
                insert.populate(oRec).addBatch();
                nBatch++;

                if ((maxRecordBatchSize > -1) && (i > 0)) {
                    if ((i % maxRecordBatchSize) == 0) {
                        insert.execute();

                        insert = new SQLInsertClause(conn, configuration, t);
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
            SQLUpdateClause update = new SQLUpdateClause(conn, configuration, t);
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

                        update = new SQLUpdateClause(conn, configuration, t);
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
            SQLDeleteClause delete = new SQLDeleteClause(conn, configuration, t);
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

                        delete = new SQLDeleteClause(conn, configuration, t);
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
            RecordBatchType recordBatchType, Connection connection, int maxRecordBatchSize) throws SQLException {

        logger.debug("Batch way {}...", recordBatchType);

        if (recordBatchType == RecordBatchType.INSERT) {
            return batchRecords(mapId, records, currentWays,
                                null, RecordBatchType.INSERT, connection, maxRecordBatchSize);
        }
        else if (recordBatchType == RecordBatchType.UPDATE) {
            List<List<BooleanExpression>> predicateList = new LinkedList<>();
            for (Object o : records) {
                CurrentWays way = (CurrentWays) o;
                predicateList.add(Collections.singletonList(Expressions.asBoolean(currentWays.id.eq(way.getId()))));
            }

            return batchRecords(mapId, records, currentWays,
                                predicateList, RecordBatchType.UPDATE, connection, maxRecordBatchSize);
        }
        else { //recordBatchType == RecordBatchType.DELETE
            List<List<BooleanExpression>> predicateList = new LinkedList<>();
            for (Object o : records) {
                CurrentWays way = (CurrentWays) o;
                predicateList.add(Collections.singletonList(Expressions.asBoolean(currentWays.id.eq(way.getId()))));
            }

            return batchRecords(mapId, records, currentWays,
                                predicateList, RecordBatchType.DELETE, connection, maxRecordBatchSize);
        }
    }

    public static long batchRecordsDirectNodes(long mapId, List<?> records,
            RecordBatchType recordBatchType, Connection connection, int maxRecordBatchSize) {
        logger.debug("Batch node {}...", recordBatchType);

        if (recordBatchType == RecordBatchType.INSERT) {
            return batchRecords(mapId, records, currentNodes,
                    null, RecordBatchType.INSERT, connection, maxRecordBatchSize);
        }
        else if (recordBatchType == RecordBatchType.UPDATE) {
            List<List<BooleanExpression>> predicateList = new LinkedList<>();
            for (Object o : records) {
                CurrentNodes node = (CurrentNodes) o;
                predicateList.add(Collections.singletonList(Expressions.asBoolean(currentNodes.id.eq(node.getId()))));
            }

            return batchRecords(mapId, records, currentNodes,
                    predicateList, RecordBatchType.UPDATE, connection, maxRecordBatchSize);
        }
        else { //recordBatchType == RecordBatchType.DELETE
            List<List<BooleanExpression>> predicateList = new LinkedList<>();
            for (Object o : records) {
                CurrentNodes node = (CurrentNodes) o;
                predicateList.add(Collections.singletonList(Expressions.asBoolean(currentNodes.id.eq(node.getId()))));
            }

            return batchRecords(mapId, records, currentNodes,
                    predicateList, RecordBatchType.DELETE, connection, maxRecordBatchSize);
        }
    }

    public static long batchRecordsDirectRelations(long mapId, List<?> records,
            RecordBatchType recordBatchType, Connection connection, int maxRecordBatchSize) {
        logger.debug("Batch relation {}...", recordBatchType);

        if (recordBatchType == RecordBatchType.INSERT) {
            return batchRecords(mapId, records, currentRelations,
                    null, RecordBatchType.INSERT, connection, maxRecordBatchSize);
        }
        else if (recordBatchType == RecordBatchType.UPDATE) {
            List<List<BooleanExpression>> predicateList = new LinkedList<>();
            for (Object o : records) {
                CurrentRelations relation = (CurrentRelations) o;
                predicateList.add(Collections.singletonList(Expressions.asBoolean(currentRelations.id.eq(relation.getId()))));
            }

            return batchRecords(mapId, records, currentRelations,
                                predicateList, RecordBatchType.UPDATE, connection, maxRecordBatchSize);
        }
        else { //recordBatchType == RecordBatchType.DELETE
            List<List<BooleanExpression>> predicateList = new LinkedList<>();
            for (Object o : records) {
                CurrentRelations relation = (CurrentRelations) o;
                predicateList.add(Collections.singletonList(Expressions.asBoolean(currentRelations.id.eq(relation.getId()))));
            }

            return batchRecords(mapId, records, currentRelations,
                                predicateList, RecordBatchType.DELETE, connection, maxRecordBatchSize);
        }
    }

    /**
     * Returns table size in byte
     */
    public static long getTableSizeInBytes(String tableName) {
        try (Connection conn = createConnection()) {
            return new SQLQuery<>(conn, getConfiguration())
                    .select(Expressions.numberTemplate(Long.class, "pg_total_relation_size('" + tableName + "')"))
                    .from()
                    .fetchOne();
        }
        catch (SQLException e) {
            String msg = "Error retrieving table size in bytes of " + tableName + " table!";
            throw new RuntimeException(msg, e);
        }
    }
}
