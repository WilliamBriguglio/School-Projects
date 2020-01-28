#William Briguglio
#Oct. 5th, 2018
#_________________________IMPORTS______________________________________________________

import numpy as np;
from sklearn.cluster import KMeans
from sklearn.mixture import GaussianMixture
from sklearn.metrics import davies_bouldin_score as dbs
import matplotlib.pyplot as plt

#_________________________FUNCTIONS____________________________________________________

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
	
def plotData2(X, Y, Y2, d):
	plt.subplot
	plt.title(d)
	for i in range(0,len(Y)):
		if(Y[i] == 1):
			if(Y2[i] == 1):
				plt.plot(X[i,0],X[i,1], c='r', marker='^', ms=5, mfc='w')#pos+ class, pos+ cluster
			else:
				plt.plot(X[i,0],X[i,1], c='r', marker='.', mfc='w')#pos+ class, neg- cluster
		else:
			if(Y2[i] == 1):
				plt.plot(X[i,0],X[i,1], c='b', marker='^', ms=5, mfc='w')#neg- class, pos+ cluster
			else:
				plt.plot(X[i,0],X[i,1], c='b', marker='.', mfc='w')#neg- class, neg- cluster
	plt.show()
	return
	
#_________________________MAIN_________________________________________________________	
	
Data = ['halfkernel.dat',
		'twospirals.dat',
		'clusterincluster.dat',
		'twogaussians.dat'];

print("\n\n\n\tResults will be outputted to Results.txt...")
fp=open("Results.txt", "w")

KM = KMeans(n_clusters=2); #initialize k-Means object with k = 1
EM = GaussianMixture(n_components=2); #initialize Expectation maximization object with k = 1
k = 2; 

for d in Data:
	fp.write("\nData Set:"+d+"\n");	
	data = np.loadtxt("data/"+d);#Importing dataset
	X = data[:,1:3];#features
	Y = data[:,0];#labels
	
	for k in range(2,21):
		KM.n_clusters = k;
		EM.n_components = k;
	
		#plotData(X, Y, d);
		kmeans = KM.fit(X);#calcuate clusters
		KM_Y = kmeans.labels_;
		EMclusters = EM.fit(X);#get sample's clusters membership
		EM_Y = EM.fit_predict(X);#get sample's clusters membership
		
		fp.write("\tDavies-Bouldin Index(k = "+str(k)+"):\n\t\tk-Means:"+str(dbs(X, KM_Y))+"\tEM: "+str(dbs(X, EM_Y))+"\n");
	
		#plotData2(X, Y, KM_Y, d);
		#plotData2(X, Y, EM_Y, d);

fp.close()