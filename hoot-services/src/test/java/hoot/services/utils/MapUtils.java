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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
package hoot.services.utils;

import static hoot.services.models.db.QFolders.folders;
import static hoot.services.models.db.QMaps.maps;
import static hoot.services.models.db.QUsers.users;
import static hoot.services.utils.DbUtils.createQuery;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.sql.Timestamp;
import java.util.Calendar;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.querydsl.core.types.dsl.Expressions;
import com.querydsl.sql.SQLQuery;

import hoot.services.models.db.Maps;
import hoot.services.models.db.QCurrentNodes;
import hoot.services.models.db.QCurrentRelationMembers;
import hoot.services.models.db.QCurrentRelations;
import hoot.services.models.db.QCurrentWayNodes;
import hoot.services.models.db.QCurrentWays;
import hoot.services.models.db.Users;


public final class MapUtils {

    private MapUtils() {}

    /**
     *
     *
     * @param mapId

     *             //TODO: This code needs to be changed to dynamically read in
     *             the data types from querydsl. If I make a change to the
     *             schema in liquibase, it will never be picked up unless this
     *             static code is also changed. See r6777
     */
    public static void createMap(long mapId) {
        try {
            Connection connection = createQuery().getConnection();

            // changesets
            String createTblSql = "CREATE TABLE changesets_" + mapId + "(id bigserial NOT NULL, "
                    + " user_id bigint NOT NULL, " + " created_at timestamp without time zone NOT NULL, "
                    + " min_lat double precision NOT NULL, " + " max_lat double precision NOT NULL, "
                    + " min_lon double precision NOT NULL, " + " max_lon double precision NOT NULL, "
                    + " closed_at timestamp without time zone NOT NULL, " + " num_changes integer NOT NULL DEFAULT 0, "
                    + " tags hstore, " + " CONSTRAINT pk_changesets_" + mapId + " PRIMARY KEY (id ), "
                    + " CONSTRAINT changesets_" + mapId + "_user_id_fkey FOREIGN KEY (user_id) "
                    + " REFERENCES users (id) MATCH SIMPLE " + " ON UPDATE NO ACTION ON DELETE NO ACTION "
                    + " ) WITH ( OIDS=FALSE );";

            createTable(createTblSql, connection);

            // current_nodes
            createTblSql = "CREATE TABLE current_nodes_" + mapId + "(id bigserial NOT NULL, "
                    + " latitude double precision NOT NULL, " + " longitude double precision NOT NULL, "
                    + " changeset_id bigint NOT NULL, " + " visible boolean NOT NULL DEFAULT true, "
                    + " \"timestamp\" timestamp without time zone NOT NULL DEFAULT now(), " + " tile bigint NOT NULL, "
                    + " version bigint NOT NULL DEFAULT 1, " + " tags hstore, " + " CONSTRAINT current_nodes_" + mapId
                    + "_pkey" + " PRIMARY KEY (id ), " + " CONSTRAINT current_nodes_" + mapId
                    + "_changeset_id_fkey FOREIGN KEY (changeset_id) " + " REFERENCES changesets_" + mapId
                    + " (id) MATCH SIMPLE " + " ON UPDATE NO ACTION ON DELETE NO ACTION " + " ) WITH ( OIDS=FALSE );";

            createTable(createTblSql, connection);

            // current_relation_members
            createTblSql = "CREATE TABLE current_relation_members_" + mapId + "(relation_id bigint NOT NULL, "
                    + " member_type nwr_enum NOT NULL, " + " member_id bigint NOT NULL, "
                    + " member_role character varying(255) NOT NULL, " + " sequence_id integer NOT NULL DEFAULT 0, "
                    + " CONSTRAINT current_relation_members_" + mapId
                    + "_pkey PRIMARY KEY (relation_id , member_type , member_id , member_role , sequence_id ) "
                    + " ) WITH ( OIDS=FALSE );";

            createTable(createTblSql, connection);

            // current_relations
            createTblSql = "CREATE TABLE current_relations_" + mapId + "(" + "  id bigserial NOT NULL,"
                    + "  changeset_id bigint NOT NULL,"
                    + "  \"timestamp\" timestamp without time zone NOT NULL DEFAULT now(),"
                    + "  visible boolean NOT NULL DEFAULT true," + "  version bigint NOT NULL DEFAULT 1,"
                    + "  tags hstore," + "  CONSTRAINT current_relations_" + mapId + "_pkey PRIMARY KEY (id ),"
                    + "  CONSTRAINT current_relations_" + mapId + "_changeset_id_fkey FOREIGN KEY (changeset_id)"
                    + "      REFERENCES changesets_" + mapId + " (id) MATCH SIMPLE"
                    + "      ON UPDATE NO ACTION ON DELETE NO ACTION" + ")" + "WITH (" + "  OIDS=FALSE" + ");";

            createTable(createTblSql, connection);

            // current_way_nodes
            createTblSql = "CREATE TABLE current_way_nodes_" + mapId + "(" + "  way_id bigint NOT NULL,"
                    + "  node_id bigint NOT NULL," + "  sequence_id bigint NOT NULL," + "  CONSTRAINT current_way_"
                    + mapId + "_nodes_pkey PRIMARY KEY (way_id , sequence_id )" + ")" + "WITH (" + "  OIDS=FALSE"
                    + ");";

            createTable(createTblSql, connection);

            // current_ways
            createTblSql = "CREATE TABLE current_ways_" + mapId + "(" + "  id bigserial NOT NULL,"
                    + "  changeset_id bigint NOT NULL,"
                    + "  \"timestamp\" timestamp without time zone NOT NULL DEFAULT now(),"
                    + "  visible boolean NOT NULL DEFAULT true," + "  version bigint NOT NULL DEFAULT 1,"
                    + "  tags hstore," + "  CONSTRAINT current_ways_" + mapId + "_pkey PRIMARY KEY (id ),"
                    + "  CONSTRAINT current_ways_" + mapId + "_changeset_id_fkey FOREIGN KEY (changeset_id)"
                    + "      REFERENCES changesets_" + mapId + " (id) MATCH SIMPLE"
                    + "      ON UPDATE NO ACTION ON DELETE NO ACTION" + ")" + "WITH (" + "  OIDS=FALSE" + ");";

            createTable(createTblSql, connection);
        }
        catch (SQLException e) {
            throw new RuntimeException("Error creating map with id = " + mapId, e);
        }
    }

