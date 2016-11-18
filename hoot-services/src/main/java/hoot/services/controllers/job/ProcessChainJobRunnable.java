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
package hoot.services.controllers.job;

import static hoot.services.HootProperties.CHAIN_JOS_STATUS_PING_INTERVAL;

import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.UUID;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.ApplicationContext;
import org.springframework.transaction.PlatformTransactionManager;
import org.springframework.transaction.TransactionStatus;

import hoot.services.ApplicationContextUtils;
import hoot.services.models.db.JobStatus;
import hoot.services.nativeinterfaces.JobExecutionManager;
import hoot.services.nativeinterfaces.NativeInterfaceException;
import hoot.services.utils.JsonUtils;


class ProcessChainJobRunnable implements Runnable {
    private static final Logger logger = LoggerFactory.getLogger(ProcessChainJobRunnable.class);
    private static final long CHAIN_JOS_STATUS_PING_INTERVAL_VALUE;

    private final String jobId;
    private final String jobs;
    private final JobStatusManager jobStatusManager;
    private final JobExecutionManager jobExecMan;
    private final JSONObject jobInfo = new JSONObject();
    private final JSONArray childrenInfo = new JSONArray();


    static {
        Long value = Long.parseLong(CHAIN_JOS_STATUS_PING_INTERVAL);
        CHAIN_JOS_STATUS_PING_INTERVAL_VALUE = (value < 1000) ? 1000 : value;
    }

    ProcessChainJobRunnable(String jobId, String jobs, JobStatusManager jobStatusManager, JobExecutionManager jobExecMan) {
        this.jobId = jobId;
        this.jobs = jobs;
        this.jobStatusManager = jobStatusManager;
        this.jobExecMan = jobExecMan;
    }

    @Override
    public void run() {
        ApplicationContext appContext = ApplicationContextUtils.getApplicationContext();
        PlatformTransactionManager txManager = appContext.getBean("transactionManager", PlatformTransactionManager.class);

        TransactionStatus transactionStatus = null;
        try {
            transactionStatus = txManager.getTransaction(null);
            processCommand();
            txManager.commit(transactionStatus);
        }
        catch (Exception e) {
            if ((transactionStatus != null) && !transactionStatus.isCompleted()) {
                txManager.rollback(transactionStatus);
            }
            logger.error("Error executing job with ID = {}", jobId, e);
        }
    }

    private void processCommand() {
        logger.debug("Start chain job: {}", jobId);

        jobStatusManager.addJob(jobId);

        jobInfo.put("chainjobstatus", jobId);

        JSONObject childJobInfo = null;
        try {
            JSONParser parser = new JSONParser();
            JSONArray chain = (JSONArray) parser.parse(jobs);

            int nChain = chain.size();
            jobInfo.put("childrencount", String.valueOf(nChain));

            for (Object aChain : chain) {
                String internalJobId = UUID.randomUUID().toString();

                // prep child job

                JSONObject job = (JSONObject) aChain;
                String excType = job.get("exectype").toString();
                String warnings = null;

                if (excType.equalsIgnoreCase("reflection")) {
                    // getting jobInfo from inside since it generates job id.
                    childJobInfo = execReflection(jobId, job, jobStatusManager);
                    Object oWarn = childJobInfo.get("warnings");
                    if (oWarn != null) {
                        warnings = oWarn.toString();
                    }
                }
                else if (excType.equalsIgnoreCase("reflection_sync")) {
                    childJobInfo = execReflectionSync(jobId, internalJobId, job, jobStatusManager);
                }
                else {
                    // create and set info
                    childJobInfo = createChildInfo(internalJobId, JobStatusManager.JOB_STATUS.RUNNING.toString());
                    setJobInfo(jobInfo, childJobInfo, childrenInfo, JobStatusManager.JOB_STATUS.RUNNING.toString(), "processing");
                    jobStatusManager.updateJob(jobId, jobInfo.toString());

                    JSONObject result = processJob(internalJobId, job);

                    // try to get warning
                    Object oWarn = result.get("warnings");
                    if (oWarn != null) {
                        warnings = oWarn.toString();
                    }
                }

                // if we have warnings then pass on
                String resDetail = "success";
                if (warnings != null) {
                    resDetail = "WARNINGS: " + warnings;
                }

                setJobInfo(jobInfo, childJobInfo, childrenInfo, JobStatusManager.JOB_STATUS.COMPLETE.toString(), resDetail);

                jobStatusManager.updateJob(jobId, jobInfo.toString());
            }

            jobStatusManager.setComplete(jobId, jobInfo.toString());
        }
        catch (Exception e) {
            logger.error("Error during processing command where jobId = {}, jobs = {}", jobId, jobs, e);

            if (childJobInfo != null) {
                setJobInfo(jobInfo, childJobInfo, childrenInfo, JobStatusManager.JOB_STATUS.FAILED.toString(), e.getMessage());
            }

            jobStatusManager.setFailed(jobId, jobInfo.toString());
        }
        finally {
            logger.debug("End process chain Job: {}", jobId);
        }
    }

