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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.utils;


import static hoot.services.HootProperties.DB_NAME;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Types;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.commons.dbcp.BasicDataSource;
import org.postgresql.util.PGobject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.mysema.query.sql.Configuration;
import com.mysema.query.sql.PostgresTemplates;
import com.mysema.query.sql.RelationalPathBase;
import com.mysema.query.sql.SQLQuery;
import com.mysema.query.sql.SQLTemplates;
import com.mysema.query.sql.dml.SQLDeleteClause;
import com.mysema.query.sql.dml.SQLInsertClause;
import com.mysema.query.sql.dml.SQLUpdateClause;
import com.mysema.query.sql.types.EnumAsObjectType;
import com.mysema.query.types.Predicate;
import com.mysema.query.types.expr.BooleanExpression;

import hoot.services.HootProperties;
import hoot.services.db2.CurrentNodes;
import hoot.services.db2.CurrentRelations;
import hoot.services.db2.CurrentWays;
import hoot.services.db2.QCurrentNodes;
import hoot.services.db2.QCurrentRelations;
import hoot.services.db2.QCurrentWays;
import hoot.services.db2.QMaps;
import hoot.services.db2.QReviewBookmarks;
import hoot.services.db2.QUsers;
import hoot.services.models.osm.Element.ElementType;


/**
 * General Hoot services database utilities
 */
public final class DbUtils {
    private static final Logger logger = LoggerFactory.getLogger(DbUtils.class);

    private static final SQLTemplates templates = PostgresTemplates.builder().quote().build();
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
     * @param conn
     * @param mapName
     * @return List of map ids
     */
    public static List<Long> getMapIdsByName(Connection conn, String mapName) {
        SQLQuery query = new SQLQuery(conn, getConfiguration());

        List<Long> mapIds = query
                .from(QMaps.maps)
                .where(QMaps.maps.displayName.eq(mapName))
                .orderBy(QMaps.maps.id.asc())
                .list(QMaps.maps.id);

        return mapIds;
    }

    public static String getDisplayNameById(Connection conn, long mapId) {
        SQLQuery query = new SQLQuery(conn, getConfiguration());

        String displayName = query
                .from(QMaps.maps)
                .where(QMaps.maps.id.eq(mapId))
                .uniqueResult(QMaps.maps.displayName);

        return displayName;
    }

    // TODO: use reflection to get these three element count queries down to one

    /**
     * Get current_nodes record count by map name
     *
     * @param conn
     * @param mapName
     * @return count of nodes record
     */
    public static long getNodesCountByName(Connection conn, String mapName) {
        long recordCount = 0;

        List<Long> mapIds = getMapIdsByName(conn, mapName);

        for (Long mapId : mapIds) {
            SQLQuery query = new SQLQuery(conn, getConfiguration(mapId.toString()));
            recordCount += query.from(QCurrentNodes.currentNodes).count();
        }

        return recordCount;
    }

    /**
     * Get current_ways record count by map name
     *
     * @param conn
     * @param mapName
     * @return current_ways record count
     */
    public static long getWayCountByName(Connection conn, String mapName) {
        long recordCount = 0;

        List<Long> mapIds = getMapIdsByName(conn, mapName);

        for (Long mapId : mapIds) {
            SQLQuery query = new SQLQuery(conn, getConfiguration(mapId.toString()));
            recordCount += query.from(QCurrentWays.currentWays).count();
        }

        return recordCount;
    }

    /**
     * Get current_relations record count by map name
     *
     * @param conn
     * @param mapName
     * @return current_relations record count
     */
    public static long getRelationCountByName(Connection conn, String mapName) {
        long recordCount = 0;

        List<Long> mapIds = getMapIdsByName(conn, mapName);

        for (Long mapId : mapIds) {
            SQLQuery query= new SQLQuery(conn, getConfiguration(mapId.toString()));
            recordCount += query.from(QCurrentRelations.currentRelations).count();
        }

        return recordCount;
    }

