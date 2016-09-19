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
package hoot.services.models.db;

import static com.querydsl.core.types.PathMetadataFactory.*;

import com.querydsl.core.types.dsl.*;

import com.querydsl.core.types.PathMetadata;
import javax.annotation.Generated;
import com.querydsl.core.types.Path;

import com.querydsl.sql.ColumnMetadata;
import java.sql.Types;




/**
 * QCurrentWayNodes is a Querydsl query type for CurrentWayNodes
 */
@Generated("com.querydsl.sql.codegen.MetaDataSerializer")
public class QCurrentWayNodes extends com.querydsl.sql.RelationalPathBase<CurrentWayNodes> {

    private static final long serialVersionUID = -677563263;

    public static final QCurrentWayNodes currentWayNodes = new QCurrentWayNodes("current_way_nodes");

    public final NumberPath<Long> nodeId = createNumber("nodeId", Long.class);

    public final NumberPath<Long> sequenceId = createNumber("sequenceId", Long.class);

    public final NumberPath<Long> wayId = createNumber("wayId", Long.class);

    public final com.querydsl.sql.PrimaryKey<CurrentWayNodes> currentWayNodesPkey = createPrimaryKey(wayId, sequenceId);

    public QCurrentWayNodes(String variable) {
        super(CurrentWayNodes.class, forVariable(variable), "public", "current_way_nodes");
        addMetadata();
    }

    public QCurrentWayNodes(String variable, String schema, String table) {
        super(CurrentWayNodes.class, forVariable(variable), schema, table);
        addMetadata();
    }

    public QCurrentWayNodes(Path<? extends CurrentWayNodes> path) {
        super(path.getType(), path.getMetadata(), "public", "current_way_nodes");
        addMetadata();
    }

    public QCurrentWayNodes(PathMetadata metadata) {
        super(CurrentWayNodes.class, metadata, "public", "current_way_nodes");
        addMetadata();
    }

    public void addMetadata() {
        addMetadata(nodeId, ColumnMetadata.named("node_id").withIndex(2).ofType(Types.BIGINT).withSize(19).notNull());
        addMetadata(sequenceId, ColumnMetadata.named("sequence_id").withIndex(3).ofType(Types.BIGINT).withSize(19).notNull());
        addMetadata(wayId, ColumnMetadata.named("way_id").withIndex(1).ofType(Types.BIGINT).withSize(19).notNull());
    }

}

