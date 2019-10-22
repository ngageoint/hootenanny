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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.command;


import static hoot.services.HootProperties.CONFIG_OPTIONS;
import static hoot.services.HootProperties.HOME_FOLDER;

import java.io.File;
import java.io.IOException;
import java.nio.charset.Charset;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import org.apache.commons.io.FileUtils;

import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.ObjectMapper;

import hoot.services.ApplicationContextUtils;
import hoot.services.HootProperties;


public abstract class ExternalCommand implements Command {

    private String caller;
    private String command;
    private Map<String, ?> substitutionMap;
    private File workDir;
    private Boolean trackable;
    protected static Map<String, Map<String, String>> configOptions = null;

    static {
        try {
            ObjectMapper mapper = new ObjectMapper();
            TypeReference<?> schema = new TypeReference<Map<String, Map<String, Object>>>(){};
            // get json of all config options...
            String file = FileUtils.readFileToString(new File(HOME_FOLDER, CONFIG_OPTIONS), Charset.defaultCharset());
            configOptions = mapper.readValue(file, schema);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    protected String jobId;

    protected ExternalCommand() {}

    protected ExternalCommand(String jobId) {
        this.jobId = jobId;
    }

    protected void configureCommand(String command, Map<String, ?> substitutionMap, Class<?> caller) {
        this.caller = caller.getName();
        this.command = command;
        this.substitutionMap = substitutionMap;
        this.workDir = new File(HootProperties.TEMP_OUTPUT_PATH);
        this.trackable = Boolean.TRUE;
    }

    protected void configureCommand(String command, Map<String, ?> substitutionMap, Class<?> caller, File workDir) {
        this.caller = caller.getName();
        this.command = command;
        this.substitutionMap = substitutionMap;
        this.workDir = workDir;
        this.trackable = Boolean.TRUE;
    }

    protected void configureCommand(String command, Map<String, ?> substitutionMap, Class<?> caller, Boolean trackable) {
        this.configureCommand(command, substitutionMap, caller);
        this.trackable = trackable;
    }

    protected void configureCommand(String command, Map<String, ?> substitutionMap, Class<?> caller, File workDir, Boolean trackable) {
        this.configureCommand(command, substitutionMap, caller, workDir);
        this.trackable = trackable;
    }

    protected void configureCommand(String command, Class<?> caller, Boolean trackable) {
        this.configureCommand(command, new HashMap<>(), caller);
        this.trackable = trackable;
    }

    protected void configureCommand(String command, Class<?> caller, File workDir, Boolean trackable) {
        this.configureCommand(command, new HashMap<>(), caller, workDir);
        this.trackable = trackable;
    }

    protected static List<String> toHootOptions(List<String> options) {
        List<String> hootOptions = new LinkedList<>();
        options.forEach(option -> {
            if (option.endsWith(".conf")) {
                hootOptions.add("-C");
            } else {
                hootOptions.add("-D");
            }
            hootOptions.add(option);
        });
        return hootOptions;
    }

    public String getCaller() {
        return caller;
    }

    public String getCommand() {
        return command;
    }

    public Map<String, ?> getSubstitutionMap() {
        return substitutionMap;
    }

    public File getWorkDir() {
        return workDir;
    }

    public Boolean getTrackable() {
        return trackable;
    }

    public void setCaller(String caller) {
        this.caller = caller;
    }

    public void setCommand(String command) {
        this.command = command;
    }

    public void setSubstitutionMap(Map<String, ?> substitutionMap) {
        this.substitutionMap = substitutionMap;
    }

    public void setWorkDir(File workDir) {
        this.workDir = workDir;
    }

    public void setTrackable(Boolean trackable) {
        this.trackable = trackable;
    }

    @Override
    public CommandResult execute() {
        ExternalCommandManager externalCommandManager =
                ApplicationContextUtils.getApplicationContext().getBean(ExternalCommandManager.class);

        if (externalCommandManager == null) {
            throw new IllegalStateException("Could not find bean: " + ExternalCommandManager.class.getName());
        }

        if (this.getJobId() != null) {
            return externalCommandManager.exec(getJobId(),this);
        }
        else {
            return externalCommandManager.exec(this);
        }
    }

    public String getJobId() {
        return jobId;
    }

    public void setJobId(String jobId) {
        this.jobId = jobId;
    }

    @Override
    public String toString() {
        return "ExternalCommand{" +
                "caller='" + caller + '\'' +
                ", command='" + command + '\'' +
                ", substitutionMap=" + substitutionMap +
                ", workDir=" + workDir +
                ", trackable=" + trackable +
                ", jobId='" + jobId + '\'' +
                '}';
    }
}
