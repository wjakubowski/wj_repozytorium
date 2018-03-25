#!/usr/bin/python
import os, math
from copy import copy

def optimalRound(number):
	if number >= 1 or (number- int(number)) == 0.0:
		return 0
	rest = number - int(number)
	return abs(int(math.log(abs(rest-int(rest)),10))-1)	

def optimalRoundForArange(*numbers):
	positions = [optimalRound(i) for i in numbers]
	return max(positions)

def arange(start, stop , step):
	iter = start
	if start <= stop and step > 0:
		while iter < stop:
			yield iter
			iter += step
	elif step < 0:
		while iter >= stop:
			yield iter
			iter += step
	else:
		raise Exception("Bad parameters start = {}, stop = {}, step = {}, infinit range in arange function.".format (start, stop , step))
			
def xrange(start, stop = None, step = None):
	if stop == None or abs(stop - start) < step:
		return [start]
	else:
		return arange(start, stop , step)

def map_fun(param, list_):
	tmp = []
	for i in list_:
		for l in xrange(*param):
			cop = copy(i)
			cop.append(l)
			tmp.append(cop)
	return tmp

def gridParameters(params):
	parameters = [[]]
	for i in range(len(params)):
		parameters = map_fun(params[i][1], parameters)
	return parameters

"""
params = []
params.append(("X",[3, 3, 1]))
params.append(("Y",[-100]))
params.append(("Z",[0, 4, 1]))

os.system("cls")
print(gridParameters(params))"""



	
	
	
	
	
	
	
	