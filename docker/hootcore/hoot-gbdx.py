# Run a Hootenanny translation using the GbdxTaskInterface

import os
import subprocess
import shutil
import zipfile

from gbdx_task_interface import GbdxTaskInterface

class HootGbdxTask(GbdxTaskInterface):

    # Nice function from StackOverflow to increment a filename if it exists
    def checkFile(self,path):
        path = os.path.expanduser(path)
        if not os.path.exists(path):
            return path

        root, ext = os.path.splitext(os.path.expanduser(path))
        dir       = os.path.dirname(root)
        fname     = os.path.basename(root)
        candidate = fname+ext
        index     = 0
        ls        = set(os.listdir(dir))
        while candidate in ls:
            candidate = "{}_{}{}".format(fname,index,ext)
            index    += 1
        
        return os.path.join(dir,candidate)
        # End checkFile


    # This is a bit overkill. It will search for multiple extensions but we really
    # only need one in this script
    def findFiles(self,inputDir,extens):
        found = {x: [] for x in extens} # lists of found files

        # Walk the tree
        for dirpath, dirnames, files in os.walk(inputDir):
            # Loop through the file names for the current step
            for name in files:
                # Split the name by '.' & get the last element
                ext = name.lower().rsplit('.', 1)[-1]
         
                # Save the full name if ext matches
                if ext in extens:
                    found[ext].append(os.path.join(dirpath, name))

        return found
        # End findFiles



    def convertFile(self,inputFile,outputFile):
        hootCmd = ['hoot','convert','--error',
                '-D','convert.ops=hoot::TranslationOp',
                '-D','translation.script=/var/lib/hootenanny/translations/GBDX.js']

        hootCmd.append(inputFile)
        hootCmd.append(outputFile)

        hootOut = ''

        # Debug
        # print('Hoot: %s') % (' '.join(hootCmd))

        # Very basic error checking
        try:
            hootOut = subprocess.check_output(hootCmd)

        # Question: If one file fails, should we about the lot or just keep going?
        except subprocess.CalledProcessError, e:
            return e.output

        # with "--error" the Hoot command shoud not have any output. If it does then
        # it is an error.
        if len(hootOut) > 0:
            return hootOut
        # End convertFile


    def processGeoJsonFiles(self,inputDir,outputDir):
        print('\nThe geojson Files')

        fList = self.findFiles(inputDir,['geojson'])
        for inputFile in fList['geojson']:
            print('Full Path: %s') % (inputFile)
            
            fName = os.path.basename(inputFile)
            print('File Name: %s') % (fName)
            
            outputFile = self.checkFile(os.path.join(outputDir,fName))
            print('Output Name: %s') % (outputFile)

            returnText = self.convertFile(inputFile,outputFile)
            if returnText is not None:
                self.status = 'failed'
                self.reason = returnText
                print('ReturnText: %s') % (returnText)
            print('')
        # End processGeoJsonFiles


    def processJsonFiles(self,inputDir,outputDir):
        print('\nThe json Files')
        tmpInName = self.checkFile(os.path.join(inputDir,'hootIn%s.geojson' % os.getpid()))
        tmpOutName = self.checkFile(os.path.join(outputDir,'hootOut%s.geojson' % os.getpid()))
        print('Tmp In Path: %s') % (tmpInName)
        print('Tmp Out Path: %s') % (tmpOutName)

        fList = self.findFiles(inputDir,['json'])
        for inputFile in fList['json']:
            print('Full Path: %s') % (inputFile)

            # Make sure that we don't have anything from a previous run
            if os.path.isfile(tmpInName):
                os.remove(tmpInName)

            if os.path.isfile(tmpOutName):
                os.remove(tmpOutName)

            # Now rename the input to the tmp Input geojson name
            os.rename(inputFile,tmpInName)

            # Process the file and output it to the tmp Output geojson name
            returnText = self.convertFile(tmpInName,tmpOutName)
            if returnText is not None:
                self.status = 'failed'
                self.reason = returnText
                print('ReturnText: %s') % (returnText)
            
            # Make sure that we got some output before trying to do anything with it.
            if os.path.isfile(tmpOutName):
                fName = os.path.basename(inputFile)
                print('File Name: %s') % (fName)
                
                outputFile = self.checkFile(os.path.join(outputDir,fName))
                print('Output Name: %s') % (outputFile)
                os.rename(tmpOutName,outputFile)
        
            # Clean up. Move the input file back to what it was
            os.rename(tmpInName,inputFile)
            print('')        # End processJsonFiles


    def processZipFiles(self,inputDir,outputDir):
        print('The zip Files')
        tmpDirName = self.checkFile(os.path.join(inputDir,'hoot%s' % os.getpid()))
        print('Tmp Dir Path: %s') % (tmpDirName)
        
        fList = self.findFiles(inputDir,['zip'])
        for inputFile in fList['zip']:
            print('Full Path: %s') % (inputFile)

            # Defensive cleaning 
            if os.path.isfile(tmpDirName):
                os.remove(tmpDirName)

            if os.path.isdir(tmpDirName):
                shutil.rmtree(tmpDirName)

            os.makedirs(tmpDirName)

            try:
                zip = zipfile.ZipFile(inputFile)
                zip.extractall(path=tmpDirName)

            except zipfile.BadZipFile, e:
                # Just rm the Dir and move on
                shutil.rmtree(tmpDirName)
                continue

            # Now process the unpacked files
            self.processGeoJsonFiles(tmpDirName,outputDir)
            self.processJsonFiles(tmpDirName,outputDir)

            # Now get recursive :-)  We may have zip files of zip files
            # This is very defensive
            self.processZipFiles(tmpDirName,outputDir)

            print('')
            #shutil.rmtree(tmpDirName)
        # End processZipFiles


    def invoke(self):
        inputDir = self.get_input_data_port('geojson')
        outputDir = self.get_output_data_port('data_out')

        if not os.path.exists(outputDir):
            os.makedirs(outputDir)

        # Now go looking for files to convert
        self.processGeoJsonFiles(inputDir,outputDir)
        self.processJsonFiles(inputDir,outputDir)
        self.processZipFiles(inputDir,outputDir)


            # fName = os.path.basename(inputFile)
            # print('File Name: %s') % (fName)
            
            # outputFile = self.checkFile(os.path.join(outputDir,fName))
            # print('Output Name: %s') % (outputFile)

            # returnText = self.convertFile(inputFile,outputFile)
            # if returnText is not None:
            #     self.status = 'failed'
            #     self.reason = returnText
            #     print('ReturnText: %s') % (returnText)
                       

            # # Skip anything that isn't something we can work with.
            # if not (file.endswith('.geojson') or file.endswith('.json')):
            #     continue

            # # Hootenanny uses .geojson, not .json
            # if file.endswith('.json'):
            #     tFile = file.replace('.json','.geojson')
            #     os.rename(os.path.join(inputDir,file),os.path.join(inputDir,tFile))
            # else:
            #     tFile = file

            # inputFile = os.path.join(inputDir,tFile)
            # outputFile = os.path.join(outputDir,tFile)


            # # Rename the output back to .json if that is what we got as an input
            # # Also rename the input back as well
            # if file != tFile:
            #     newOutputFile = outputFile.replace('.geojson','.json')
            #     os.rename(outputFile,newOutputFile)
            #     os.rename(os.path.join(inputDir,tFile),os.path.join(inputDir,file))

            # # with "--error" the Hoot command shoud not have any output. If it does then
            # # it is an error.
            # if len(hootOut) > 0:
            #     self.status = 'failed'
            #     self.reason = hootOut


if __name__ == "__main__":
    with HootGbdxTask() as task:
        task.invoke()
