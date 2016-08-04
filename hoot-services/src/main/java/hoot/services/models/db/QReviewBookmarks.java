package hoot.services.models.db;

import static com.mysema.query.types.PathMetadataFactory.*;

import com.mysema.query.types.path.*;

import com.mysema.query.types.PathMetadata;
import javax.annotation.Generated;
import com.mysema.query.types.Path;

import com.mysema.query.sql.ColumnMetadata;




/**
 * QReviewBookmarks is a Querydsl query type for ReviewBookmarks
 */
@Generated("com.mysema.query.sql.codegen.MetaDataSerializer")
public class QReviewBookmarks extends com.mysema.query.sql.RelationalPathBase<ReviewBookmarks> {

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

    public final com.mysema.query.sql.PrimaryKey<ReviewBookmarks> reviewBookmarksPk = createPrimaryKey(id);

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

    public QReviewBookmarks(PathMetadata<?> metadata) {
        super(ReviewBookmarks.class, metadata, "public", "review_bookmarks");
        addMetadata();
    }

    public void addMetadata() {
        addMetadata(createdAt, ColumnMetadata.named("created_at").ofType(93).withSize(29).withDigits(6).notNull());
        addMetadata(createdBy, ColumnMetadata.named("created_by").ofType(-5).withSize(19));
        addMetadata(detail, ColumnMetadata.named("detail").ofType(1111).withSize(2147483647));
        addMetadata(id, ColumnMetadata.named("id").ofType(-5).withSize(19).notNull());
        addMetadata(lastModifiedAt, ColumnMetadata.named("last_modified_at").ofType(93).withSize(29).withDigits(6));
        addMetadata(lastModifiedBy, ColumnMetadata.named("last_modified_by").ofType(-5).withSize(19));
        addMetadata(mapId, ColumnMetadata.named("map_id").ofType(-5).withSize(19).notNull());
        addMetadata(relationId, ColumnMetadata.named("relation_id").ofType(-5).withSize(19).notNull());
    }

}