    // this function is currently used by WfsManager only and WfsManager does
    // not perform any job tracking. It is raw class.
    private JSONObject execReflectionSync(String jobId, String childJobId, JSONObject job,
            JobStatusManager jobStatusManager) throws Exception {
        String className = job.get("class").toString();
        String methodName = job.get("method").toString();

        String internalJobId = (job.get("internaljobid") == null) ? null : job.get("internaljobid").toString();

        JSONArray paramsList = (JSONArray) job.get("params");

        Class<?>[] paramTypes = new Class[paramsList.size()];
        Object[] parameters = new Object[paramsList.size()];
        for (int i = 0; i < paramsList.size(); i++) {
            JSONObject param = (JSONObject) paramsList.get(i);
            String paramType = param.get("paramtype").toString();
            Object oIsPrim = param.get("isprimitivetype");
            if ((oIsPrim != null) && oIsPrim.toString().equalsIgnoreCase("true")) {
                Class<?> classWrapper = Class.forName(paramType);
                paramTypes[i] = (Class<?>) classWrapper.getField("TYPE").get(null);
            }
            else {
                paramTypes[i] = Class.forName(paramType);
            }
            parameters[i] = param.get("value");
        }

        Class<?> springBeanClass = Class.forName(className);

        ApplicationContext appContext = ApplicationContextUtils.getApplicationContext();
        Object springBeanInstance = appContext.getBean(springBeanClass);

        JSONObject childJobInfo;
        String currentChildJobId = childJobId;

        // May be we would need create interface to guarranttee that it will
        // return a job id?  Add internal job id to end of method call
        if (internalJobId != null) {
            currentChildJobId = internalJobId;
            childJobInfo = createChildInfo(currentChildJobId, JobStatusManager.JOB_STATUS.RUNNING.toString());
            setJobInfo(jobInfo, childJobInfo, childrenInfo, JobStatusManager.JOB_STATUS.RUNNING.toString(), "processing");
            jobStatusManager.updateJob(jobId, jobInfo.toString());

            Object[] newParams = new Object[paramsList.size() + 1];
            System.arraycopy(parameters, 0, newParams, 0, parameters.length);
            newParams[parameters.length] = internalJobId;

            Class<?>[] newParamTypes = new Class[paramsList.size() + 1];
            System.arraycopy(paramTypes, 0, newParamTypes, 0, paramsList.size());
            newParamTypes[parameters.length] = String.class;
            Method method = springBeanClass.getDeclaredMethod(methodName, newParamTypes);
            // This will blow if the method is not designed to handle job id
            method.invoke(springBeanInstance, newParams);
        }
        else {
            Method method = springBeanClass.getDeclaredMethod(methodName, paramTypes);
            Object oReflectJobId = method.invoke(springBeanInstance, parameters);
            if (oReflectJobId != null) {
                currentChildJobId = oReflectJobId.toString();
            }

            // Updating job status info. Looks like we need to wait till job is
            // done to get job id. With this we can not canel..
            childJobInfo = createChildInfo(currentChildJobId, JobStatusManager.JOB_STATUS.RUNNING.toString());
            setJobInfo(jobInfo, childJobInfo, childrenInfo, JobStatusManager.JOB_STATUS.RUNNING.toString(), "processing");
            jobStatusManager.updateJob(jobId, jobInfo.toString());
        }

        return childJobInfo;
    }

