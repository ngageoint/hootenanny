package hoot.services.models.db;

import static com.mysema.query.types.PathMetadataFactory.*;

import com.mysema.query.types.path.*;

import com.mysema.query.types.PathMetadata;
import javax.annotation.Generated;
import com.mysema.query.types.Path;

import com.mysema.query.sql.ColumnMetadata;




/**
 * QCurrentRelationMembers is a Querydsl query type for CurrentRelationMembers
 */
@Generated("com.mysema.query.sql.codegen.MetaDataSerializer")
public class QCurrentRelationMembers extends com.mysema.query.sql.RelationalPathBase<CurrentRelationMembers> {

    private static final long serialVersionUID = 738359070;

    public static final QCurrentRelationMembers currentRelationMembers = new QCurrentRelationMembers("current_relation_members");

    public final NumberPath<Long> memberId = createNumber("memberId", Long.class);

    public final StringPath memberRole = createString("memberRole");

    public final SimplePath<Object> memberType = createSimple("memberType", Object.class);

    public final NumberPath<Long> relationId = createNumber("relationId", Long.class);

    public final NumberPath<Integer> sequenceId = createNumber("sequenceId", Integer.class);

    public final com.mysema.query.sql.PrimaryKey<CurrentRelationMembers> currentRelationMembersPkey = createPrimaryKey(relationId, memberType, memberId, memberRole, sequenceId);

    public QCurrentRelationMembers(String variable) {
        super(CurrentRelationMembers.class, forVariable(variable), "public", "current_relation_members");
        addMetadata();
    }

    public QCurrentRelationMembers(String variable, String schema, String table) {
        super(CurrentRelationMembers.class, forVariable(variable), schema, table);
        addMetadata();
    }

    public QCurrentRelationMembers(Path<? extends CurrentRelationMembers> path) {
        super(path.getType(), path.getMetadata(), "public", "current_relation_members");
        addMetadata();
    }

    public QCurrentRelationMembers(PathMetadata<?> metadata) {
        super(CurrentRelationMembers.class, metadata, "public", "current_relation_members");
        addMetadata();
    }

    public void addMetadata() {
        addMetadata(memberId, ColumnMetadata.named("member_id").ofType(-5).withSize(19).notNull());
        addMetadata(memberRole, ColumnMetadata.named("member_role").ofType(12).withSize(255).notNull());
        addMetadata(memberType, ColumnMetadata.named("member_type").ofType(1111).withSize(2147483647).notNull());
        addMetadata(relationId, ColumnMetadata.named("relation_id").ofType(-5).withSize(19).notNull());
        addMetadata(sequenceId, ColumnMetadata.named("sequence_id").ofType(4).withSize(10).notNull());
    }

}

