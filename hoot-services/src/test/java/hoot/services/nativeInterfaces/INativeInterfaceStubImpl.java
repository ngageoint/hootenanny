package hoot.services.nativeInterfaces;

import org.json.simple.JSONObject;


public class INativeInterfaceStubImpl implements INativeInterface {
    @Override
    public JSONObject exec(JSONObject command) throws NativeInterfaceException {
        JSONObject json = new JSONObject();

        // hoot version --debug
        String coreVersionWithDebugInfo = "Hootenanny 0.2.23_1036_ga13f8a9_dirty Built By: vagrant\n"
                + "17:14:43.796 DEBUG ...p/hoot/core/cmd/VersionCmd.cpp(  81) GEOS Version: 3.4.2\n"
                + "17:14:43.796 DEBUG ...p/hoot/core/cmd/VersionCmd.cpp(  82) GDAL Version: 1.10.1\n"
                + "17:14:43.797 DEBUG ...p/hoot/core/cmd/VersionCmd.cpp(  83) GLPK Version: 4.52\n"
                + "17:14:43.797 DEBUG ...p/hoot/core/cmd/VersionCmd.cpp(  84) Qt Version: 4.8.6\n"
                + "17:14:43.797 DEBUG ...p/hoot/core/cmd/VersionCmd.cpp(  88) Boost Version: 1.54.0\n"
                + "17:14:43.797 DEBUG ...p/hoot/core/cmd/VersionCmd.cpp(  90) STXXL Version: STXXL v1.3.1 (release)\n"
                + "17:14:43.797 DEBUG ...p/hoot/core/cmd/VersionCmd.cpp(  93) CppUnit Version: 1.13.1\n"
                + "17:14:43.797 DEBUG ...p/hoot/core/cmd/VersionCmd.cpp(  96) Memory usage, vm: 402.3MB rss: 35.93MB\n";

        json.put("stdout", coreVersionWithDebugInfo);
        return json;
    }

    @Override
    public void terminate(String jobId) throws NativeInterfaceException {

    }

    @Override
    public String getJobProgress(String jobId) {
        return null;
    }

    public void init() {
        //
    }

    public void destroy() {
        //
    }
}
