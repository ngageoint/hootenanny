package hoot.services.models.db;

import static com.mysema.query.types.PathMetadataFactory.*;

import com.mysema.query.types.path.*;

import com.mysema.query.types.PathMetadata;
import javax.annotation.Generated;
import com.mysema.query.types.Path;

import com.mysema.query.sql.ColumnMetadata;




/**
 * QJobStatus is a Querydsl query type for JobStatus
 */
@Generated("com.mysema.query.sql.codegen.MetaDataSerializer")
public class QJobStatus extends com.mysema.query.sql.RelationalPathBase<JobStatus> {

    private static final long serialVersionUID = 364396917;

    public static final QJobStatus jobStatus = new QJobStatus("job_status");

    public final DateTimePath<java.sql.Timestamp> end = createDateTime("end", java.sql.Timestamp.class);

    public final StringPath jobId = createString("jobId");

    public final NumberPath<Double> percentComplete = createNumber("percentComplete", Double.class);

    public final DateTimePath<java.sql.Timestamp> start = createDateTime("start", java.sql.Timestamp.class);

    public final NumberPath<Integer> status = createNumber("status", Integer.class);

    public final StringPath statusDetail = createString("statusDetail");

    public final com.mysema.query.sql.PrimaryKey<JobStatus> jobStatusPkey = createPrimaryKey(jobId);

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

    public QJobStatus(PathMetadata<?> metadata) {
        super(JobStatus.class, metadata, "public", "job_status");
        addMetadata();
    }

    public void addMetadata() {
        addMetadata(end, ColumnMetadata.named("end").ofType(93).withSize(35).withDigits(6));
        addMetadata(jobId, ColumnMetadata.named("job_id").ofType(12).withSize(64).notNull());
        addMetadata(percentComplete, ColumnMetadata.named("percent_complete").ofType(8).withSize(17).withDigits(17));
        addMetadata(start, ColumnMetadata.named("start").ofType(93).withSize(35).withDigits(6).notNull());
        addMetadata(status, ColumnMetadata.named("status").ofType(4).withSize(10).notNull());
        addMetadata(statusDetail, ColumnMetadata.named("status_detail").ofType(12).withSize(2147483647));
    }

}

