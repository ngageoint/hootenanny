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
package hoot.services.db;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.sql.Timestamp;
import java.sql.Types;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.support.ClassPathXmlApplicationContext;

import com.mysema.query.sql.Configuration;
import com.mysema.query.sql.PostgresTemplates;
import com.mysema.query.sql.SQLQuery;
import com.mysema.query.sql.SQLTemplates;
import com.mysema.query.sql.dml.SQLDeleteClause;
import com.mysema.query.sql.dml.SQLInsertClause;
import com.mysema.query.sql.dml.SQLUpdateClause;
import com.mysema.query.sql.types.EnumAsObjectType;
import com.mysema.query.types.expr.BooleanExpression;
import com.mysema.query.types.expr.NumberExpression;
import com.mysema.query.types.template.NumberTemplate;

import hoot.services.HootProperties;
import hoot.services.db.postgres.PostgresUtils;
import hoot.services.db2.CurrentNodes;
import hoot.services.db2.CurrentRelations;
import hoot.services.db2.CurrentWays;
import hoot.services.db2.JobStatus;
import hoot.services.db2.QChangesets;
import hoot.services.db2.QCurrentNodes;
import hoot.services.db2.QCurrentRelationMembers;
import hoot.services.db2.QCurrentRelations;
import hoot.services.db2.QCurrentWayNodes;
import hoot.services.db2.QCurrentWays;
import hoot.services.db2.QJobStatus;
import hoot.services.db2.QMaps;
import hoot.services.db2.QUsers;
import hoot.services.models.osm.Element.ElementType;


/**
 * General Hoot services database utilities
 */
public class DbUtils {
    private static final Logger log = LoggerFactory.getLogger(DbUtils.class);

    public static final String TIMESTAMP_DATE_FORMAT = "YYYY-MM-dd HH:mm:ss";

    public static final String TIME_STAMP_REGEX = "\\d{4}-\\d{2}-\\d{2}\\s\\d{2}:\\d{2}:\\d{2}\\.\\d+";

    private static SQLTemplates templates = null;

    private static org.apache.commons.dbcp.BasicDataSource dbcp_datasource = null;
    private static ClassPathXmlApplicationContext appContext = null;

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

    /**
     * Returns a record batch type given the corresponding entity change type
     *
     * @param entityChangeType
     *            an entity change type
     * @return a record batch type
     */
    public static RecordBatchType recordBatchTypeForEntityChangeType(final EntityChangeType entityChangeType) {
        switch (entityChangeType) {
            case CREATE:

                return RecordBatchType.INSERT;

            case MODIFY:

                return RecordBatchType.UPDATE;

            case DELETE:

                return RecordBatchType.DELETE;

            default:

                assert (false);
                return null;
        }
    }

    public static Configuration getConfiguration() {
        return getConfiguration("");
    }

    public static Configuration getConfiguration(final long mapId) {
        return getConfiguration("" + mapId);
    }

    public static Configuration getConfiguration(String mapId) {
        if (templates == null) {
            templates = PostgresTemplates.builder().quote().build();
        }

        Configuration configuration = new Configuration(templates);
        configuration.register("current_relation_members", "member_type",
                new EnumAsObjectType<nwr_enum>(nwr_enum.class));
        if (mapId != null && mapId.length() > 0) {
            overrideTable(mapId, configuration);
        }
        return configuration;
    }

    public static void overrideTable(String mapId, Configuration config) {
        if (config != null) {
            config.registerTableOverride("current_relation_members", "current_relation_members_" + mapId);
            config.registerTableOverride("current_relations", "current_relations_" + mapId);
            config.registerTableOverride("current_way_nodes", "current_way_nodes_" + mapId);
            config.registerTableOverride("current_ways", "current_ways_" + mapId);
            config.registerTableOverride("current_nodes", "current_nodes_" + mapId);
            config.registerTableOverride("changesets", "changesets_" + mapId);
        }
    }

    public static Connection createConnection() {
        try {
            if (dbcp_datasource == null) {
                appContext = new ClassPathXmlApplicationContext(new String[] { "db/spring-database.xml" });
                dbcp_datasource = appContext.getBean("dataSource", org.apache.commons.dbcp.BasicDataSource.class);
            }

            return dbcp_datasource.getConnection();
        }
        catch (Exception e) {
            log.error(e.getMessage());
        }
        return null;
    }

    public static boolean closeConnection(Connection conn) throws Exception {
        if (conn != null) {
            if (conn.isClosed()) {
                return true;
            }
            try {
                conn.close();
                return true;
            }
            catch (Exception e) {
                throw e;
            }
        }
        return false;
    }

    /**
     * Determines whether any OSM element records exist in the services database
     *
     * @param conn
     *            JDBC Connection
     * @return true if any OSM element records exist in the services database;
     *         false otherwise
     * @throws Exception
     */
    public static boolean elementDataExistsInServicesDb(Connection conn) throws Exception {
        long recordCount = 0;

        QCurrentNodes currentNodes = QCurrentNodes.currentNodes;
        QCurrentWayNodes currentWayNodes = QCurrentWayNodes.currentWayNodes;
        QCurrentWays currentWays = QCurrentWays.currentWays;
        QCurrentRelationMembers currentRelationMembers = QCurrentRelationMembers.currentRelationMembers;
        QCurrentRelations currentRelations = QCurrentRelations.currentRelations;

        SQLQuery query = new SQLQuery(conn, DbUtils.getConfiguration());

        recordCount += query.from(currentNodes).count();
        recordCount += query.from(currentWayNodes).count();
        recordCount += query.from(currentWays).count();
        recordCount += query.from(currentRelationMembers).count();
        recordCount += query.from(currentRelations).count();

        return recordCount > 0;
    }

