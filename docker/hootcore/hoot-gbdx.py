# Run a Hootenanny translation using the GbdxTaskInterface

import os
import subprocess

from gbdx_task_interface import GbdxTaskInterface

class HootGbdxTask(GbdxTaskInterface):

    def invoke(self):
        # Get input
        # NOTE: We could/should do this in a loop. Are we expecting more than one file?
        inputDir = self.get_input_data_port('geojson')

        fileName = os.listdir(inputDir)[0]

        fName,ext = os.path.splitext(fileName)

        # Hoot needs *.geojson.  Not *.json
        if ext == '.json':
            os.rename(os.path.join(inputDir,fileName),os.path.join(inputDir,fName) + '.geojson')
            fileName = fName + '.geojson'

        inputFile = os.path.join(inputDir,fileName)

        # Get output
        outputDir = self.get_output_data_port('data_out')

        if not os.path.exists(outputDir):
            os.makedirs(outputDir)

        # NOTE: This is the same name as the input
        outputFile = os.path.join(outputDir,fileName)

        hootCmd = ['hoot','convert','--error',
                   '-D','convert.ops=hoot::TranslationOp',
                   '-D','translation.script=/var/lib/hootenanny/translations/GBDX.js']

        hootCmd.append(inputFile)
        hootCmd.append(outputFile)

        hootOut = ''
        # Very basic error checking
        try:
            hootOut = subprocess.check_output(hootCmd)

        except subprocess.CalledProcessError, e:
            self.status = 'failed'
            self.reason = e.output

        # with "--error" the Hoot command shoud not have any output. If it does then
        # it is an error.
        if len(hootOut) > 0:
            self.status = 'failed'
            self.reason = hootOut


if __name__ == "__main__":
    with HootGbdxTask() as task:
        task.invoke()
