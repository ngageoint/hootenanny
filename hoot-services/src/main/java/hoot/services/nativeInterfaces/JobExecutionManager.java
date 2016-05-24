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
package hoot.services.nativeInterfaces;

import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;


/**
 * @author Jong Choi
 *
 *         The purpose of this class is to provide one additional indirection
 *         for managing Native Interface execution. This class is created by
 *         spring in CoreServiceContext.xml. Also note that nativeInterface
 *         member is assicated with this class in spring also. This class also
 *         manages the job status tracking through jobStatusManager memeber.
 *
 */
public class JobExecutionManager {
    @SuppressWarnings("unused")
    private static final Logger log = LoggerFactory.getLogger(JobExecutionManager.class);

    private INativeInterface nativeInterface;

    public INativeInterface getNativeInterface() {
        return nativeInterface;
    }

    /**
     * @param nativeInterface
     */
    public void setNativeInterface(INativeInterface nativeInterface) {
        this.nativeInterface = nativeInterface;
    }

    public JobExecutionManager() {

    }

    /**
     * Execute job using associated Native Interface class.
     *
     * @param command
     * @throws NativeInterfaceException
     */
    public JSONObject exec(JSONObject command) throws Exception {
        return execWithResult(command);
    }

    public void terminate(String jobId) throws Exception {
        try {
            getNativeInterface().terminate(jobId);
        }
        catch (NativeInterfaceException ne) {
            throw ne;
        }
        catch (Exception e) {
            throw new NativeInterfaceException(e.getMessage(), NativeInterfaceException.HttpCode.SERVER_ERROR);
        }
    }

    public JSONObject execWithResult(JSONObject command) throws Exception {
        JSONObject ret = null;
        try {
            ret = getNativeInterface().exec(command);
        }
        catch (NativeInterfaceException ne) {
            throw ne;
        }
        catch (Exception e) {
            throw new NativeInterfaceException(e.getMessage(), NativeInterfaceException.HttpCode.SERVER_ERROR);
        }
        return ret;
    }

    public String getProgress(String jobId) throws Exception {
        String progress = "";
        try {
            progress = getNativeInterface().getJobProgress(jobId);
        }
        catch (Exception e) {
            throw new NativeInterfaceException(e.getMessage(), NativeInterfaceException.HttpCode.SERVER_ERROR);
        }
        return progress;
    }

    /**
     * See CoreServiceContext.xml
     */
    public void destroy() {
        //
    }

    /**
     * See CoreServiceContext.xml
     */
    public void init() {
        //
    }
}
