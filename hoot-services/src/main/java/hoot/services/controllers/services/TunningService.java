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
import java.sql.Connection;
import java.util.Collection;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.UUID;

import javax.ws.rs.core.Response.Status;

import org.json.simple.JSONObject;
import org.openstreetmap.osmosis.core.container.v0_6.EntityContainer;
import org.openstreetmap.osmosis.core.domain.v0_6.Entity;
import org.openstreetmap.osmosis.core.domain.v0_6.Node;
import org.openstreetmap.osmosis.core.domain.v0_6.Relation;
import org.openstreetmap.osmosis.core.domain.v0_6.RelationMember;
import org.openstreetmap.osmosis.core.domain.v0_6.Tag;
import org.openstreetmap.osmosis.core.domain.v0_6.Way;
import org.openstreetmap.osmosis.core.domain.v0_6.WayNode;
import org.openstreetmap.osmosis.core.task.v0_6.RunnableSource;
import org.openstreetmap.osmosis.core.task.v0_6.Sink;
import org.openstreetmap.osmosis.xml.common.CompressionMethod;
import org.openstreetmap.osmosis.xml.v0_6.XmlReader;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.HootProperties;
import hoot.services.command.CommandResult;
import hoot.services.command.CommandRunner;
import hoot.services.command.ICommandRunner;
import hoot.services.db.DbUtils;
import hoot.services.job.Executable;
import hoot.services.utils.FileUtils;
import hoot.services.utils.ResourceErrorHandler;


public class TunningService implements Executable {

    private static final Logger logger = LoggerFactory.getLogger(TunningService.class);

    private String finalStatusDetail;
    private Double totalSize = 0.0;
    private final String tempPath = HootProperties.getProperty("tempOutputPath");
    private final String coreScriptPath = HootProperties.getProperty("coreScriptPath");

    @Override
    public String getFinalStatusDetail() {
        return finalStatusDetail;
    }

    public TunningService() {
    }

    @Override
    public void exec(JSONObject command) throws Exception {
        JSONObject res = new JSONObject();
        String input = command.get("input").toString();
        String inputtype = command.get("inputtype").toString();
        Connection conn = DbUtils.createConnection();
        long starttime = new Date().getTime();

        //?????
        UUID.randomUUID().toString();

        try {
            String tempOutputPath = "";
            if (inputtype.equalsIgnoreCase("db")) {
                DbUtils.getNodesCountByName(conn, input);
                DbUtils.getWayCountByName(conn, input);
                DbUtils.getRelationCountByName(conn, input);

                // if the count is greater than threshold then just use it and tell it too big
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
                if (!FileUtils.validateFilePath(tempPath, tempOutputPath)) {
                    throw new Exception("input can not contain path.");
                }
            }
            else {
                tempOutputPath = input;
            }

            File outputFile = new File(tempOutputPath);

            JobSink sinkImplementation = parseOsm(outputFile);

            long endTime = new Date().getTime();
            logger.debug("Start:{}  - End: {} Diff:{} TOTAL:{} NODES:{} Way:{} Relations:{}",
                    starttime, endTime, endTime - starttime, totalSize, sinkImplementation.getTotalNodes(),
                    sinkImplementation.getTotalWay(), sinkImplementation.getTotalRelation());

            res.put("EstimatedSize", totalSize * 15);
            res.put("NodeCount", sinkImplementation.getTotalNodes());
            res.put("WayCount", sinkImplementation.getTotalWay());
            res.put("RelationCount", sinkImplementation.getTotalRelation());
        }
        catch (Exception ex) {
            ResourceErrorHandler.handleError("Tuning Service error: " + ex, Status.INTERNAL_SERVER_ERROR, logger);
        }
        finally {
            DbUtils.closeConnection(conn);
        }

        finalStatusDetail = res.toString();
    }

    private JobSink parseOsm(File inputOsmFile) throws Exception {
        CompressionMethod compression = CompressionMethod.None;

        RunnableSource reader = new XmlReader(inputOsmFile, false, compression);
        Thread readerThread = new Thread(reader);

        JobSink sink = new JobSink();
        Map<String, Object> sinkParam = new HashMap<>();
        sink.initialize(sinkParam);

        reader.setSink(sink);

        readerThread.start();

        while (readerThread.isAlive()) {
            try {
                readerThread.join();
            }
            catch (InterruptedException ignored) {
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

    private static class JobSink implements Sink {
        @Override
        public void release() {
        }

        @Override
        public void complete() {
        }

        private Double totalOSMsize = (double) 0;
        private Double totalNodeCnt = (double) 0;
        private Double totalWayCnt = (double) 0;
        private Double totalRelationCnt = (double) 0;

        @Override
        public void initialize(Map<String, Object> metaData) {
        }

        double getTotalSize() {
            return totalOSMsize;
        }

        double getTotalNodes() {
            return totalNodeCnt;
        }

        double getTotalWay() {
            return totalWayCnt;
        }

        double getTotalRelation() {
            return totalRelationCnt;
        }

        double calcTagsByteSize(Collection<Tag> tags){
            double totalBytes = 0;
            for (Tag curTag : tags) {
                String tagKey = curTag.getKey();
                String tagVal = curTag.getValue();

                totalBytes += tagKey.length();
                totalBytes += tagVal.length();
            }
            return totalBytes;
        }

        @Override
        public void process(EntityContainer entityContainer) {
            Entity entity = entityContainer.getEntity();

            if (entity instanceof Node) {
                totalOSMsize += 72 + calcTagsByteSize(entity.getTags());
                totalNodeCnt++;
            }
            else if (entity instanceof Way) {
                Double nByteSize = 96 + calcTagsByteSize(entity.getTags());

                List<WayNode> wayNodes = ((Way) entity).getWayNodes();

                for (WayNode wayNode : wayNodes) {
                    String wnID = String.valueOf(wayNode.getNodeId());
                    nByteSize += wnID.length();
                }

                totalOSMsize += nByteSize;
                totalWayCnt++;
            }
            else if (entity instanceof Relation) {
                Double nByteSize = 64 + calcTagsByteSize(entity.getTags());

                List<RelationMember> relationMembers = ((Relation) entity).getMembers();
                for (RelationMember relationMember : relationMembers) {
                    nByteSize += relationMember.getMemberRole().length();
                    nByteSize += 8;
                }

                totalOSMsize += nByteSize;
                totalRelationCnt++;
            }
        }
    }
}
