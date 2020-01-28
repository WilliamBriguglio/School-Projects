'''
William Briguglio
The following code is completely my own.
The following code was used by me to implement feature selection for the first 4 classification schemes, as well as searching for the best classification results 
using SVM with linear kernel for all 4 schemes
'''
#_________________________IMPORTS______________________________________________________
import numpy as np;
import pandas as pd;
import csv;
import sklearn.preprocessing as preproc
from sklearn.feature_selection import SelectKBest, chi2, RFE, VarianceThreshold, SelectFromModel, VarianceThreshold;
from sklearn.ensemble import RandomForestClassifier;
from sklearn.svm import SVC;
from sklearn.neighbors import KNeighborsClassifier;
from sklearn.model_selection import cross_val_predict;
from sklearn.naive_bayes import GaussianNB
from sklearn.metrics import confusion_matrix, precision_score, recall_score;
import matplotlib.pyplot as plt;
from sklearn.model_selection import GridSearchCV;

import winsound;

#_________________________FUNCTIONS____________________________________________________
def loadData():
	clin = 'Prostate_Cancer_Data/prad_tcga_clinical_data.csv';
	gene = 'Prostate_Cancer_Data/prad_tcga_genes.csv';	
	validClass = ['T2a','T2b','T2c','T3a','T3b','T4' ];
	
	print("Loading data...")	
	clin = (np.array(pd.read_csv(clin, delimiter = ','))); 
	Y = clin[:,60]; #get labels(PATG_T_STAGE)
	Yid = clin[:,1]; #get patient ID's from clinical data
	print("\t-Labels loaded.");

	X = pd.read_csv(gene, delimiter = ',')
	Xid = X.columns.values; #get patient ID's from gene expression data
	Xid = np.delete(Xid, (0)); #remove row label "ID"
	X = np.array(X); #get features(GENE EXPRESSION)
	XGeneID = X[:,0] #get gene ID's
	X = np.delete(X, (0), axis=1); #remove gene ID
	X = np.transpose(X)	#obtain m x n matrix; m = |samples| & n = |features|
	print("\t-Features loaded.\n");
	
	for i in reversed(range(0, Yid.size)):#remove patients in clin data but not gene data
		if Yid[i] not in Xid:
			Yid = np.delete(Yid, i);
			Y = np.delete(Y, i);
			
	for i in reversed(range(0, Y.size)): #remove samples/labels with invalid class
		if Y[i] not in validClass:
			Y = np.delete(Y, i);
			X = np.delete(X, (i), axis=0);
	
	print("Data ready.");		
	return X, Y, XGeneID;
	
def removeSubClasses(Y):#removes subclasses from 
	for i in range(0, len(Y)):
		if	((Y[i] == 'T2a') | (Y[i] == 'T2b') | (Y[i] == 'T2c')):
			Y[i] = 'T2';
		if	((Y[i] == 'T3a') | (Y[i] == 'T3b')):
			Y[i] = 'T3';
        
#_________________________MAIN_________________________________________________________	


#---------------------------Code to Select Features------------------------------------
#'''
X, Y, GeneIDs = loadData(); #Obtain Labels(Y) and Features(X);
##X containes 487 rows of 60,483 gene expressions
##Y containes 487 labels in order corresponding to samples in X

print(X.shape);


##remove constant features
Sel = VarianceThreshold(threshold=0.0);
X2 = Sel.fit_transform(X);
GeneIDs = GeneIDs[Sel.get_support(indices=True)]

print(X2.shape);
exit();

Y2 = np.zeros(Y.size)
removeSubClasses(Y); #remove subclasses i.e. T2x = T2
for i in range(0,487): #change class labels
	if Y[i] == 'T2':
		Y2[i] = 1;
	if Y[i] == 'T3':
		Y2[i] = 1;
	if Y[i] == 'T4':
		Y2[i] = 2;

#select best features using selectFromModel with RandomForestClassifier
clf = RandomForestClassifier(n_estimators=100, class_weight="balanced");
sfm = SelectFromModel(clf);
sfm.fit(X2, Y2)
X_transform = sfm.transform(X2);
GeneIDs = GeneIDs[sfm.get_support(indices=True)]

np.savetxt('SFMGeneIDs_T4.csv', GeneIDs, delimiter=",", fmt='%s');
np.savetxt('SFMFeats_T4.csv', X_transform, delimiter=",");

#exit();
#'''
#---------------------------Code to Load Selected Features and Labels------------------

GeneIDs = [];
with open("SFMGeneIDs_T4.csv", 'r') as f: #load labels
	dat = csv.reader(f, delimiter=',')
	for row in dat:
		GeneIDs.append(row[0]);
GeneIDs = np.array(GeneIDs);

Y = [];
with open("SelLabels.csv", 'r') as f: #load labels
	dat = csv.reader(f, delimiter=',')
	for row in dat:
		Y.append(row[0]);
Y = np.array(Y);

Y2 = np.zeros(Y.size)
removeSubClasses(Y); #remove subclasses i.e. T2x = T2
for i in range(0,487): #Change class labels
	if Y[i] == 'T2':
		Y2[i] = 1;
	if Y[i] == 'T3':
		Y2[i] = 1;
	if Y[i] == 'T4':
		Y2[i] = 2;
		
X = [];
with open("SFMFeats_T4.csv", 'r') as f: #load features obtained from Select From Model with Random Forest
	dat = csv.reader(f, delimiter=',')
	for row in dat:
		X.append(row);
X = np.array(X);

X = preproc.normalize(X, norm='l2'); #normalize to increase computation speed
X = preproc.scale(X); #scale to increase computation speed and effectiveness of classifier


SVM = SVC(kernel = 'linear' , class_weight='balanced');
parameters = {'C':[1, 10, 100, 1000]} 


f=open("Scores_linear_T4_throw.txt", "w+");

#Ns = [10,20,30,40,50,60,70,80,90,100];
#Ns = [10,15,20,25,30];
#Ns = [11,12,13,14,15,16,17,18,19];
Ns = [14];

for n in Ns: 
	
	print("starting rfe(",n,")")
	rfe = RFE(estimator=SVM, n_features_to_select=n, step=1)
	print("...")
	rfe.fit(X, Y2)
	print("Done!")
	ranking = rfe.ranking_

	#obtain most important features according to RFE ranking
	rmv = [];
	for i in range(0, ranking.size):
		if ranking[i] != 1:
			rmv.append(i);
	XRFE = np.delete(X, rmv, axis=-1);
	GeneIDsRFE = np.delete(GeneIDs, rmv, axis=0);
	np.savetxt('Linear_T4.csv', GeneIDsRFE, delimiter=",", fmt='%s');
	
	clf = GridSearchCV(SVM, parameters, cv=3, scoring='f1_weighted', n_jobs=-1); #grid search to find best params according to scoring function
	clf.fit(XRFE, Y)
	SVM.C = clf.cv_results_['params'][clf.best_index_]['C'];
	
	Y_pred = cross_val_predict(SVM, XRFE, Y2, cv = 3)	
	
	c = confusion_matrix(Y2, Y_pred);
	f.write("n: "+str(n)+"\tC:"+str(SVM.C)+"\n");
	f.write("w:"+str(precision_score(Y2, Y_pred, average='weighted'))+"\n");
	print(str(precision_score(Y2, Y_pred, average='weighted')))
	f.write("m:"+str(precision_score(Y2, Y_pred, average='micro'))+"\n");
	f.write(str(c));
	f.write("\n\n");
f.close();

winsound.Beep(2500, 1000);