    /**
     * Determines whether any OSM element records exist in the services database
     *
     * @return true if any OSM element records exist in the services database;
     *         false otherwise
     */
    public static boolean elementDataExistsInServicesDb() {
        long recordCount = 0;

        SQLQuery query = createQuery().query();
        recordCount += query.from(QCurrentNodes.currentNodes).fetchCount();
        recordCount += query.from(QCurrentWayNodes.currentWayNodes).fetchCount();
        recordCount += query.from(QCurrentWays.currentWays).fetchCount();
        recordCount += query.from(QCurrentRelationMembers.currentRelationMembers).fetchCount();
        recordCount += query.from(QCurrentRelations.currentRelations).fetchCount();

        return (recordCount > 0);
    }

    public static long insertMap(long userId) {
        return insertMap(userId, new Timestamp(Calendar.getInstance().getTimeInMillis()));
    }

    public static long insertMap(long userId, Timestamp ts) {
        Long newId = createQuery()
                .select(Expressions.numberTemplate(Long.class, "nextval('maps_id_seq')"))
                .from()
                .fetchOne();

        if (newId != null) {

            createQuery().insert(maps)
            .columns(maps.id, maps.createdAt, maps.displayName, maps.publicCol, maps.userId)
            .values(newId, ts, "map-with-id-" + newId, true, userId)
            .execute();
        }

        createMap(newId);

        return newId;
    }

    public static long insertUser() {
        return insertUser(false, false);
    }
    public static long insertAdvancedUser() {
        return insertUser(true, false);
    }
    public static long insertAdminUser() {
        return insertUser(false, true);
    }
    public static long insertAdvancedAdminUser() {
        return insertUser(true, true);
    }
    public static long insertUser(boolean advanced, boolean admin) {
        Map<String,String> privs = new HashMap<>();
        privs.put("advanced", String.valueOf(advanced));
        privs.put("admin", String.valueOf(admin));

        Long newId = createQuery()
                .select(Expressions.numberTemplate(Long.class, "nextval('users_id_seq')"))
                .from()
                .fetchOne();

        assert newId != null : "failed to generate new test user id";

        // don't collide with someone's actual osm user id
        newId *= -1;
        long rowsAffected = createQuery().insert(users)
                .columns(users.id, users.displayName, users.email, users.provider_access_key,
                        users.provider_access_token, users.hootservices_created_at,
                        users.hootservices_last_authorize, users.provider_created_at, users.privileges)
                .values(newId, newId + "::MapUtils::insertUser()", newId + "@hootenanny.test", "provider_access_key",
                        "provider_access_token",
                        Expressions.currentTimestamp(), Expressions.currentTimestamp(), Expressions.currentTimestamp(),
                        privs)
                .execute();

        assert rowsAffected == 1 : "failed to insert test user";

        return newId;
    }

    public static Users getUser(Long id) {
        return createQuery().select(users).from(users).where(users.id.eq(id)).fetchFirst();
    }

    public static void deleteUser(long userId) {
        createQuery().delete(users).where(users.id.eq(userId)).execute();
    }

    public static void cleanupTestUsers() {
        List<Maps> testMaps = createQuery().select(maps).from(maps)
                .where(maps.userId.lt(0)).fetch();
        testMaps.stream().forEach(m -> {
            DbUtils.deleteMapRelatedTablesByMapId(m.getId());
            DbUtils.deleteMap(m.getId());
        });

        createQuery().delete(users).where(users.displayName.like("%::MapUtils::insertUser()")).execute();
    }

    public static long addPrivateFolder(String name, long userId) {
        Long newId = createQuery()
                .select(Expressions.numberTemplate(Long.class, "nextval('folders_id_seq')"))
                .from()
                .fetchOne();

        Timestamp now = new Timestamp(System.currentTimeMillis());


        createQuery()
                .insert(folders).columns(folders.id, folders.createdAt, folders.displayName, folders.publicCol, folders.userId, folders.parentId)
                .values(newId, now, name, false, userId, 0).execute();

        return newId;
    }

    private static void createTable(String createTblSql, Connection conn) throws SQLException {
        try (PreparedStatement stmt = conn.prepareStatement(createTblSql)) {
            stmt.executeUpdate();
        }
    }
}