    /**
     * Gets the map id list from map name
     *
     * @param conn
     * @param mapName
     * @return List of map ids
     * @throws Exception
     */
    public static List<Long> getMapIdsByName(Connection conn, String mapName) throws Exception {
        QMaps maps = QMaps.maps;
        SQLQuery query = new SQLQuery(conn, DbUtils.getConfiguration());

        final List<Long> mapIds = query.from(maps).where(maps.displayName.eq(mapName)).orderBy(maps.id.asc())
                .list(maps.id);

        return mapIds;
    }

    public static String getDisplayNameById(final Connection conn, final long mapId) throws Exception {
        QMaps maps = QMaps.maps;
        SQLQuery query = new SQLQuery(conn, DbUtils.getConfiguration());

        String displayName = query.from(maps).where(maps.id.eq(mapId)).uniqueResult(maps.displayName);

        return displayName;
    }

    // TODO: use reflection to get these three element count queries down to one

    /**
     * Get current_nodes record count by map name
     *
     * @param conn
     * @param mapName
     * @return count of nodes record
     * @throws Exception
     */
    public static long getNodesCountByName(Connection conn, String mapName) throws Exception {
        long recordCount = 0;
        try {
            List<Long> mapIds = getMapIdsByName(conn, mapName);

            for (int i = 0; i < mapIds.size(); i++) {
                Long mapId = mapIds.get(i);
                QCurrentNodes currentNodes = QCurrentNodes.currentNodes;
                SQLQuery query = new SQLQuery(conn, DbUtils.getConfiguration(mapId.toString()));

                recordCount += query.from(currentNodes).count();
            }
        }
        catch (Exception e) {
            String msg = "Error countin Nodes record.  ";

            throw new Exception(msg);
        }
        return recordCount;
    }

    /**
     * Get current_ways record count by map name
     *
     * @param conn
     * @param mapName
     * @return current_ways record count
     * @throws Exception
     */
    public static long getWayCountByName(Connection conn, String mapName) throws Exception {
        long recordCount = 0;
        try {
            List<Long> mapIds = getMapIdsByName(conn, mapName);

            for (int i = 0; i < mapIds.size(); i++) {
                Long mapId = mapIds.get(i);
                QCurrentWays currentWays = QCurrentWays.currentWays;
                SQLQuery query = new SQLQuery(conn, DbUtils.getConfiguration(mapId.toString()));

                recordCount += query.from(currentWays).count();
            }
        }
        catch (Exception e) {
            String msg = "Error countin Way record.  ";

            throw new Exception(msg);
        }
        return recordCount;
    }

    /**
     * Get current_relations record count by map name
     *
     * @param conn
     * @param mapName
     * @return current_relations record count
     * @throws Exception
     */
    public static long getRelationCountByName(Connection conn, String mapName) throws Exception {
        long recordCount = 0;
        try {
            List<Long> mapIds = getMapIdsByName(conn, mapName);

            for (int i = 0; i < mapIds.size(); i++) {
                Long mapId = mapIds.get(i);
                QCurrentRelations currentRelations = QCurrentRelations.currentRelations;
                SQLQuery query = new SQLQuery(conn, DbUtils.getConfiguration(mapId.toString()));

                recordCount += query.from(currentRelations).count();
            }
        }
        catch (Exception e) {
            String msg = "Error countin Relations record.  ";

            throw new Exception(msg);
        }
        return recordCount;
    }

    /**
     *
     *
     * @param mapId
     * @throws Exception
     */
    public static void deleteMapRelatedTablesByMapId(final long mapId) throws Exception {

        String dbname = HootProperties.getProperty("dbName");
        DataDefinitionManager ddm = new DataDefinitionManager();
        List<String> tables = new ArrayList<String>();
        tables.add("current_way_nodes_" + mapId);
        tables.add("current_relation_members_" + mapId);
        tables.add("current_nodes_" + mapId);
        tables.add("current_ways_" + mapId);
        tables.add("current_relations_" + mapId);
        tables.add("changesets_" + mapId);

        ddm.deleteTables(tables, dbname);
    }

    // remove this. replace by calling hoot core layer delete native command

    /**
     *
     *
     * @param conn
     * @param mapId
     * @throws Exception
     */
    public static void deleteOSMRecord(Connection conn, Long mapId) throws Exception {
        try {
            deleteMapRelatedTablesByMapId(mapId);

            conn.setAutoCommit(false);
            Configuration configuration = getConfiguration();

            QMaps maps = QMaps.maps;
            new SQLDeleteClause(conn, configuration, maps).where(maps.id.eq(mapId)).execute();

            conn.commit();
        }
        catch (Exception e) {
            String msg = "Error deleting OSM record.  ";
            msg += "  " + e.getCause().getMessage();

            throw new Exception(msg);
        }
        finally {
            conn.setAutoCommit(true);
        }
    }

