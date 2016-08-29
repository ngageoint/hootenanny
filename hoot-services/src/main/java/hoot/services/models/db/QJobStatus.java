package hoot.services.models.db;

import static com.querydsl.core.types.PathMetadataFactory.*;

import com.querydsl.core.types.dsl.*;

import com.querydsl.core.types.PathMetadata;
import javax.annotation.Generated;
import com.querydsl.core.types.Path;

import com.querydsl.sql.ColumnMetadata;
import java.sql.Types;




/**
 * QJobStatus is a Querydsl query type for JobStatus
 */
@Generated("com.querydsl.sql.codegen.MetaDataSerializer")
public class QJobStatus extends com.querydsl.sql.RelationalPathBase<JobStatus> {

    private static final long serialVersionUID = 364396917;

    public static final QJobStatus jobStatus = new QJobStatus("job_status");

    public final DateTimePath<java.sql.Timestamp> end = createDateTime("end", java.sql.Timestamp.class);

    public final StringPath jobId = createString("jobId");

    public final NumberPath<Double> percentComplete = createNumber("percentComplete", Double.class);

    public final DateTimePath<java.sql.Timestamp> start = createDateTime("start", java.sql.Timestamp.class);

    public final NumberPath<Integer> status = createNumber("status", Integer.class);

    public final StringPath statusDetail = createString("statusDetail");

    public final com.querydsl.sql.PrimaryKey<JobStatus> jobStatusPkey = createPrimaryKey(jobId);

    public QJobStatus(String variable) {
        super(JobStatus.class, forVariable(variable), "public", "job_status");
        addMetadata();
    }

    public QJobStatus(String variable, String schema, String table) {
        super(JobStatus.class, forVariable(variable), schema, table);
        addMetadata();
    }

    public QJobStatus(Path<? extends JobStatus> path) {
        super(path.getType(), path.getMetadata(), "public", "job_status");
        addMetadata();
    }

    public QJobStatus(PathMetadata metadata) {
        super(JobStatus.class, metadata, "public", "job_status");
        addMetadata();
    }

    public void addMetadata() {
        addMetadata(end, ColumnMetadata.named("end").withIndex(3).ofType(Types.TIMESTAMP).withSize(35).withDigits(6));
        addMetadata(jobId, ColumnMetadata.named("job_id").withIndex(1).ofType(Types.VARCHAR).withSize(64).notNull());
        addMetadata(percentComplete, ColumnMetadata.named("percent_complete").withIndex(5).ofType(Types.DOUBLE).withSize(17).withDigits(17));
        addMetadata(start, ColumnMetadata.named("start").withIndex(2).ofType(Types.TIMESTAMP).withSize(35).withDigits(6).notNull());
        addMetadata(status, ColumnMetadata.named("status").withIndex(4).ofType(Types.INTEGER).withSize(10).notNull());
        addMetadata(statusDetail, ColumnMetadata.named("status_detail").withIndex(6).ofType(Types.VARCHAR).withSize(2147483647));
    }

}

