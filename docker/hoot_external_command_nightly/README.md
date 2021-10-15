# Overview

This Dockerfile builds an image with the latest nightly version of the Hootenanny core from RPM's and sets up Flask for running conflations on command.

NOTE:
* This **does not** include the UI
* This **does not** setup a user in the container.


# Testing

1) In a terminal, run the `run_image.sh` script
  This will:
  * Mount the current directory onto the container as `/hoot_data`
  * Start the server listening on port 5000

2) In a different terminal, run the `test_image.sh` script
  You should see output like this on the terminal:
{
"key": "4b45ebc3",
"result_url": "http://127.0.0.1:5000/scripts/RunCommandsAcrossContainers?key=4b45ebc3&wait=false",
"status": "running"
}

 You can check the job status by running `curl` with the `result_url`
````
dev:hoot_external_command_nightly $ curl "http://127.0.0.1:5000/scripts/RunCommandsAcrossContainers?key=4b45ebc3&wait=false"
{
  "end_time": 1634215174.1849353,
  "error": "",
  "key": "4b45ebc3",
  "process_time": 1.0444128513336182,
  "report": "12:39:33.505 STATUS ...pp/hoot/core/util/Progress.cpp(  81) Conflate (0%): Conflating .../hoot_data/ToyTestA.osm with .../hoot_data/ToyTestB.osm and writing the output to .../hoot_data/ToyOutput.osm...\n12:39:33.505 STATUS ...pp/hoot/core/util/Progress.cpp(  81) Conflate

  <--- removed wall of text --->

  ToyOutput.osm...\n12:39:34.163 STATUS ...pp/hoot/core/util/Progress.cpp(  81) Conflate (100%): Conflation job completed in 00:00 for reference map: .../hoot_data/ToyTestA.osm and secondary map: .../hoot_data/ToyTestB.osm and written to output: .../hoot_data/ToyOutput.osm\n",
  "returncode": 0,
  "start_time": 1634215173.1405225
}

```

4) When the command finishes, you will see a file called "ToyOutput.osm" in the current directory. This is the conflation of ToyTestA.osm and ToyTestB.osm

