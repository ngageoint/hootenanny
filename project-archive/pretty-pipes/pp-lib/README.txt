
Be sure to set LD_LIBRARY_PATH to include the hoot-hadoop directory. E.g.

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HOOT_HOME/hoot-hadoop/

To build: "mvn package"

== Eclipse ==

To create eclipse project: "mvn eclipse:eclipse"

To run the eclipse project you must setup a run configuration that sets LD_LIBRARY_PATH appropriately.