    /**
     * Drops the postgis render db created for hoot map dataset
     *
     * @param conn
     *            JDBC Connection
     * @param mapName
     *            String
     * @throws Exception
     */
    public static void deleteRenderDb(Connection conn, String mapName) throws Exception {
        try {
            List<Long> mapIds = getMapIdsByName(conn, mapName);
            if (mapIds.size() > 0) {
                long mapId = mapIds.get(0);
                String dbname = conn.getCatalog() + "_renderdb_" + String.valueOf(mapId);
                DataDefinitionManager ddm = new DataDefinitionManager();
                try {
                    ddm.deleteDb(dbname, false);
                }
                catch (SQLException e) {
                    try {
                        ddm.deleteDb(conn.getCatalog() + "_renderdb_" + mapName, false);
                    }
                    catch (SQLException f) {
                        log.warn(
                                "No renderdb present to delete for " + mapName + " or map id " + String.valueOf(mapId));
                    }
                }

            }
        }
        catch (Exception e) {
            String msg = "Error dropping postgis render database.  ";
            msg += "  " + e.getCause().getMessage();
            throw new Exception(msg);
        }
    }

    // remove this. replace by calling hoot core layer delete native command

    /**
     *
     *
     * @param conn
     * @param mapName
     * @throws Exception
     */
    public static void deleteOSMRecordByName(Connection conn, String mapName) throws Exception {
        Configuration configuration = getConfiguration();

        QMaps maps = QMaps.maps;
        List<Long> mapIds = new SQLQuery(conn, configuration).from(maps)
                .where(maps.displayName.equalsIgnoreCase(mapName)).list(maps.id);

        if (mapIds.size() > 0) {
            Long mapId = mapIds.get(0);
            deleteMapRelatedTablesByMapId(mapId);

            conn.setAutoCommit(false);

            // ListSubQuery<Long> res = new SQLSubQuery().from(maps)
            // .where(maps.displayName.equalsIgnoreCase(mapName)).list(maps.id);

            new SQLDeleteClause(conn, configuration, maps).where(maps.displayName.eq(mapName)).execute();

            conn.commit();
        }
    }

    /**
     * Determines whether any changeset data exists in the services database
     *
     * @param conn
     *            JDBC Connection
     * 
     * @return true if changeset data exists; false otherwise
     * @throws Exception
     */
    public static boolean changesetDataExistsInServicesDb(Connection conn) throws Exception {
        int recordCtr = 0;
        QChangesets changesets = QChangesets.changesets;
        SQLQuery query = new SQLQuery(conn, DbUtils.getConfiguration());

        recordCtr += query.from(changesets).count();

        return recordCtr > 0;
    }

    public static long getTestUserId(Connection conn) {
        QUsers users = QUsers.users;
        SQLQuery query = new SQLQuery(conn, DbUtils.getConfiguration());

        // there is only ever one test user
        return query.from(users).singleResult(users.id);
    }

    public static long insertUser(Connection conn) throws Exception {
        Long newId = (long) -1;
        NumberExpression<Long> expression = NumberTemplate.create(Long.class, "nextval('users_id_seq')");
        Configuration configuration = getConfiguration();

        SQLQuery query = new SQLQuery(conn, configuration);

        List<Long> ids = query.from().list(expression);

        if (ids != null && ids.size() > 0) {
            newId = ids.get(0);
            QUsers users = QUsers.users;

            new SQLInsertClause(conn, configuration, users).columns(users.id, users.displayName, users.email)
                    .values(newId, "user-with-id-" + newId, "user-with-id-" + newId).execute();

        }
        return newId.longValue();
    }

    public static void deleteUser(Connection conn, long userId) throws Exception {
        Configuration configuration = getConfiguration();
        QUsers users = QUsers.users;
        new SQLDeleteClause(conn, configuration, users).where(users.id.eq(userId)).execute();
    }

