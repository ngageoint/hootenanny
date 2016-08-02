package hoot.services.models.db;

import static com.mysema.query.types.PathMetadataFactory.*;

import com.mysema.query.types.path.*;

import com.mysema.query.types.PathMetadata;
import javax.annotation.Generated;
import com.mysema.query.types.Path;

import com.mysema.query.sql.ColumnMetadata;




/**
 * QCurrentWayNodes is a Querydsl query type for CurrentWayNodes
 */
@Generated("com.mysema.query.sql.codegen.MetaDataSerializer")
public class QCurrentWayNodes extends com.mysema.query.sql.RelationalPathBase<CurrentWayNodes> {

    private static final long serialVersionUID = -677563263;

    public static final QCurrentWayNodes currentWayNodes = new QCurrentWayNodes("current_way_nodes");

    public final NumberPath<Long> nodeId = createNumber("nodeId", Long.class);

    public final NumberPath<Long> sequenceId = createNumber("sequenceId", Long.class);

    public final NumberPath<Long> wayId = createNumber("wayId", Long.class);

    public final com.mysema.query.sql.PrimaryKey<CurrentWayNodes> currentWayNodesPkey = createPrimaryKey(wayId, sequenceId);

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

    public QCurrentWayNodes(PathMetadata<?> metadata) {
        super(CurrentWayNodes.class, metadata, "public", "current_way_nodes");
        addMetadata();
    }

    public void addMetadata() {
        addMetadata(nodeId, ColumnMetadata.named("node_id").ofType(-5).withSize(19).notNull());
        addMetadata(sequenceId, ColumnMetadata.named("sequence_id").ofType(-5).withSize(19).notNull());
        addMetadata(wayId, ColumnMetadata.named("way_id").ofType(-5).withSize(19).notNull());
    }

}