    /**
     *
     *
     * @param mapId
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
     * @param conn
     *            JDBC Connection
     * @param mapName
     *            String
     */
    public static void deleteRenderDb(Connection conn, String mapName) {
        List<Long> mapIds = getMapIdsByName(conn, mapName);

        if (!mapIds.isEmpty()) {
            long mapId = mapIds.get(0);
            String dbname = null;
            try {
                dbname = conn.getCatalog() + "_renderdb_" + mapId;
            }
            catch (SQLException e) {
                throw new RuntimeException("Error deleting renderdb for map with id = " + mapId, e);
            }

            try {
                DataDefinitionManager.deleteDb(dbname, false);
            }
            catch (SQLException e) {
                logger.warn("Error deleting {} database!", dbname, e);
                try {
                    DataDefinitionManager.deleteDb(conn.getCatalog() + "_renderdb_" + mapName, false);
                }
                catch (SQLException ee) {
                    logger.warn("No renderdb present to delete for {} or map id {}", mapName, mapId, ee);
                }
            }
        }
    }

    // remove this. replace by calling hoot core layer delete native command

    /**
     *
     *
     * @param conn
     * @param mapName
     */
    public static void deleteOSMRecordByName(Connection conn, String mapName) {
        Configuration configuration = getConfiguration();

        List<Long> mapIds = new SQLQuery(conn, configuration)
                .from(QMaps.maps)
                .where(QMaps.maps.displayName.equalsIgnoreCase(mapName))
                .list(QMaps.maps.id);

        if (!mapIds.isEmpty()) {
            Long mapId = mapIds.get(0);

            deleteMapRelatedTablesByMapId(mapId);

            new SQLDeleteClause(conn, configuration, QMaps.maps)
                    .where(QMaps.maps.displayName.eq(mapName))
                    .execute();
        }
    }
    
    /**
    *
    * @param conn
    * @param mapName
    */
    public static void deleteBookmarksById(Connection conn, String mapName) {
        List<Long> mapIds = getMapIdsByName(conn, mapName);

        if (!mapIds.isEmpty()) {
            long mapId = mapIds.get(0);
            new SQLDeleteClause(conn, getConfiguration(), QReviewBookmarks.reviewBookmarks)
                    .where(QReviewBookmarks.reviewBookmarks.mapId.eq(mapId))
                    .execute();
        }
    }

    public static long getTestUserId(Connection conn) {
        SQLQuery query = new SQLQuery(conn, getConfiguration());
        // there is only ever one test user
        return query.from(QUsers.users).singleResult(QUsers.users.id);
    }

    public static long updateMapsTableTags(Map<String, String> tags, long mapId, Connection conn) {
        long execResult;
        String sql = "update maps set tags = COALESCE(tags, '') || ?::hstore " + "where id=?";

        try (PreparedStatement ps = conn.prepareStatement(sql)) {
            String hstoreStr = "";
            for (Map.Entry<String, String> pairs : tags.entrySet()) {
                if (!hstoreStr.isEmpty()) {
                    hstoreStr += ",";
                }
                hstoreStr += "\"" + pairs.getKey() + "\"=>\"" + pairs.getValue() + "\"";
            }
            ps.setObject(1, hstoreStr, Types.OTHER);
            ps.setLong(2, mapId);

            execResult = ps.executeUpdate();
        }
        catch (SQLException e) {
            throw new RuntimeException("Error trying to update map's tags.  mapId = " + mapId, e);
        }

        return execResult;
    }

