#!/usr/bin/env python3
import os, sys, shutil
from utils import gridParameters, optimalRoundForArange

outputDir = "./examplePath"
mifFileName = "examplePath"

#symulation parameters
#format: [start_value, stop, step] or [single_value]

paramsProperties = []
paramsProperties.append(("freeAniz", [20e3], 0))
paramsProperties.append(("Voltage", [-0.1, 0.2, 0.2], 4))
paramsProperties.append(("mtop", [-1,3,2], 0))
paramsProperties.append(("mfree", [-1,3,2], 0))
paramsProperties.append(("mbottom", [-1,3,2], 0))

mifFilePath = "./{}.mif".format(mifFileName)
scriptFilePath = "./skrypt.txt"

#read template shell script file
scriptStr = ""
with open(scriptFilePath, 'r') as f:
	for line in f:
		scriptStr += line
	f.close()

#create parameters grid from parameters range
symulParamGrid = gridParameters(paramsProperties)

#cheack best rounding position for parameters - could sort dirs by name
#parameterRoundPosition = [optimalRoundForArange(*(i[1])) for i in paramsProperties]

#create main output dir
try:
	os.mkdir(outputDir)
except:
	pass
	
#process simulations with parameters grid nodes
for parameters in symulParamGrid:
	parametersPathElement = ""
	mifParametersStr = ""
	for i in range(len(paramsProperties)):
		#parametersPathElement += "{}_{}_".format(paramsProperties[i][0], format(parameters[i], '.{}f'.format(parameterRoundPosition[i])))
		parametersPathElement += "{}_{}_".format(paramsProperties[i][0], format(parameters[i], '.{}f'.format(paramsProperties[i][2])))
		mifParametersStr += "{} {} ".format(paramsProperties[i][0],parameters[i])
	currentWorkDir = "{}/{}".format(outputDir, parametersPathElement)
	outputOdtFilePath = "{}/{}".format(currentWorkDir, mifFileName)
	mifParametersStr += "{} {} ".format("output_file", outputOdtFilePath)

	try:
		os.mkdir(currentWorkDir)
	except:
		pass

	scriptPath = '{}/skrypt.sh'.format(currentWorkDir, mifFilePath, outputOdtFilePath, mifParametersStr)
	shutil.copy(mifFilePath, "{}/{}".format(currentWorkDir, mifFilePath))
	
	with open(scriptPath,'w') as f:
		f.write(scriptStr.format(currentWorkDir, currentWorkDir, currentWorkDir, mifFilePath, outputOdtFilePath, mifParametersStr))
		f.close()
		
	with open("{}/symulation_parameters.txt".format(currentWorkDir),'w') as f:
		f.write(mifParametersStr)
		f.close()
			
		command = "sbatch {} ".format(scriptPath)
	print "\nTerminal command:\n", command , '\n'

	os.system(command)
	
	
	
	
	
	
	
	