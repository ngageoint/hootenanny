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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.services;

import java.io.File;
import java.io.IOException;
import java.sql.Connection;
import java.util.Collection;
import java.util.Date;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.UUID;

import javax.ws.rs.core.Response.Status;

import org.json.simple.JSONObject;
import org.openstreetmap.osmosis.core.container.v0_6.EntityContainer;
import org.openstreetmap.osmosis.core.domain.v0_6.Entity;
import org.openstreetmap.osmosis.core.domain.v0_6.RelationMember;
import org.openstreetmap.osmosis.core.domain.v0_6.Tag;
import org.openstreetmap.osmosis.core.domain.v0_6.WayNode;
import org.openstreetmap.osmosis.core.task.v0_6.RunnableSource;
import org.openstreetmap.osmosis.core.task.v0_6.Sink;
import org.openstreetmap.osmosis.xml.common.CompressionMethod;
import org.openstreetmap.osmosis.xml.v0_6.XmlReader;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.support.ClassPathXmlApplicationContext;

import hoot.services.HootProperties;
import hoot.services.command.CommandResult;
import hoot.services.command.CommandRunner;
import hoot.services.command.ICommandRunner;
import hoot.services.db.DbUtils;
import hoot.services.job.Executable;
import hoot.services.utils.ResourceErrorHandler;


public class TunningService implements Executable {

    private static final Logger log = LoggerFactory.getLogger(TunningService.class);
    @SuppressWarnings("unused")
    private ClassPathXmlApplicationContext appContext;
    private String finalStatusDetail;
    private Double totalSize = null;
    private String tempPath = null;
    private String coreScriptPath = null;

    @Override
    public String getFinalStatusDetail() {
        return finalStatusDetail;
    }

    public TunningService() throws IOException {
        totalSize = new Double(0);
        appContext = new ClassPathXmlApplicationContext(new String[] { "db/spring-database.xml" });
        tempPath = HootProperties.getProperty("tempOutputPath");
        coreScriptPath = HootProperties.getProperty("coreScriptPath");
    }

    @Override
    public void exec(JSONObject command) throws Exception {

        JSONObject res = new JSONObject();
        String input = command.get("input").toString();
        String inputtype = command.get("inputtype").toString();
        Connection conn = DbUtils.createConnection();
        long starttime = new Date().getTime();
        UUID.randomUUID().toString();
        try {
            String tempOutputPath = "";
            if (inputtype.equalsIgnoreCase("db")) {
                DbUtils.getNodesCountByName(conn, input);
                DbUtils.getWayCountByName(conn, input);
                DbUtils.getRelationCountByName(conn, input);

                // if the count is greater than threshold then just use it and
                // tell it too big

                ICommandRunner cmdRunner = new CommandRunner();

                String commandArr = "make -f " + coreScriptPath + "/makeconvert INPUT=" + input + " OUTPUT=" + tempPath
                        + "/" + input + ".osm";
                CommandResult result = cmdRunner.exec(commandArr);

                if (result.getExitStatus() == 0) {
                    result.getStdout();
                }
                else {
                    String err = result.getStderr();
                    throw new Exception(err);
                }

                tempOutputPath = tempPath + "/" + input + ".osm";

                // fortify fix
                if (!hoot.services.utils.FileUtils.validateFilePath(tempPath, tempOutputPath)) {
                    throw new Exception("input can not contain path.");
                }
            }
            else {
                tempOutputPath = input;
            }

            File outputFile = new File(tempOutputPath);

            JobSink sinkImplementation = parseOsm(outputFile);

            long endTime = new Date().getTime();
            log.debug("Start:" + starttime + "  - End: " + endTime + " Diff:" + (endTime - starttime) + " TOTAL:"
                    + totalSize + " NODES:" + sinkImplementation.getTotalNodes() + " Way:"
                    + sinkImplementation.getTotalWay() + " Relations:" + sinkImplementation.getTotalRelation());

            res.put("EstimatedSize", totalSize * 15);
            res.put("NodeCount", sinkImplementation.getTotalNodes());
            res.put("WayCount", sinkImplementation.getTotalWay());
            res.put("RelationCount", sinkImplementation.getTotalRelation());
        }
        catch (Exception ex) {
            ResourceErrorHandler.handleError("Tuning Service error: " + ex.toString(), Status.INTERNAL_SERVER_ERROR,
                    log);
        }
        finally {
            DbUtils.closeConnection(conn);
        }

        finalStatusDetail = res.toString();

    }