    /**
     *
     *
     * @param mapId
     * @throws Exception
     *             //TODO: This code needs to be changed to dynamically read in
     *             the data types from querydsl. If I make a change to the
     *             schema in liquibase, it will never be picked up unless this
     *             static code is also changed. See r6777
     */
    public static void createMap(final long mapId) throws Exception {
        try {
            String dbname = HootProperties.getProperty("dbName");

            DataDefinitionManager ddm = new DataDefinitionManager();

            // cnagesets
            String createTblSql = "CREATE TABLE changesets_" + mapId + "(id bigserial NOT NULL, "
                    + " user_id bigint NOT NULL, " + " created_at timestamp without time zone NOT NULL, "
                    + " min_lat double precision NOT NULL, " + " max_lat double precision NOT NULL, "
                    + " min_lon double precision NOT NULL, " + " max_lon double precision NOT NULL, "
                    + " closed_at timestamp without time zone NOT NULL, " + " num_changes integer NOT NULL DEFAULT 0, "
                    + " tags hstore, " + " CONSTRAINT pk_changesets_" + mapId + " PRIMARY KEY (id ), "
                    + " CONSTRAINT changesets_" + mapId + "_user_id_fkey FOREIGN KEY (user_id) "
                    + " REFERENCES users (id) MATCH SIMPLE " + " ON UPDATE NO ACTION ON DELETE NO ACTION "
                    + " ) WITH ( OIDS=FALSE );";

            ddm.createTable(createTblSql, dbname);

            // current_nodes
            createTblSql = "CREATE TABLE current_nodes_" + mapId + "(id bigserial NOT NULL, "
                    + " latitude double precision NOT NULL, " + " longitude double precision NOT NULL, "
                    + " changeset_id bigint NOT NULL, " + " visible boolean NOT NULL DEFAULT true, "
                    + " \"timestamp\" timestamp without time zone NOT NULL DEFAULT now(), " + " tile bigint NOT NULL, "
                    + " version bigint NOT NULL DEFAULT 1, " + " tags hstore, " + " CONSTRAINT current_nodes_" + mapId
                    + "_pkey" + " PRIMARY KEY (id ), " + " CONSTRAINT current_nodes_" + mapId
                    + "_changeset_id_fkey FOREIGN KEY (changeset_id) " + " REFERENCES changesets_" + mapId
                    + " (id) MATCH SIMPLE " + " ON UPDATE NO ACTION ON DELETE NO ACTION " + " ) WITH ( OIDS=FALSE );";

            ddm.createTable(createTblSql, dbname);

            // current_relation_members
            createTblSql = "CREATE TABLE current_relation_members_" + mapId + "(relation_id bigint NOT NULL, "
                    + " member_type nwr_enum NOT NULL, " + " member_id bigint NOT NULL, "
                    + " member_role character varying(255) NOT NULL, " + " sequence_id integer NOT NULL DEFAULT 0, "
                    + " CONSTRAINT current_relation_members_" + mapId
                    + "_pkey PRIMARY KEY (relation_id , member_type , member_id , member_role , sequence_id ) "
                    + " ) WITH ( OIDS=FALSE );";

            ddm.createTable(createTblSql, dbname);

            // current_relations
            createTblSql = "CREATE TABLE current_relations_" + mapId + "(" + "  id bigserial NOT NULL,"
                    + "  changeset_id bigint NOT NULL,"
                    + "  \"timestamp\" timestamp without time zone NOT NULL DEFAULT now(),"
                    + "  visible boolean NOT NULL DEFAULT true," + "  version bigint NOT NULL DEFAULT 1,"
                    + "  tags hstore," + "  CONSTRAINT current_relations_" + mapId + "_pkey PRIMARY KEY (id ),"
                    + "  CONSTRAINT current_relations_" + mapId + "_changeset_id_fkey FOREIGN KEY (changeset_id)"
                    + "      REFERENCES changesets_" + mapId + " (id) MATCH SIMPLE"
                    + "      ON UPDATE NO ACTION ON DELETE NO ACTION" + ")" + "WITH (" + "  OIDS=FALSE" + ");";
            ddm.createTable(createTblSql, dbname);

            // current_way_nodes

            createTblSql = "CREATE TABLE current_way_nodes_" + mapId + "(" + "  way_id bigint NOT NULL,"
                    + "  node_id bigint NOT NULL," + "  sequence_id bigint NOT NULL," + "  CONSTRAINT current_way_"
                    + mapId + "_nodes_pkey PRIMARY KEY (way_id , sequence_id )" + ")" + "WITH (" + "  OIDS=FALSE"
                    + ");";
            ddm.createTable(createTblSql, dbname);

            // current_ways
            createTblSql = "CREATE TABLE current_ways_" + mapId + "(" + "  id bigserial NOT NULL,"
                    + "  changeset_id bigint NOT NULL,"
                    + "  \"timestamp\" timestamp without time zone NOT NULL DEFAULT now(),"
                    + "  visible boolean NOT NULL DEFAULT true," + "  version bigint NOT NULL DEFAULT 1,"
                    + "  tags hstore," + "  CONSTRAINT current_ways_" + mapId + "_pkey PRIMARY KEY (id ),"
                    + "  CONSTRAINT current_ways_" + mapId + "_changeset_id_fkey FOREIGN KEY (changeset_id)"
                    + "      REFERENCES changesets_" + mapId + " (id) MATCH SIMPLE"
                    + "      ON UPDATE NO ACTION ON DELETE NO ACTION" + ")" + "WITH (" + "  OIDS=FALSE" + ");";

            ddm.createTable(createTblSql, dbname);

        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    public static long insertMap(final long userId, Connection conn) throws Exception {
        Long newId = (long) -1;
        NumberExpression<Long> expression = NumberTemplate.create(Long.class, "nextval('maps_id_seq')");
        Configuration configuration = getConfiguration();

        SQLQuery query = new SQLQuery(conn, configuration);

        List<Long> ids = query.from().list(expression);

        if (ids != null && ids.size() > 0) {
            newId = ids.get(0);
            QMaps maps = QMaps.maps;

            final Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());

            new SQLInsertClause(conn, configuration, maps)
                    .columns(maps.id, maps.createdAt, maps.displayName, maps.publicCol, maps.userId)
                    .values(newId, now, "map-with-id-" + newId, true, userId).execute();

        }
        DbUtils.createMap(newId.longValue());
        return newId.longValue();
    }

    /**
     *
     *
     * @param jobId
     * @param status
     * @param conn
     * @throws Exception
     */
    public static void insertJobStatus(final String jobId, final int status, Connection conn) throws Exception {
        Configuration configuration = getConfiguration();
        QJobStatus jobStatus = QJobStatus.jobStatus;

        final Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());

        new SQLInsertClause(conn, configuration, jobStatus).columns(jobStatus.jobId, jobStatus.status, jobStatus.start)
                .values(jobId, status, now).execute();
    }

