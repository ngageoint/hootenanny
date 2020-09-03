# Run a Hootenanny translation using the GbdxTaskInterface

# NOTE: There have been many changes to the file output type, nameing etc There is commented
# out code that should get removed once things are stable.
# -- Mattj

import os
import csv
import json
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

    # Another handy function from StackOverflow
    def zipdir(self,path,zipf):
        # Iterate all the directories and files
        for root, dirs, files in os.walk(path):
            # Create a prefix variable with the folder structure inside the path folder. 
            # So if a file is at the path directory will be at the root directory of the zip file
            # so the prefix will be empty. If the file belongs to a containing folder of path folder 
            # then the prefix will be that folder.
            if root.replace(path,'') == '':
                    prefix = ''
            else:
                    # Keep the folder structure after the path folder, append a '/' at the end 
                    # and remove the first character, if it is a '/' in order to have a path like 
                    # folder1/folder2/file.txt
                    prefix = root.replace(path, '') + '/'
                    if (prefix[0] == '/'):
                            prefix = prefix[1:]
            for filename in files:
                    actual_file_path = root + '/' + filename
                    zipped_file_path = prefix + filename
                    zipf.write( actual_file_path, zipped_file_path)


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



    def convertFileJson(self,envVars,inputFile,outputFile):
        override = 'schema.translation.override=' + json.dumps(envVars)
        hootCmd = ['hoot','convert','--error',
                '-D','convert.ops=hoot::AddBboxVisitor;hoot::TranslationOp',
                '-D','map.writer.skip.empty=true',
                '-D',override,
                # '-D','ogr.debug.dumptags=true',
                '-D','schema.translation.script=/var/lib/hootenanny/translations/GBDX.js']

        hootCmd.append(inputFile)
        hootCmd.append(outputFile)

        hootOut = ''

        # Debug
        # print('Hoot Command: %s') % (' '.join(hootCmd))

        # Very basic error checking
        try:
            hootOut = subprocess.check_output(hootCmd)

        # Question: If one file fails, should we abort the lot or just keep going?
        except subprocess.CalledProcessError, e:
            return e.output

        # with "--error" the Hoot command shoud not have any output. If it does then
        # it is an error or we turned on debugging.
        if len(hootOut) > 0:
            # print('Hoot Command: %s') % (' '.join(hootCmd))
            # print('\nHootOut: {}'.format(hootOut))
            return hootOut
        # End convertFileJson


    def convertFileXml(self,envVars,inputFile,outputFile):
        override = 'schema.translation.override=' + json.dumps(envVars)

        hootCmd = ['hoot','convert','--error',
                '-D','convert.ops=hoot::AddBboxVisitor;hoot::TranslationOp',
                '-D','osm.add.bbox.tag=true',
                '-D','map.writer.skip.empty=true',
                '-D',override,
                '-D','schema.translation.script=/var/lib/hootenanny/translations/GBDX_XML_Shape.js']
                # '-D','ogr.debug.dumptags=true',

        hootCmd.append(inputFile)
        hootCmd.append(outputFile)

        hootOut = ''

        # Debug
        # print('Hoot Command: %s') % (' '.join(hootCmd))

        # Very basic error checking
        try:
            hootOut = subprocess.check_output(hootCmd)

        # Question: If one file fails, should we about the lot or just keep going?
        except subprocess.CalledProcessError, e:
            return e.output

        # with "--error" the Hoot command shoud not have any output. If it does then
        # it is an error or we turned on debugging.
        if len(hootOut) > 0:
            # print('Hoot Command: %s') % (' '.join(hootCmd))
            # print('\nHootOut: {}'.format(hootOut))
            return hootOut
        # End convertFileXml


    def processGeoJsonFiles(self,inputDir,fileNumber,fList,envVars,outputDirJson,outputDirXml):
        # print 'Processing GeoJSON: Start'
        # print 'GeoJSON List: {}\n'.format(fList)
        # print 'GeoJSON Output: {}  {}'.format(outputDirJson, outputDirXml)

        # jsonIndexFile = open(os.path.join(outputDirJson,'json_index.csv'),'a')
        # xmlIndexFile = open(os.path.join(outputDirXml,'xml_index.csv'),'a')

        for inputFile in fList:
            fileNumber += 1

            # print 'Processing: {} as file{}'.format(inputFile,fileNumber)
            # jsonIndexFile.write('jsonFile{},{}\n'.format(fileNumber,inputFile))
            # xmlIndexFile.write('xmlFile{},{}\n'.format(fileNumber,inputFile))

            fName = os.path.splitext(os.path.basename(inputFile))[0]

            # tOutputDirJson = os.path.join(outputDirJson,'jsonFile%s.gbdx' % fileNumber)
            # tOutputDirJson = os.path.join(outputDirJson,os.path.basename(inputFile).replace('.geojson','.gbdx'))
            tOutputDirJson = os.path.join(outputDirJson,fName + '.gbdx')

            # outputFile = self.checkFile(os.path.join(outputDir,fName))
            returnTextJson = self.convertFileJson(envVars,inputFile,tOutputDirJson)
            if returnTextJson is not None:
                self.status = 'failed'
                self.reason = returnTextJson

            # tOutputDirXml = os.path.join(outputDirXml,'xmlFile%s.gxml' % fileNumber)
            # tOutputDirXml = os.path.join(outputDirXml,os.path.basename(inputFile).replace('.geojson','.gxml'))
            tOutputDirXml = os.path.join(outputDirXml,fName + '.gxml')
            returnTextXml = self.convertFileXml(envVars,inputFile,tOutputDirXml)
            if returnTextXml is not None:
                self.status = 'failed'
                self.reason = str(returnTextJson) + ';' + returnTextXml

        # jsonIndexFile.close()
        # xmlIndexFile.close()
        #print 'Processing GeoJSON: End'
        return fileNumber
        # End processGeoJsonFiles


    def processJsonFiles(self,inputDir,fileNumber,fList,envVars,outputDirJson,outputDirXml):
        # print 'Processing JSON: Start'
        # print 'JSON List: {}\n'.format(fList)
        tmpInName = self.checkFile(os.path.join(inputDir,'hootIn%s.geojson' % os.getpid()))

        # jsonIndexFile = open(os.path.join(outputDirJson,'json_index.csv'),'a')
        # xmlIndexFile = open(os.path.join(outputDirXml,'xml_index.csv'),'a')

        for inputFile in fList:
            fileNumber += 1

            # print 'Processing: {} as file{}'.format(inputFile,fileNumber)

            # jsonIndexFile.write('jsonFile{},{}\n'.format(fileNumber,inputFile))
            # xmlIndexFile.write('xmlFile{},{}\n'.format(fileNumber,inputFile))

            # Make sure that we don't have anything from a previous run
            if os.path.isfile(tmpInName):
                os.remove(tmpInName)

            # Now rename the input to the tmp Input geojson name
            os.rename(inputFile,tmpInName)

            fName = os.path.splitext(os.path.basename(inputFile))[0]

            # Process the file and output it to the tmp Output geojson name
            # tOutputDirJson = os.path.join(outputDirJson,'jsonFile%s.gbdx' % fileNumber)
            tOutputDirJson = os.path.join(outputDirJson,fName + '.gbdx')

            returnTextJson = self.convertFileJson(envVars,tmpInName,tOutputDirJson)
            if returnTextJson is not None:
                self.status = 'failed'
                self.reason = returnTextJson

            # tOutputDirXml = os.path.join(outputDirXml,'xmlFile%s.gxml' % fileNumber)
            tOutputDirXml = os.path.join(outputDirXml,fName + '.gxml')
            returnTextXml = self.convertFileXml(envVars,tmpInName,tOutputDirXml)
            if returnTextXml is not None:
                self.status = 'failed'
                self.reason = str(returnTextJson) + ';' + returnTextXml

            # Clean up. Move the input file back to what it was
            os.rename(tmpInName,inputFile)

        # jsonIndexFile.close()
        # xmlIndexFile.close()
        # print 'Processing JSON: End'
        return fileNumber
        # End processJsonFiles


    def processZipFiles(self,inputDir,fileNumber,fList,envVars,outputDirJson,outputDirXml):
        # print '\nProcessing Zip: Start'
        tmpDirName = self.checkFile(os.path.join(inputDir,'hoot%s' % os.getpid()))
        # print '  Zip inputDir: {}'.format(inputDir)
        # print '  Zip TmpDir: {}'.format(tmpDirName)
        # print '  Zip List: {}\n'.format(fList)

        for inputFile in fList:
            # print 'Processing Zip File: {}'.format(inputFile)
    
            # Defensive cleaning 
            if os.path.isfile(tmpDirName):
                os.remove(tmpDirName)

            if os.path.isdir(tmpDirName):
                shutil.rmtree(tmpDirName)

            # print '    About to Create: {}'.format(tmpDirName)
            os.makedirs(tmpDirName)

            try:
                zip = zipfile.ZipFile(inputFile)
                zip.extractall(path=tmpDirName)

            except zipfile.BadZipFile, e:
                # Just rm the Dir and move on
                shutil.rmtree(tmpDirName)
                continue

            # Make sure that we have files to work with
            tList = self.findFiles(tmpDirName,['geojson','json','zip','shp','properties'])

            #print 'Unpacked Zip: {}  Files: {}\n'.format(inputFile,tList)

            # Check if the zip file has anything we can process
            if tList['geojson'] == [] and tList['json'] == [] and tList['zip'] == [] and tList['shp'] == []:
                # print 'Empty Zip Directory. About to Remove: {}\n'.format(tmpDirName)
                shutil.rmtree(tmpDirName)
                continue
    
            envVars = {}
            if (tList['properties']):
                # print fileList['properties']
                envVars = self.processProperties(tList['properties'][0])

            # Now process the unpacked files
            if (tList['geojson']):
                # print 'Zip Processing: {}  Filenumber: {}'.format(tList['geojson'], fileNumber)
                fileNumber = self.processGeoJsonFiles(tmpDirName,fileNumber,tList['geojson'],envVars,outputDirJson,outputDirXml)

            if (tList['shp']):
                # print 'Zip Processing: {}  Filenumber: {}'.format(tList['shp'], fileNumber)
                fileNumber = self.processGeoJsonFiles(tmpDirName,fileNumber,tList['shp'],envVars,outputDirJson,outputDirXml)

            if (tList['json']):
                # print 'Zip Processing: {}  Filenumber: {}'.format(tList['json'], fileNumber)
                fileNumber = self.processJsonFiles(tmpDirName,fileNumber,tList['json'],envVars,outputDirJson,outputDirXml)

            # Now get recursive :-)  We may have zip files of zip files
            # This is very defensive
            # print '## About to Process Zip Files'
            if (tList['zip']):
                # print 'Zip Processing: {}  Filenumber: {}'.format(tList['zip'], fileNumber)
                fileNumber = self.processZipFiles(tmpDirName,fileNumber,tList['zip'],envVars,outputDirJson,outputDirXml)
            # print '## Back from Process Zip Files'
        
            # Cleanup before returning
            # print '    About to Remove: {}\n'.format(tmpDirName)
            shutil.rmtree(tmpDirName)

        # print 'Processing Zip: End'
        return fileNumber
        # End processZipFiles


    def processProperties(self,filename):
        file = open(filename,'rb')
        #print('Mapping Properties: {}'.format(filename))

        eVars = {}
        for rawLine in file:
            # NOTE: Sometimes the values have '=' in them
            line = rawLine.split('=',1)

            if (line[0].find('.') > -1):
                key = line[0].split('.')[1]
            else:
                key = line[0]

            eVars[key] = line[1].strip()

        return eVars
    # End processProperties


    def invoke(self):
        inputDir = self.get_input_data_port('geojson')

        # outputDir = os.path.join(self.get_output_data_port('data_out'),'results')
        outputDir = self.get_output_data_port('data_out')

        if not os.path.exists(outputDir):
            os.makedirs(outputDir)
        else:
            shutil.rmtree(outputDir)
            os.makedirs(outputDir)

        # Add an empty file to the output directory to keep the pipeline happy
        # open(os.path.join(outputDir,'.empty'), 'a').close()

        # Working Directory. We zip this and make the output.
        workDirJson = self.checkFile(os.path.join('/tmp','hootJson%s' % os.getpid()))
        workDirXml = self.checkFile(os.path.join('/tmp','hootXml%s' % os.getpid()))
        
        # print 'workDir: {}\n'.format(workDir)
        if not os.path.exists(workDirJson):
            os.makedirs(workDirJson)
        else:
            shutil.rmtree(workDirJson)
            os.makedirs(workDirJson)

        # print 'workDir: {}\n'.format(workDir)
        if not os.path.exists(workDirXml):
            os.makedirs(workDirXml)
        else:
            shutil.rmtree(WorkDirXml)
            os.makedirs(workDirXml)

        # jsonIndexFile = open(os.path.join(workDirJson,'json_index.csv'),'a')
        # jsonIndexFile.write('output_directory,detection_file\n')
        # jsonIndexFile.close()

        # xmlIndexFile = open(os.path.join(workDirXml,'xml_index.csv'),'a')
        # xmlIndexFile.write('output_directory,detection_file\n')
        # xmlIndexFile.close()

        # Check if we have something to work with
        fileList = self.findFiles(inputDir,['geojson','json','shp','zip','properties'])

        envVars = {}
        if (fileList['properties']):
            # print fileList['properties']
            envVars = self.processProperties(fileList['properties'][0])
         
         # We will increment this each time we write something.
        fileNumber = 0

        # Now go looking for files to convert
        if (fileList['geojson']):
            # print('Processing: {}'.format(fileList['geojson']))
            fileNumber = self.processGeoJsonFiles(inputDir,fileNumber,fileList['geojson'],envVars,workDirJson,workDirXml)
            # print('fileNumber: {}'.format(fileNumber))

        if (fileList['json']):
            # print('Processing: {}'.format(fileList['json']))
            fileNumber = self.processJsonFiles(inputDir,fileNumber,fileList['json'],envVars,workDirJson,workDirXml)
            # print('fileNumber: {}'.format(fileNumber))

        if (fileList['zip']):
            # print('Processing: {}'.format(fileList['zip']))
            fileNumber = self.processZipFiles(inputDir,fileNumber,fileList['zip'],envVars,workDirJson,workDirXml)
            # print('fileNumber: {}'.format(fileNumber))

        if (fileList['shp']):
            # print('Processing: {}'.format(fileList['shp']))
            fileNumber = self.processGeoJsonFiles(inputDir,fileNumber,fileList['shp'],envVars,workDirJson,workDirXml)
            # print('fileNumber: {}'.format(fileNumber))

        # Zip up the outputs.
        zipf = zipfile.ZipFile(os.path.join(outputDir,'results_json.zip'), 'w', zipfile.ZIP_DEFLATED)
        self.zipdir(workDirJson, zipf)
        zipf.close()

        zipf = zipfile.ZipFile(os.path.join(outputDir,'results_xml.zip'), 'w', zipfile.ZIP_DEFLATED)
        self.zipdir(workDirXml, zipf)
        zipf.close()

        # cleanup
        shutil.rmtree(workDirJson)
        shutil.rmtree(workDirXml)


if __name__ == "__main__":
    with HootGbdxTask() as task:
        task.invoke()