    private JSONObject execReflection(String jobId, JSONObject job, JobStatusManager jobStatusManager) throws Exception {
        JSONObject childJobInfo = execReflectionSync(jobId, null, job, jobStatusManager);
        Object oReflectJobId = childJobInfo.get("id");

        if (oReflectJobId != null) {
            String reflectionJobId = oReflectJobId.toString();
            boolean isDone = false;
            // If we do not get any Job Status then try 10 time and give up
            int nRetryCnt = 0;
            while (!isDone) {
                JSONObject status = getJobStatusObj(reflectionJobId);
                if (status != null) {
                    String currStat = status.get("status").toString();
                    if (!currStat.equalsIgnoreCase("running")) {
                        isDone = true;
                        if (currStat.equals(JobStatusManager.JOB_STATUS.FAILED.toString())) {
                            throw new Exception(status.get("statusDetail").toString());
                        }
                        if (status.get("statusDetail") != null) {
                            childJobInfo.put("warnings", status.get("statusDetail").toString());
                            childJobInfo.put("detail", "warning");
                            childJobInfo.put("status", JobStatusManager.JOB_STATUS.COMPLETE.toString());
                        }
                    }
                }
                else {
                    try {
                        Thread.sleep(1000);
                    }
                    catch (InterruptedException ignored) {
                        isDone = true;
                    }

                    if (nRetryCnt > 10) {
                        // isDone = true;
                    }
                    nRetryCnt++;
                }
                if (!isDone) {
                    try {
                        Thread.sleep(CHAIN_JOS_STATUS_PING_INTERVAL_VALUE);
                    }
                    catch (InterruptedException ignored) {
                        //
                    }
                }
            }
        }

        return childJobInfo;
    }

    private JSONObject processJob(String jobId, JSONObject command) throws NativeInterfaceException {
        logger.debug("processing Job: {}", jobId);

        command.put("jobId", jobId);

        String resourceName = command.get("caller").toString();
        JobFieldsValidator validator = new JobFieldsValidator(resourceName);

        Map<String, String> paramsMap = JsonUtils.paramsToMap(command);

        List<String> missingList = new ArrayList<>();
        if (!validator.validateRequiredExists(paramsMap, missingList)) {
            logger.error("Missing following required field(s): {}", missingList);
        }

        logger.debug("calling native request Job: {}", jobId);

        return jobExecMan.exec(command);
    }

    private JSONObject getJobStatusObj(String jobId) {
        JSONObject status = new JSONObject();

        status.put("jobId", jobId);
        JobStatus jobStatusObj = jobStatusManager.getJobStatusObj(jobId);
        if (jobStatusObj == null) {
            return null;
        }

        JobStatusManager.JOB_STATUS stat = JobStatusManager.JOB_STATUS.fromInteger(jobStatusObj.getStatus());
        if (stat == null) {
            stat = JobStatusManager.JOB_STATUS.UNKNOWN;
        }

        status.put("status", stat.toString());
        status.put("statusDetail", jobStatusObj.getStatusDetail());

        return status;
    }

    private static void setJobInfo(JSONObject jobInfo, JSONObject child, JSONArray children, String stat, String detail) {
        for (Object aChildren : children) {
            JSONObject c = (JSONObject) aChildren;
            if (c.get("id").toString().equals(child.get("id").toString())) {
                c.put("status", stat);
                c.put("detail", detail);
                return;
            }
        }

        child.put("status", stat);
        child.put("detail", detail);
        children.add(child);
        jobInfo.put("children", children);
    }

    private static JSONObject createChildInfo(String id, String stat) {
        JSONObject child = new JSONObject();
        child.put("id", id);
        child.put("status", stat);

        return child;
    }
}