    /**
     * Updates job status. If the record does not exist then creates.
     *
     * @param jobId
     * @param jobStatus
     * @param isComplete
     * @param conn
     * @throws Exception
     */
    public static void updateJobStatus(final String jobId, final int jobStatus, boolean isComplete,
            final String statusDetail, Connection conn) throws Exception {
        Configuration configuration = DbUtils.getConfiguration();

        QJobStatus jobStatusTbl = QJobStatus.jobStatus;
        SQLQuery query = new SQLQuery(conn, configuration);
        JobStatus stat = query.from(jobStatusTbl).where(jobStatusTbl.jobId.eq(jobId)).singleResult(jobStatusTbl);
        if (stat != null) {
            if (isComplete == true) {
                stat.setPercentComplete(100.0);
                stat.setEnd(new Timestamp(Calendar.getInstance().getTimeInMillis()));
            }
            stat.setStatus(jobStatus);
            if (statusDetail != null) {
                stat.setStatusDetail(statusDetail);
            }

            new SQLUpdateClause(conn, configuration, jobStatusTbl).populate(stat)
                    .where(jobStatusTbl.jobId.eq(stat.getJobId())).execute();
        }
        else {
            stat = new JobStatus();
            stat.setJobId(jobId);
            stat.setStatus(jobStatus);
            Timestamp ts = new Timestamp(Calendar.getInstance().getTimeInMillis());
            stat.setStart(ts);
            if (isComplete == true) {
                stat.setEnd(ts);
            }
            new SQLInsertClause(conn, configuration, jobStatusTbl).populate(stat).execute();
        }
    }

    /**
     * retrieves job status.
     *
     * @param jobId
     *            ID of the job
     * @param conn
     *            JDBC Connection
     * @return a numeric job status
     * @throws Exception
     */
    public static int getJobStatus(final String jobId, Connection conn) throws Exception {
        QJobStatus jobStatusTbl = QJobStatus.jobStatus;
        SQLQuery query = new SQLQuery(conn, DbUtils.getConfiguration());
        JobStatus stat = query.from(jobStatusTbl).where(jobStatusTbl.jobId.eq(jobId)).singleResult(jobStatusTbl);

        return stat.getStatus();
    }