    private JobSink parseOsm(File inputOsmFile) throws Exception {
        CompressionMethod compression = CompressionMethod.None;

        RunnableSource reader;
        reader = new XmlReader(inputOsmFile, false, compression);
        Thread readerThread = new Thread(reader);

        JobSink sink = new JobSink();
        Map<String, Object> sinkParam = new HashMap<String, Object>();
        sink.initialize(sinkParam);

        reader.setSink(sink);

        readerThread.start();

        while (readerThread.isAlive()) {
            try {
                readerThread.join();
            }
            catch (InterruptedException e) {
                //
            }
        }

        totalSize = sink.getTotalSize();

        return sink;

    }

    /**
     * see CoreServiceContext.xml
     */
    public void init() {
        //
    }

    /**
     * see CoreServiceContext.xml
     */
    public void destroy() {
        //
    }

    public class JobSink implements Sink {

        @Override
        public void release() {
            /**/ }

        @Override
        public void complete() {
            //
        }

        private Double totalOSMsize = (double) 0;
        private Double totalNodeCnt = (double) 0;
        private Double totalWayCnt = (double) 0;
        private Double totalRelationCnt = (double) 0;

        @Override
        public void initialize(Map<String, Object> arg0) {
            //
        }

        public double getTotalSize() {
            return totalOSMsize.doubleValue();
        }

        public double getTotalNodes() {
            return totalNodeCnt.doubleValue();
        }

        public double getTotalWay() {
            return totalWayCnt.doubleValue();
        }

        public double getTotalRelation() {
            return totalRelationCnt.doubleValue();
        }

        @Override
        public void process(EntityContainer arg0) {

            Entity entity = arg0.getEntity();

            if (entity instanceof org.openstreetmap.osmosis.core.domain.v0_6.Node) {

                Double nByteSize = (double) 72;
                Collection<Tag> t = entity.getTags();
                for (Iterator iterator = t.iterator(); iterator.hasNext();) {
                    Tag curTag = (Tag) iterator.next();
                    String tagKey = curTag.getKey();
                    String tagVal = curTag.getValue();

                    nByteSize += tagKey.length();
                    nByteSize += tagVal.length();

                }

                totalOSMsize += nByteSize.doubleValue();
                totalNodeCnt++;

            }
            else if (entity instanceof org.openstreetmap.osmosis.core.domain.v0_6.Way) {
                Double nByteSize = (double) 96;
                Collection<Tag> t = entity.getTags();
                for (Iterator iterator = t.iterator(); iterator.hasNext();) {
                    Tag curTag = (Tag) iterator.next();
                    String tagKey = curTag.getKey();
                    String tagVal = curTag.getValue();

                    nByteSize += tagKey.length();
                    nByteSize += tagVal.length();

                }

                List<org.openstreetmap.osmosis.core.domain.v0_6.WayNode> ways = ((org.openstreetmap.osmosis.core.domain.v0_6.Way) entity)
                        .getWayNodes();

                for (int i = 0; i < ways.size(); i++) {
                    WayNode wn = ways.get(i);
                    String wnID = "" + wn.getNodeId();
                    nByteSize += wnID.length();
                }

                totalOSMsize += nByteSize.doubleValue();
                totalWayCnt++;

            }
            else if (entity instanceof org.openstreetmap.osmosis.core.domain.v0_6.Relation) {
                Double nByteSize = (double) 64;

                Collection<Tag> t = entity.getTags();
                for (Iterator iterator = t.iterator(); iterator.hasNext();) {
                    Tag curTag = (Tag) iterator.next();
                    String tagKey = curTag.getKey();
                    String tagVal = curTag.getValue();

                    nByteSize += tagKey.length();
                    nByteSize += tagVal.length();

                }

                List<RelationMember> mems = ((org.openstreetmap.osmosis.core.domain.v0_6.Relation) entity).getMembers();
                for (int i = 0; i < mems.size(); i++) {
                    RelationMember m = mems.get(i);

                    nByteSize += m.getMemberRole().length();
                    nByteSize += 8;
                }
                totalOSMsize += nByteSize.doubleValue();
                totalRelationCnt++;
            }

        }

    }
}
