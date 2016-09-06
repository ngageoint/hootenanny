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
package hoot.services.nativeinterfaces;

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
    private static final Logger logger = LoggerFactory.getLogger(JobExecutionManager.class);

    private NativeInterface nativeInterface;

    /**
     * @param nativeInterface
     */
    public void setNativeInterface(NativeInterface nativeInterface) {
        this.nativeInterface = nativeInterface;
    }

    public JobExecutionManager() {}

    public String getProgress(String jobId) throws NativeInterfaceException {
        try {
            return nativeInterface.getJobProgress(jobId);
        }
        catch (Exception e) {
            throw new NativeInterfaceException(e.getMessage(), NativeInterfaceException.HttpCode.SERVER_ERROR, e);
        }
    }

    /**
     * Execute job using associated Native Interface class.
     *
     * @param command
     * @throws NativeInterfaceException
     */
    public JSONObject exec(JSONObject command) throws NativeInterfaceException {
        return execWithResult(command);
    }

    public JSONObject execWithResult(JSONObject command) throws NativeInterfaceException {
        try {
            return nativeInterface.exec(command);
        }
        catch (NativeInterfaceException ne) {
            throw ne;
        }
        catch (Exception e) {
            throw new NativeInterfaceException(e.getMessage(), NativeInterfaceException.HttpCode.SERVER_ERROR, e);
        }
    }
}