    public static long batchRecords(final long mapId, final List<?> records,
            com.mysema.query.sql.RelationalPathBase<?> t, List<List<BooleanExpression>> predicateslist,
            final RecordBatchType recordBatchType, Connection conn, int maxRecordBatchSize) throws Exception {
        log.debug("Batch element " + recordBatchType.toString() + "...");

        try {
            Configuration configuration = getConfiguration(mapId);

            switch (recordBatchType) {
                case INSERT:

                    SQLInsertClause insert = new SQLInsertClause(conn, configuration, t);
                    long nBatch = 0;
                    for (int i = 0; i < records.size(); i++) {
                        Object oRec = records.get(i);
                        insert.populate(oRec).addBatch();
                        nBatch++;

                        if (maxRecordBatchSize > -1 && i > 0) {
                            if (i % maxRecordBatchSize == 0) {
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

                case UPDATE:

                    SQLUpdateClause update = new SQLUpdateClause(conn, configuration, t);
                    long nBatchUpdate = 0;
                    for (int i = 0; i < records.size(); i++) {
                        Object oRec = records.get(i);

                        List<BooleanExpression> predicates = predicateslist.get(i);

                        BooleanExpression[] params;
                        params = new BooleanExpression[predicates.size()];

                        for (int ii = 0; ii < predicates.size(); ii++) {
                            params[ii] = predicates.get(ii);
                        }

                        update.populate(oRec).where(params).addBatch();
                        nBatchUpdate++;

                        if (maxRecordBatchSize > -1 && i > 0) {
                            if (i % maxRecordBatchSize == 0) {
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

                case DELETE:

                    SQLDeleteClause delete = new SQLDeleteClause(conn, configuration, t);
                    long nBatchDel = 0;
                    for (int i = 0; i < records.size(); i++) {
                        records.get(i);

                        List<BooleanExpression> predicates = predicateslist.get(i);

                        BooleanExpression[] params;
                        params = new BooleanExpression[predicates.size()];

                        for (int ii = 0; ii < predicates.size(); ii++) {
                            params[ii] = predicates.get(ii);
                        }

                        delete.where(params).addBatch();
                        nBatchDel++;
                        if (maxRecordBatchSize > -1 && i > 0) {
                            if (i % maxRecordBatchSize == 0) {
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

                default:
                    throw new Exception("");
            }
        }
        finally {
            // conn.setAutoCommit(true);
        }
    }

    /**
     *
     *
     * @param input
     * @return String
     * @throws Exception
     */
    public static String escapeJson(String input) throws Exception {
        JSONParser parser = new JSONParser();
        JSONObject json = (JSONObject) parser.parse(input);

        // Special handling of ADV_OPTIONS
        String key = "ADV_OPTIONS";
        if (json.containsKey(key)) {
            String advopts = json.get(key).toString();
            String cleanup = advopts.replaceAll("-D \"", "'").replaceAll("=", "': '").replaceAll("\"", "',")
                    .replaceAll("'", "\"");
            // wrap with curly braces and remove trailing comma
            cleanup = "{" + cleanup.substring(0, cleanup.length() - 1) + "}";
            // System.out.println(cleanup);
            JSONObject obj = (JSONObject) parser.parse(cleanup);

            json.put(key, obj);
        }
        // System.out.println(JSONObject.escape(json.toString()).toString());

        return JSONObject.escape(json.toString());
    }

    public static long updateMapsTableTags(final Map<String, String> tags, final long mapId, final Connection conn)
            throws SQLException {
        long execResult = -1;
        PreparedStatement ps = null;
        try {
            String sql = null;

            sql = "update maps set tags = COALESCE(tags, '') || ?::hstore " + "where id=?";
            ps = conn.prepareStatement(sql);
            String hstoreStr = "";
            Iterator it = tags.entrySet().iterator();
            while (it.hasNext()) {
                Map.Entry pairs = (Map.Entry) it.next();
                if (hstoreStr.length() > 0) {
                    hstoreStr += ",";
                }
                hstoreStr += "\"" + pairs.getKey() + "\"=>\"" + pairs.getValue() + "\"";
            }
            ps.setObject(1, hstoreStr, Types.OTHER);
            ps.setLong(2, mapId);

            execResult = ps.executeUpdate();
        }
        finally {
            if (ps != null) {
                ps.close();
            }
        }
        return execResult;
    }

    public static Map<String, String> getMapsTableTags(final long mapId, final Connection conn) throws Exception {
        Map<String, String> tags = new HashMap<String, String>();
        QMaps mp = QMaps.maps;

        List<Object> res = new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(mp).where(mp.id.eq(mapId))
                .list(mp.tags);

        if (res.size() > 0) {
            Object oTag = res.get(0);
            tags = PostgresUtils.postgresObjToHStore((org.postgresql.util.PGobject) oTag);
        }

        return tags;
    }

    public static long batchRecordsDirectNodes(final long mapId, final List<?> records,
            final RecordBatchType recordBatchType, Connection conn, int maxRecordBatchSize) throws Exception {
        log.debug("Batch node " + recordBatchType.toString() + "...");

        long updateCount = 0;
        PreparedStatement ps = null;
        try {
            String sql = null;

            int count = 0;

            switch (recordBatchType) {
                case INSERT:

                    sql = "insert into current_nodes_" + mapId + " (id, latitude, "
                            + "longitude, changeset_id, visible, \"timestamp\", tile, version, tags) "
                            + "values (?, ?, ?, ?, ?, ?, ?, ?, ?)";

                    ps = conn.prepareStatement(sql);
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

                        @SuppressWarnings("unchecked")
                        Map<String, String> tags = (Map<String, String>) node.getTags();

                        String hstoreStr = "";
                        Iterator it = tags.entrySet().iterator();
                        while (it.hasNext()) {
                            Map.Entry pairs = (Map.Entry) it.next();
                            if (hstoreStr.length() > 0) {
                                hstoreStr += ",";
                            }
                            hstoreStr += "\"" + pairs.getKey() + "\"=>\"" + pairs.getValue() + "\"";
                        }
                        ps.setObject(9, hstoreStr, Types.OTHER);
                        ps.addBatch();

                        if (maxRecordBatchSize > -1) {
                            if (++count % maxRecordBatchSize == 0) {
                                updateCount += ps.executeBatch().length;
                            }
                        }
                    }

                break;

                case UPDATE:

                    sql = "update current_nodes_" + mapId + " set  latitude=?, "
                            + "longitude=?, changeset_id=?, visible=?, \"timestamp\"=?, tile=?, version=?, tags=? "
                            + "where id=?";
                    ps = conn.prepareStatement(sql);
                    for (Object o : records) {
                        CurrentNodes node = (CurrentNodes) o;

                        ps.setDouble(1, node.getLatitude());
                        ps.setDouble(2, node.getLongitude());
                        ps.setLong(3, node.getChangesetId());
                        ps.setBoolean(4, node.getVisible());
                        ps.setTimestamp(5, node.getTimestamp());
                        ps.setLong(6, node.getTile());
                        ps.setLong(7, node.getVersion());

                        @SuppressWarnings("unchecked")
                        Map<String, String> tags = (Map<String, String>) node.getTags();

                        String hstoreStr = "";
                        Iterator it = tags.entrySet().iterator();
                        while (it.hasNext()) {
                            Map.Entry pairs = (Map.Entry) it.next();
                            if (hstoreStr.length() > 0) {
                                hstoreStr += ",";
                            }
                            hstoreStr += "\"" + pairs.getKey() + "\"=>\"" + pairs.getValue() + "\"";
                        }
                        ps.setObject(8, hstoreStr, Types.OTHER);

                        ps.setLong(9, node.getId());

                        ps.addBatch();

                        if (maxRecordBatchSize > -1) {
                            if (++count % maxRecordBatchSize == 0) {
                                updateCount += ps.executeBatch().length;
                                ps.clearBatch();
                            }
                        }
                    }

                break;

                case DELETE:

                    sql = "delete from current_nodes_" + mapId + " where id=?";
                    ps = conn.prepareStatement(sql);
                    for (Object o : records) {
                        CurrentNodes node = (CurrentNodes) o;

                        ps.setLong(1, node.getId());

                        ps.addBatch();
                        if (maxRecordBatchSize > -1) {
                            if (++count % maxRecordBatchSize == 0) {
                                updateCount += ps.executeBatch().length;
                                ps.clearBatch();
                            }
                        }
                    }

                break;

                default:
                    throw new Exception("");
            }

            updateCount += ps.executeBatch().length;
        }
        finally {
            if (ps != null) {
                ps.close();
            }

        }
        return updateCount;
    }

    public static long batchRecordsDirectWays(final long mapId, final List<?> records,
            final RecordBatchType recordBatchType, Connection conn, int maxRecordBatchSize) throws Exception {
        log.debug("Batch way " + recordBatchType.toString() + "...");

        long updateCount = 0;
        PreparedStatement ps = null;
        try {
            String sql = null;

            int count = 0;

            switch (recordBatchType) {
                case INSERT:

                    sql = "insert into current_ways_" + mapId
                            + " (id, changeset_id, \"timestamp\", visible, version, tags) "
                            + "values (?, ?, ?, ?, ?, ?)";

                    ps = conn.prepareStatement(sql);
                    for (Object o : records) {
                        CurrentWays way = (CurrentWays) o;

                        ps.setLong(1, way.getId());
                        ps.setLong(2, way.getChangesetId());
                        ps.setTimestamp(3, way.getTimestamp());
                        ps.setBoolean(4, way.getVisible());
                        ps.setLong(5, way.getVersion());

                        @SuppressWarnings("unchecked")
                        Map<String, String> tags = (Map<String, String>) way.getTags();

                        String hstoreStr = "";
                        Iterator it = tags.entrySet().iterator();
                        while (it.hasNext()) {
                            Map.Entry pairs = (Map.Entry) it.next();
                            if (hstoreStr.length() > 0) {
                                hstoreStr += ",";
                            }
                            hstoreStr += "\"" + pairs.getKey() + "\"=>\"" + pairs.getValue() + "\"";
                        }
                        ps.setObject(6, hstoreStr, Types.OTHER);
                        ps.addBatch();

                        if (maxRecordBatchSize > -1) {
                            if (++count % maxRecordBatchSize == 0) {
                                updateCount += ps.executeBatch().length;
                            }
                        }

                    }

                break;

                case UPDATE:

                    sql = "update current_ways_" + mapId
                            + " set changeset_id=?, visible=?, \"timestamp\"=?, version=?, tags=? " + "where id=?";
                    ps = conn.prepareStatement(sql);
                    for (Object o : records) {
                        CurrentWays way = (CurrentWays) o;

                        ps.setLong(1, way.getChangesetId());
                        ps.setBoolean(2, way.getVisible());
                        ps.setTimestamp(3, way.getTimestamp());
                        ps.setLong(4, way.getVersion());

                        @SuppressWarnings("unchecked")
                        Map<String, String> tags = (Map<String, String>) way.getTags();

                        String hstoreStr = "";
                        Iterator it = tags.entrySet().iterator();
                        while (it.hasNext()) {
                            Map.Entry pairs = (Map.Entry) it.next();
                            if (hstoreStr.length() > 0) {
                                hstoreStr += ",";
                            }
                            hstoreStr += "\"" + pairs.getKey() + "\"=>\"" + pairs.getValue() + "\"";
                        }
                        ps.setObject(5, hstoreStr, Types.OTHER);

                        ps.setLong(6, way.getId());

                        ps.addBatch();

                        if (maxRecordBatchSize > -1) {
                            if (++count % maxRecordBatchSize == 0) {
                                updateCount += ps.executeBatch().length;
                            }
                        }
                    }

                break;

                case DELETE:

                    sql = "delete from current_ways_" + mapId + " where id=?";
                    ps = conn.prepareStatement(sql);
                    for (Object o : records) {
                        CurrentWays way = (CurrentWays) o;

                        ps.setLong(1, way.getId());

                        ps.addBatch();

                        if (maxRecordBatchSize > -1) {
                            if (++count % maxRecordBatchSize == 0) {
                                updateCount += ps.executeBatch().length;
                            }
                        }

                    }

                break;

                default:
                    throw new Exception("");
            }

            updateCount += ps.executeBatch().length;
        }
        finally {
            if (ps != null) {
                ps.close();
            }
        }
        return updateCount;
    }

    public static long batchRecordsDirectRelations(final long mapId, final List<?> records,
            final RecordBatchType recordBatchType, Connection conn, int maxRecordBatchSize) throws Exception {
        log.debug("Batch relation " + recordBatchType.toString() + "...");

        long updateCount = 0;
        PreparedStatement ps = null;
        try {
            String sql = null;
            int count = 0;

            switch (recordBatchType) {
                case INSERT:

                    sql = "insert into current_relations_" + mapId
                            + " (id, changeset_id, \"timestamp\", visible, version, tags) "
                            + "values (?, ?, ?, ?, ?, ?)";

                    ps = conn.prepareStatement(sql);
                    for (Object o : records) {
                        CurrentRelations rel = (CurrentRelations) o;

                        ps.setLong(1, rel.getId());
                        ps.setLong(2, rel.getChangesetId());
                        ps.setTimestamp(3, rel.getTimestamp());
                        ps.setBoolean(4, rel.getVisible());
                        ps.setLong(5, rel.getVersion());

                        @SuppressWarnings("unchecked")
                        Map<String, String> tags = (Map<String, String>) rel.getTags();

                        String hstoreStr = "";
                        Iterator it = tags.entrySet().iterator();
                        while (it.hasNext()) {
                            Map.Entry pairs = (Map.Entry) it.next();
                            if (hstoreStr.length() > 0) {
                                hstoreStr += ",";
                            }
                            hstoreStr += "\"" + pairs.getKey() + "\"=>\"" + pairs.getValue() + "\"";
                        }
                        ps.setObject(6, hstoreStr, Types.OTHER);
                        ps.addBatch();

                        if (maxRecordBatchSize > -1) {
                            if (++count % maxRecordBatchSize == 0) {
                                updateCount += ps.executeBatch().length;
                            }
                        }

                    }

                break;

                case UPDATE:

                    sql = "update current_relations_" + mapId
                            + " set changeset_id=?, visible=?, \"timestamp\"=?, version=?, tags=? " + "where id=?";
                    ps = conn.prepareStatement(sql);
                    for (Object o : records) {
                        CurrentRelations rel = (CurrentRelations) o;

                        ps.setLong(1, rel.getChangesetId());
                        ps.setBoolean(2, rel.getVisible());
                        ps.setTimestamp(3, rel.getTimestamp());
                        ps.setLong(4, rel.getVersion());

                        @SuppressWarnings("unchecked")
                        Map<String, String> tags = (Map<String, String>) rel.getTags();

                        String hstoreStr = "";
                        Iterator it = tags.entrySet().iterator();
                        while (it.hasNext()) {
                            Map.Entry pairs = (Map.Entry) it.next();
                            if (hstoreStr.length() > 0) {
                                hstoreStr += ",";
                            }
                            hstoreStr += "\"" + pairs.getKey() + "\"=>\"" + pairs.getValue() + "\"";
                        }
                        ps.setObject(5, hstoreStr, Types.OTHER);

                        ps.setLong(6, rel.getId());

                        ps.addBatch();

                        if (maxRecordBatchSize > -1) {
                            if (++count % maxRecordBatchSize == 0) {
                                updateCount += ps.executeBatch().length;
                            }
                        }
                    }

                break;

                case DELETE:

                    sql = "delete from current_relations_" + mapId + " where id=?";
                    ps = conn.prepareStatement(sql);
                    for (Object o : records) {
                        CurrentRelations rel = (CurrentRelations) o;

                        ps.setLong(1, rel.getId());

                        ps.addBatch();

                        if (maxRecordBatchSize > -1) {
                            if (++count % maxRecordBatchSize == 0) {
                                updateCount += ps.executeBatch().length;
                            }
                        }
                    }

                break;

                default:
                    throw new Exception("");
            }

            updateCount += ps.executeBatch().length;
        }
        finally {
            if (ps != null) {
                ps.close();
            }
        }
        return updateCount;
    }

    /**
     * Returns table size in byte
     */
    public static long getTableSizeInByte(final String tableName) throws Exception {
        long ret = 0;
        Connection conn = null;
        Statement stmt = null;
        ResultSet rs = null;
        try {
            conn = DbUtils.createConnection();
            stmt = conn.createStatement();

            String sql = "select pg_total_relation_size('" + tableName + "') as tablesize";
            rs = stmt.executeQuery(sql);

            if (rs == null) {
                throw new SQLException("Error executing getTableSizeInByte");
            }

            // STEP 5: Extract data from result set
            while (rs.next()) {
                // Retrieve by column name
                ret = rs.getLong("tablesize");
            }
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
        finally {
            if (rs != null)
                rs.close();
            if (stmt != null)
                stmt.close();
            if (conn != null)
                DbUtils.closeConnection(conn);
        }

        return ret;
    }

    /**
     * //TODO: change back to original element generic code
     */
    public static Object resultToObj(final ResultSet rs, final ElementType elementType) throws SQLException {
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
        else if (elementType == ElementType.Way) {
            CurrentWays ways = new CurrentWays();
            ways.setId(rs.getLong("id"));

            ways.setChangesetId(rs.getLong("changeset_id"));
            ways.setVisible(rs.getBoolean("visible"));
            ways.setTimestamp(rs.getTimestamp("timestamp"));
            ways.setVersion(rs.getLong("version"));
            ways.setTags(rs.getObject("tags"));
            return ways;
        }
        else if (elementType == ElementType.Relation) {
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
