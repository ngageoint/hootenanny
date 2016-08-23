package hoot.services.models.db;

import static com.querydsl.core.types.PathMetadataFactory.*;

import com.querydsl.core.types.dsl.*;

import com.querydsl.core.types.PathMetadata;
import javax.annotation.Generated;
import com.querydsl.core.types.Path;

import com.querydsl.sql.ColumnMetadata;
import java.sql.Types;




/**
 * QReviewBookmarks is a Querydsl query type for ReviewBookmarks
 */
@Generated("com.querydsl.sql.codegen.MetaDataSerializer")
public class QReviewBookmarks extends com.querydsl.sql.RelationalPathBase<ReviewBookmarks> {

    private static final long serialVersionUID = -1625377685;

    public static final QReviewBookmarks reviewBookmarks = new QReviewBookmarks("review_bookmarks");

    public final DateTimePath<java.sql.Timestamp> createdAt = createDateTime("createdAt", java.sql.Timestamp.class);

    public final NumberPath<Long> createdBy = createNumber("createdBy", Long.class);

    public final SimplePath<Object> detail = createSimple("detail", Object.class);

    public final NumberPath<Long> id = createNumber("id", Long.class);

    public final DateTimePath<java.sql.Timestamp> lastModifiedAt = createDateTime("lastModifiedAt", java.sql.Timestamp.class);

    public final NumberPath<Long> lastModifiedBy = createNumber("lastModifiedBy", Long.class);

    public final NumberPath<Long> mapId = createNumber("mapId", Long.class);

    public final NumberPath<Long> relationId = createNumber("relationId", Long.class);

    public final com.querydsl.sql.PrimaryKey<ReviewBookmarks> reviewBookmarksPk = createPrimaryKey(id);

    public QReviewBookmarks(String variable) {
        super(ReviewBookmarks.class, forVariable(variable), "public", "review_bookmarks");
        addMetadata();
    }

    public QReviewBookmarks(String variable, String schema, String table) {
        super(ReviewBookmarks.class, forVariable(variable), schema, table);
        addMetadata();
    }

    public QReviewBookmarks(Path<? extends ReviewBookmarks> path) {
        super(path.getType(), path.getMetadata(), "public", "review_bookmarks");
        addMetadata();
    }

    public QReviewBookmarks(PathMetadata metadata) {
        super(ReviewBookmarks.class, metadata, "public", "review_bookmarks");
        addMetadata();
    }

    public void addMetadata() {
        addMetadata(createdAt, ColumnMetadata.named("created_at").withIndex(5).ofType(Types.TIMESTAMP).withSize(29).withDigits(6).notNull());
        addMetadata(createdBy, ColumnMetadata.named("created_by").withIndex(7).ofType(Types.BIGINT).withSize(19));
        addMetadata(detail, ColumnMetadata.named("detail").withIndex(4).ofType(Types.OTHER).withSize(2147483647));
        addMetadata(id, ColumnMetadata.named("id").withIndex(1).ofType(Types.BIGINT).withSize(19).notNull());
        addMetadata(lastModifiedAt, ColumnMetadata.named("last_modified_at").withIndex(6).ofType(Types.TIMESTAMP).withSize(29).withDigits(6));
        addMetadata(lastModifiedBy, ColumnMetadata.named("last_modified_by").withIndex(8).ofType(Types.BIGINT).withSize(19));
        addMetadata(mapId, ColumnMetadata.named("map_id").withIndex(2).ofType(Types.BIGINT).withSize(19).notNull());
        addMetadata(relationId, ColumnMetadata.named("relation_id").withIndex(3).ofType(Types.BIGINT).withSize(19).notNull());
    }

}

