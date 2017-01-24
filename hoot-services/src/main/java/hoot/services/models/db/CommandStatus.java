package hoot.services.models.db;

import javax.annotation.Generated;

/**
 * CommandStatus is a Querydsl bean type
 */
@Generated("com.querydsl.codegen.BeanSerializer")
public class CommandStatus {

    private String command;

    private Integer exitCode;

    private java.sql.Timestamp finish;

    private String id;

    private String jobId;

    private java.sql.Timestamp start;

    private String stderr;

    private String stdout;

    public String getCommand() {
        return command;
    }

    public void setCommand(String command) {
        this.command = command;
    }

    public Integer getExitCode() {
        return exitCode;
    }

    public void setExitCode(Integer exitCode) {
        this.exitCode = exitCode;
    }

    public java.sql.Timestamp getFinish() {
        return finish;
    }

    public void setFinish(java.sql.Timestamp finish) {
        this.finish = finish;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getJobId() {
        return jobId;
    }

    public void setJobId(String jobId) {
        this.jobId = jobId;
    }

    public java.sql.Timestamp getStart() {
        return start;
    }

    public void setStart(java.sql.Timestamp start) {
        this.start = start;
    }

    public String getStderr() {
        return stderr;
    }

    public void setStderr(String stderr) {
        this.stderr = stderr;
    }

    public String getStdout() {
        return stdout;
    }

    public void setStdout(String stdout) {
        this.stdout = stdout;
    }

}

