#William Briguglio
#Oct. 5th, 2018
#
#_________________________IMPORTS____________________________________________________

import numpy as np;
import time;
from sklearn.naive_bayes import GaussianNB;
from sklearn.model_selection import cross_val_predict
from sklearn.metrics import confusion_matrix
from sklearn.neighbors import KNeighborsClassifier
import numpy as np
import matplotlib.pyplot as plt

#_________________________FUNCTIONS___________________________________________________

def printMetrics(cm, fp):
	TP = cm[0,0]; FN = cm[0,1]; FP=cm[1,0]; TN=cm[1,1]; n = TP+FN+FP+TN; 
	PPV = TP/(TP+FP)
	NPV = TN/(TN+FN)
	Spe = TN/(TN+FP)
	Sen = TP/(TP+FN)
	Acc = (TP+TN)/n
	np.set_printoptions(precision=2)
	fp.write("\tPPV: %.5f  NPV: %.5f  Specificity: %.5f\n\tSensitivity: %.5f  Accuracy: %.5f \n\n" % (PPV, NPV, Spe, Sen, Acc))
	return

def plotData(X, Y, d):
	plt.subplot
	plt.title(d)
	for i in range(0,len(Y)):
		if(Y[i] == 1):
			plt.plot(X[i,0],X[i,1], c='r', marker='+')#positive class
		else:
			plt.plot(X[i,0],X[i,1], c='b', marker='.')#negative class
	plt.show()
	return
	
#_________________________MAIN___________________________________________________	
	
Data = ['twospirals.dat',
		'twogaussians.dat',
		'clusterincluster.dat',
		'halfkernel.dat'];

print("\n\n\n\tResults will be outputted to Results.txt...")
fp=open("Results.txt", "w")

		
gNB = GaussianNB();
kNN = KNeighborsClassifier(n_neighbors=1, p=1)#initialize kNN with minkowski distance and p=2 which is equivalent to the Euclidean distance
for d in Data:
	data = np.loadtxt("data/"+d);#Importing dataset
	X = data[:,1:3];#features
	Y = data[:,0];#labels
	
	#plotData(X, Y, d)
	
	fp.write("Data Set: "+d+"\n")
	fp.write("\tNaive Bayes Scores:\n")
	Y_pred = cross_val_predict(gNB, X, Y, cv=10); #obtain prediction from 10-fold cros val
	cm = confusion_matrix(Y, Y_pred); #obtain confusion_matrix
	printMetrics(cm, fp)
	
	#plotData(X, Y_pred, "NB Predicted:"+d)
	
	for i in range(1,100):
		kNN.n_neighbors=i
		fp.write("\tk-NearestNeighbours(k ="+str(i)+"):\n")
		Y_pred = cross_val_predict(kNN, X, Y, cv=10); #obtain prediction from 10-fold cros val
		cm = confusion_matrix(Y, Y_pred); #obtain confusion_matrix
		printMetrics(cm, fp)
		#plotData(X, Y_pred, "k-NN Predicted:"+d)
	

fp.close()