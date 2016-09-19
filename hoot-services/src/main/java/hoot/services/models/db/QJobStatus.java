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