    public static Map<String, String> getMapsTableTags(long mapId, Connection conn) {
        Map<String, String> tags = new HashMap<>();
        QMaps mp = QMaps.maps;

        List<Object> res = new SQLQuery(conn, getConfiguration(mapId)).from(mp).where(mp.id.eq(mapId)).list(mp.tags);

        if (!res.isEmpty()) {
            Object oTag = res.get(0);
            tags = PostgresUtils.postgresObjToHStore((PGobject) oTag);
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
            RecordBatchType recordBatchType, Connection conn, int maxRecordBatchSize) throws SQLException {
        logger.debug("Batch way {}...", recordBatchType);

        long updateCount = 0;
        int count = 0;

        if (recordBatchType == RecordBatchType.INSERT) {
            String sql = "insert into current_ways_" + mapId
                    + " (id, changeset_id, \"timestamp\", visible, version, tags) "
                    + "values (?, ?, ?, ?, ?, ?)";

            try (PreparedStatement ps = conn.prepareStatement(sql)) {
                for (Object o : records) {
                    CurrentWays way = (CurrentWays) o;

                    ps.setLong(1, way.getId());
                    ps.setLong(2, way.getChangesetId());
                    ps.setTimestamp(3, way.getTimestamp());
                    ps.setBoolean(4, way.getVisible());
                    ps.setLong(5, way.getVersion());

                    Map<String, String> tags = (Map<String, String>) way.getTags();

                    String hstoreStr = "";
                    for (Map.Entry<String, String> pairs : tags.entrySet()) {
                        if (!hstoreStr.isEmpty()) {
                            hstoreStr += ",";
                        }
                        hstoreStr += "\"" + pairs.getKey() + "\"=>\"" + pairs.getValue() + "\"";
                    }
                    ps.setObject(6, hstoreStr, Types.OTHER);
                    ps.addBatch();

                    if (maxRecordBatchSize > -1) {
                        if ((++count % maxRecordBatchSize) == 0) {
                            updateCount += ps.executeBatch().length;
                        }
                    }
                }

                updateCount += ps.executeBatch().length;
            }
        }
        else if (recordBatchType == RecordBatchType.UPDATE) {
            String sql = "update current_ways_" + mapId
                    + " set changeset_id=?, visible=?, \"timestamp\"=?, version=?, tags=? " + "where id=?";

            try (PreparedStatement ps = conn.prepareStatement(sql)) {
                for (Object o : records) {
                    CurrentWays way = (CurrentWays) o;

                    ps.setLong(1, way.getChangesetId());
                    ps.setBoolean(2, way.getVisible());
                    ps.setTimestamp(3, way.getTimestamp());
                    ps.setLong(4, way.getVersion());

                    Map<String, String> tags = (Map<String, String>) way.getTags();

                    String hstoreStr = "";
                    for (Map.Entry<String, String> pairs : tags.entrySet()) {
                        if (!hstoreStr.isEmpty()) {
                            hstoreStr += ",";
                        }
                        hstoreStr += "\"" + pairs.getKey() + "\"=>\"" + pairs.getValue() + "\"";
                    }

                    ps.setObject(5, hstoreStr, Types.OTHER);
                    ps.setLong(6, way.getId());
                    ps.addBatch();

                    if (maxRecordBatchSize > -1) {
                        if ((++count % maxRecordBatchSize) == 0) {
                            updateCount += ps.executeBatch().length;
                        }
                    }
                }

                updateCount += ps.executeBatch().length;
            }
        }
        else { //if (recordBatchType == RecordBatchType.DELETE) {
            String sql = "delete from current_ways_" + mapId + " where id=?";

            try (PreparedStatement ps = conn.prepareStatement(sql)) {
                for (Object o : records) {
                    CurrentWays way = (CurrentWays) o;

                    ps.setLong(1, way.getId());
                    ps.addBatch();

                    if (maxRecordBatchSize > -1) {
                        if ((++count % maxRecordBatchSize) == 0) {
                            updateCount += ps.executeBatch().length;
                        }
                    }
                }

                updateCount += ps.executeBatch().length;
            }
        }

        return updateCount;
    }

    public static long batchRecordsDirectNodes(long mapId, List<?> records,
            RecordBatchType recordBatchType, Connection conn, int maxRecordBatchSize) throws SQLException {
        logger.debug("Batch node {}...", recordBatchType);

        long updateCount = 0;
        int count = 0;

        if (recordBatchType == RecordBatchType.INSERT) {
            String sql = "insert into current_nodes_" + mapId + " (id, latitude, "
                    + "longitude, changeset_id, visible, \"timestamp\", tile, version, tags) "
                    + "values (?, ?, ?, ?, ?, ?, ?, ?, ?)";

            try (PreparedStatement ps = conn.prepareStatement(sql)) {
                for (Object o : records) {
                    CurrentNodes node = (CurrentNodes) o;
                    ps.setLong(1, node.getId());
                    ps.setDouble(2, node.getLatitude());
                    ps.setDouble(3, node.getLongitude());
                    ps.setLong(4, node.getChangesetId());
                    ps.setBoolean(5, node.getVisible());
                    ps.setTimestamp(6, node.getTimestamp());
                    ps.setLong(7, node.getTile());
                    ps.setLong(8, node.getVersion());

                    Map<String, String> tags = (Map<String, String>) node.getTags();

                    String hstoreStr = "";
                    for (Map.Entry<String, String> pairs : tags.entrySet()) {
                        if (!hstoreStr.isEmpty()) {
                            hstoreStr += ",";
                        }
                        hstoreStr += "\"" + pairs.getKey() + "\"=>\"" + pairs.getValue() + "\"";
                    }
                    ps.setObject(9, hstoreStr, Types.OTHER);
                    ps.addBatch();

                    if (maxRecordBatchSize > -1) {
                        if ((++count % maxRecordBatchSize) == 0) {
                            updateCount += ps.executeBatch().length;
                        }
                    }
                }

                updateCount += ps.executeBatch().length;
            }
        }
        else if (recordBatchType == RecordBatchType.UPDATE) {
            String sql = "update current_nodes_" + mapId + " set  latitude=?, "
                    + "longitude=?, changeset_id=?, visible=?, \"timestamp\"=?, tile=?, version=?, tags=? "
                    + "where id=?";

            try (PreparedStatement ps = conn.prepareStatement(sql)) {
                for (Object o : records) {
                    CurrentNodes node = (CurrentNodes) o;

                    ps.setDouble(1, node.getLatitude());
                    ps.setDouble(2, node.getLongitude());
                    ps.setLong(3, node.getChangesetId());
                    ps.setBoolean(4, node.getVisible());
                    ps.setTimestamp(5, node.getTimestamp());
                    ps.setLong(6, node.getTile());
                    ps.setLong(7, node.getVersion());

                    Map<String, String> tags = (Map<String, String>) node.getTags();

                    String hstoreStr = "";
                    for (Map.Entry<String, String> pairs : tags.entrySet()) {
                        if (!hstoreStr.isEmpty()) {
                            hstoreStr += ",";
                        }
                        hstoreStr += "\"" + pairs.getKey() + "\"=>\"" + pairs.getValue() + "\"";
                    }

                    ps.setObject(8, hstoreStr, Types.OTHER);
                    ps.setLong(9, node.getId());
                    ps.addBatch();

                    if (maxRecordBatchSize > -1) {
                        if ((++count % maxRecordBatchSize) == 0) {
                            updateCount += ps.executeBatch().length;
                            ps.clearBatch();
                        }
                    }
                }

                updateCount += ps.executeBatch().length;
            }
        }
        else { //if (recordBatchType == RecordBatchType.DELETE) {
            String sql = "delete from current_nodes_" + mapId + " where id=?";

            try (PreparedStatement ps = conn.prepareStatement(sql)) {
                for (Object o : records) {
                    CurrentNodes node = (CurrentNodes) o;

                    ps.setLong(1, node.getId());
                    ps.addBatch();

                    if (maxRecordBatchSize > -1) {
                        if ((++count % maxRecordBatchSize) == 0) {
                            updateCount += ps.executeBatch().length;
                            ps.clearBatch();
                        }
                    }
                }

                updateCount += ps.executeBatch().length;
            }
        }

        return updateCount;
    }

    public static long batchRecordsDirectRelations(long mapId, List<?> records,
            RecordBatchType recordBatchType, Connection conn, int maxRecordBatchSize) throws SQLException {
        logger.debug("Batch relation {}...", recordBatchType);

        long updateCount = 0;
        int count = 0;

        if (recordBatchType == RecordBatchType.INSERT) {
            String sql = "insert into current_relations_" + mapId +
                    " (id, changeset_id, \"timestamp\", visible, version, tags) " +
                    "values (?, ?, ?, ?, ?, ?)";

            try (PreparedStatement ps = conn.prepareStatement(sql)) {
                for (Object o : records) {
                    CurrentRelations rel = (CurrentRelations) o;

                    ps.setLong(1, rel.getId());
                    ps.setLong(2, rel.getChangesetId());
                    ps.setTimestamp(3, rel.getTimestamp());
                    ps.setBoolean(4, rel.getVisible());
                    ps.setLong(5, rel.getVersion());

                    Map<String, String> tags = (Map<String, String>) rel.getTags();

                    String hstoreStr = "";
                    for (Map.Entry<String, String> pairs : tags.entrySet()) {
                        if (!hstoreStr.isEmpty()) {
                            hstoreStr += ",";
                        }
                        hstoreStr += "\"" + pairs.getKey() + "\"=>\"" + pairs.getValue() + "\"";
                    }
                    ps.setObject(6, hstoreStr, Types.OTHER);
                    ps.addBatch();

                    if (maxRecordBatchSize > -1) {
                        if ((++count % maxRecordBatchSize) == 0) {
                            updateCount += ps.executeBatch().length;
                        }
                    }
                }

                updateCount += ps.executeBatch().length;
            }
        }
        else if (recordBatchType == RecordBatchType.UPDATE) {
            String sql = "update current_relations_" + mapId
                    + " set changeset_id=?, visible=?, \"timestamp\"=?, version=?, tags=? " + "where id=?";

            try (PreparedStatement ps = conn.prepareStatement(sql)) {
                for (Object o : records) {
                    CurrentRelations rel = (CurrentRelations) o;

                    ps.setLong(1, rel.getChangesetId());
                    ps.setBoolean(2, rel.getVisible());
                    ps.setTimestamp(3, rel.getTimestamp());
                    ps.setLong(4, rel.getVersion());

                    Map<String, String> tags = (Map<String, String>) rel.getTags();

                    String hstoreStr = "";
                    for (Map.Entry<String, String> pairs : tags.entrySet()) {
                        if (!hstoreStr.isEmpty()) {
                            hstoreStr += ",";
                        }
                        hstoreStr += "\"" + pairs.getKey() + "\"=>\"" + pairs.getValue() + "\"";
                    }
                    ps.setObject(5, hstoreStr, Types.OTHER);
                    ps.setLong(6, rel.getId());

                    ps.addBatch();

                    if (maxRecordBatchSize > -1) {
                        if ((++count % maxRecordBatchSize) == 0) {
                            updateCount += ps.executeBatch().length;
                        }
                    }
                }

                updateCount += ps.executeBatch().length;
            }
        }
        else { //if (recordBatchType == RecordBatchType.DELETE) {
            String sql = "delete from current_relations_" + mapId + " where id=?";

            try (PreparedStatement ps = conn.prepareStatement(sql)) {
                for (Object o : records) {
                    CurrentRelations rel = (CurrentRelations) o;

                    ps.setLong(1, rel.getId());
                    ps.addBatch();

                    if (maxRecordBatchSize > -1) {
                        if ((++count % maxRecordBatchSize) == 0) {
                            updateCount += ps.executeBatch().length;
                        }
                    }
                }

                updateCount += ps.executeBatch().length;
            }
        }

        return updateCount;
    }

    /**
     * //TODO: change back to original element generic code
     */
    public static Object resultToObj(ResultSet rs, ElementType elementType) throws SQLException {
        if (elementType == ElementType.Node) {
            CurrentNodes nodes = new CurrentNodes();
            nodes.setId(rs.getLong("id"));
            nodes.setLatitude(rs.getDouble("latitude"));
            nodes.setLongitude(rs.getDouble("longitude"));
            nodes.setChangesetId(rs.getLong("changeset_id"));
            nodes.setVisible(rs.getBoolean("visible"));
            nodes.setTimestamp(rs.getTimestamp("timestamp"));
            nodes.setTile(rs.getLong("tile"));
            nodes.setVersion(rs.getLong("version"));
            nodes.setTags(rs.getObject("tags"));
            return nodes;
        }

        if (elementType == ElementType.Way) {
            CurrentWays ways = new CurrentWays();
            ways.setId(rs.getLong("id"));
            ways.setChangesetId(rs.getLong("changeset_id"));
            ways.setVisible(rs.getBoolean("visible"));
            ways.setTimestamp(rs.getTimestamp("timestamp"));
            ways.setVersion(rs.getLong("version"));
            ways.setTags(rs.getObject("tags"));
            return ways;
        }

        if (elementType == ElementType.Relation) {
            CurrentRelations rel = new CurrentRelations();
            rel.setId(rs.getLong("id"));
            rel.setChangesetId(rs.getLong("changeset_id"));
            rel.setVisible(rs.getBoolean("visible"));
            rel.setTimestamp(rs.getTimestamp("timestamp"));
            rel.setVersion(rs.getLong("version"));
            rel.setTags(rs.getObject("tags"));
            return rel;
        }

        return null;
    }
}
