
As of Hootenanny v0.2.39, this code is no longer supported or compiled. Conflation with Hadoop has been discontinued due lack of use and the
involved maintenance costs.

-----------------------------------------------------------------------------------

Instructions

Be sure to set LD_LIBRARY_PATH to include the hoot-hadoop directory. E.g.

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HOOT_HOME/hoot-hadoop/

To build: "mvn package"

== Eclipse ==

To create eclipse project: "mvn eclipse:eclipse"

To run the eclipse project you must setup a run configuration that sets LD_LIBRARY_PATH appropriately.
