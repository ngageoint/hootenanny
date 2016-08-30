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
package hoot.services.job;

import static hoot.services.HootProperties.CORE_SCRIPT_PATH;
import static hoot.services.HootProperties.TEMP_OUTPUT_PATH;

import java.io.File;
import java.sql.Connection;
import java.util.Collection;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

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

import hoot.services.command.CommandResult;
import hoot.services.command.CommandRunner;
import hoot.services.command.CommandRunnerImpl;
import hoot.services.utils.DbUtils;
import hoot.services.utils.FileUtils;


public class TunningService implements Executable {
    private static final Logger logger = LoggerFactory.getLogger(TunningService.class);

    private String finalStatusDetail;
    private Double totalSize;

    @Override
    public String getFinalStatusDetail() {
        return finalStatusDetail;
    }

    public TunningService() {}

    @Override
    public void exec(JSONObject command) {
        JSONObject json = new JSONObject();
        String input = command.get("input").toString();
        String inputType = command.get("inputtype").toString();
        long startTime = new Date().getTime();

        try (Connection connection = DbUtils.createConnection()) {
            String tempOutputPath;
            if (inputType.equalsIgnoreCase("db")) {
                DbUtils.getNodesCountByName(connection, input);
                DbUtils.getWayCountByName(connection, input);
                DbUtils.getRelationCountByName(connection, input);

                // if the count is greater than threshold then just use it and tell it too big
                CommandRunner cmdRunner = new CommandRunnerImpl();

                String commandArr = "make -f " + CORE_SCRIPT_PATH +
                        "/makeconvert INPUT=" + input + " OUTPUT=" + TEMP_OUTPUT_PATH
                        + "/" + input + ".osm";

                CommandResult result = cmdRunner.exec(commandArr);

                if (result.getExitStatus() != 0 /* 0 means SUCCESS*/) {
                    throw new RuntimeException(result.getStderr());
                }

                tempOutputPath = TEMP_OUTPUT_PATH + "/" + input + ".osm";

                // fortify fix
                if (!FileUtils.validateFilePath(TEMP_OUTPUT_PATH, tempOutputPath)) {
                    throw new RuntimeException("Input can not contain path.");
                }
            }
            else {
                tempOutputPath = input;
            }

            File outputFile = new File(tempOutputPath);
            JobSink sinkImplementation = this.parseOsm(outputFile);

            long endTime = new Date().getTime();

            logger.debug("Start:{} - End: {} Diff:{} TOTAL:{} NODES:{} Way:{} Relations:{}",
                    startTime, endTime, endTime - startTime, totalSize, sinkImplementation.getTotalNodes(),
                    sinkImplementation.getTotalWay(), sinkImplementation.getTotalRelation());

            json.put("EstimatedSize", totalSize * 15);
            json.put("NodeCount", sinkImplementation.getTotalNodes());
            json.put("WayCount", sinkImplementation.getTotalWay());
            json.put("RelationCount", sinkImplementation.getTotalRelation());
        }
        catch (Exception e) {
            String msg = "Tuning Service error: " + e.getMessage();
            throw new RuntimeException(msg, e);
        }

        this.finalStatusDetail = json.toString();
    }

    private JobSink parseOsm(File inputOsmFile) {
        CompressionMethod compression = CompressionMethod.None;

        RunnableSource reader = new XmlReader(inputOsmFile, false, compression);

        JobSink sink = new JobSink();
        Map<String, Object> sinkParam = new HashMap<>();
        sink.initialize(sinkParam);

        reader.setSink(sink);

        Thread readerThread = new Thread(reader);
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

    private static class JobSink implements Sink {
        @Override
        public void release() {}

        @Override
        public void complete() {}

        private Double totalOSMsize;
        private Double totalNodeCnt;
        private Double totalWayCnt;
        private Double totalRelationCnt;

        @Override
        public void initialize(Map<String, Object> metaData) {}

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

        static double calcTagsByteSize(Collection<Tag> tags){
            double totalBytes = 0;
            for (Tag tag : tags) {
                String tagKey = tag.getKey();
                String tagVal = tag.getValue();

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
                Double bytes = 96 + calcTagsByteSize(entity.getTags());

                List<WayNode> wayNodes = ((Way) entity).getWayNodes();

                for (WayNode wayNode : wayNodes) {
                    String id = String.valueOf(wayNode.getNodeId());
                    bytes += id.length();
                }

                totalOSMsize += bytes;
                totalWayCnt++;
            }
            else if (entity instanceof Relation) {
                Double bytes = 64 + calcTagsByteSize(entity.getTags());

                List<RelationMember> relationMembers = ((Relation) entity).getMembers();
                for (RelationMember relationMember : relationMembers) {
                    bytes += relationMember.getMemberRole().length();
                    bytes += 8;
                }

                totalOSMsize += bytes;
                totalRelationCnt++;
            }
        }
    }
}
