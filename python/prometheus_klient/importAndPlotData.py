import os, re
from utils import gridParameters, optimalRoundForArange
from matplotlib import pyplot as plt
import paramiko 

outputDir = "examplePath"
mifFileName = "examplePath"

#symulation parameters
#format: [start_value, stop, step] or [single_value]

paramsProperties = []
paramsProperties.append(("freeAniz", [20e3], 0))
paramsProperties.append(("Voltage", [0.021, 0.04, 0.002], 4))

remoteOutputDir = "examplePath/{}".format(outputDir)

def getColumnWithHeader(odtFileName, header):
	with open(odtFileName) as odtFile:
		headersLine = None
		unitsLine = None
		for line in odtFile:
			splitedLine = line.split()
			if splitedLine[0] == '#' and splitedLine[1] == 'Columns:':
				headersLine = re.sub("# Columns: ", "", line)
				break
				
		for line in odtFile:
			splitedLine = line.split()
			if splitedLine[0] == '#' and splitedLine[1] == 'Units:':
				unitsLine = re.sub("# Units: ", "", line)
				break
				
		allHeaders = re.findall("(\{[a-zA-Z0-9\ :-_\/]+\}|[a-zA-Z0-9:-_\/]+)", headersLine)
		allUnits = re.findall("(\{[a-zA-Z0-9\/\ ]*\}|[a-zA-Z0-9\/]+)", unitsLine)
		for i in range(len(allHeaders)):
			allHeaders[i] = re.sub("\{|\}" ,"", allHeaders[i])
			if len(re.findall(header, allHeaders[i])):
				return i, allHeaders[i], allUnits[i]
						
def drowFunctionOfVariables(odtFileName, variableNameX, variableNameY, outputFile):
	with open(odtFileName) as odtFile:
		x = []
		y = []
		xIndex, xLabel, xUnit = getColumnWithHeader(odtFileName, variableNameX)
		yIndex, yLabel, yUnit = getColumnWithHeader(odtFileName, variableNameY)
		for line in odtFile:
			if not line[0] == '#':
				splitedLine = line.split()
				x.append(splitedLine[xIndex])
				y.append(splitedLine[yIndex])
		
		plt.plot(x, y)
		plt.xlabel("{} [{}]".format(variableNameX,xUnit))
		plt.ylabel("{} [{}]".format(variableNameY,yUnit))
		plt.savefig(outputFile)
		plt.show()
		
		
		
ssh = paramiko.SSHClient()
ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
ssh.connect('prometheus.cyfronet.pl', username="exampleUserName", password="examplePassword")
sftp = ssh.open_sftp()

symulParamGrid = gridParameters(paramsProperties)

#parameterRoundPosition = [optimalRoundForArange(*(i[1])) for i in paramsProperties]

try:
	os.mkdir(outputDir)
except:
	pass

for parameters in symulParamGrid:
	parametersPathElement = ""
	for i in range(len(paramsProperties)):
		#parametersPathElement += "{}_{}_".format(paramsProperties[i][0], format(parameters[i], '.{}f'.format(parameterRoundPosition[i])))
		parametersPathElement += "{}_{}_".format(paramsProperties[i][0], format(parameters[i], '.{}f'.format(paramsProperties[i][2])))
		
	localWorkDir = "{}/{}".format(outputDir, parametersPathElement)
	try:
		os.mkdir(localWorkDir)
	except:
		pass
	remoteWorkDir = "{}/{}".format(remoteOutputDir, parametersPathElement)
	print(localWorkDir)
	print(remoteWorkDir)	
	print()
	
	sftp.get("{}/{}".format(remoteWorkDir, "{}.mif".format(mifFileName)), "{}/{}".format(localWorkDir, "{}.mif".format(mifFileName)))	
	sftp.get("{}/{}".format(remoteWorkDir, "{}.odt".format(mifFileName)), "{}/{}".format(localWorkDir, "{}.odt".format(mifFileName)))
	sftp.get("{}/{}".format(remoteWorkDir, "skrypt.sh"), "{}/{}".format(localWorkDir, "skrypt.sh"))
	sftp.get("{}/{}".format(remoteWorkDir, "skrypt_output.txt"), "{}/{}".format(localWorkDir, "skrypt_output.txt"))
	sftp.get("{}/{}".format(remoteWorkDir, "skrypt_error.txt"), "{}/{}".format(localWorkDir, "skrypt_error.txt"))
	sftp.get("{}/{}".format(remoteWorkDir, "symulation_parameters.txt"), "{}/{}".format(localWorkDir, "symulation_parameters.txt"))
			
	drowFunctionOfVariables("{}/{}".format(localWorkDir, "{}.odt".format(mifFileName)), "Simulation time", "total resistance" , "{}/opor_{}.png".format(outputDir, parametersPathElement))
	#drowFunctionOfVariables("{}/{}".format(localWorkDir, "{}.odt".format(mifFileName)), "Simulation time", "mx" , "{}/magnetyzacja_mx_{}.png".format(outputDir, parametersPathElement))

sftp.close()
ssh.close()
	


	

	