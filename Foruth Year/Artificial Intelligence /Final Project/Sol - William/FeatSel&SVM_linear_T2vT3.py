'''
William Briguglio
The following code is completely my own.
The following code was used by me to implement feature selection for the 5th classification scheme, T2 vs T3, as well as searching for the best classification results 
using SVM with linear kernel for this scheme
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
'''
X, Y, GeneIDs = loadData(); #Obtain Labels(Y) and Features(X);
##X containes 487 rows of 60,483 gene expressions
##Y containes 487 labels in order corresponding to samples in X

##remove constant features
Sel = VarianceThreshold(threshold=0.0);
X2 = Sel.fit_transform(X);
GeneIDs = GeneIDs[Sel.get_support(indices=True)]

rmv = [];
ZY = np.array([])
removeSubClasses(Y) #remove subclasses i.e. T2x = T2
for i in range(0,487): #change class labels, different labels correspond to different classification schemes
	if Y[i] == 'T2':
		ZY = np.append(ZY,[1]);
	if Y[i] == 'T3':
		ZY = np.append(ZY,[2]);
	if Y[i] == 'T4':
		rmv.append(i);
ZX = np.delete(X2, rmv,  axis=0);

#select best features using selectFromModel with RandomForestClassifier
clf = RandomForestClassifier(n_estimators=100, class_weight="balanced");
sfm = SelectFromModel(clf);
sfm.fit(ZX, ZY)
X_transform = sfm.transform(ZX);
GeneIDs = GeneIDs[sfm.get_support(indices=True)]

print(X_transform.shape)

np.savetxt('SFMGeneIDs_T2vT3_3.csv', GeneIDs, delimiter=",", fmt='%s'); #save gene ID's
np.savetxt('SFMFeats_T2vT3_3.csv', X_transform, delimiter=","); #save gene ID's
#exit();
#'''
#---------------------------Code to Load Selected Features and Labels------------------

GeneIDs = [];
with open("SFMGeneIDs_T2vT3_3.csv", 'r') as f: #load labels
	dat = csv.reader(f, delimiter=',')
	for row in dat:
		GeneIDs.append(row[0]);
GeneIDs = np.array(GeneIDs);

X = [];
with open("SFMFeats_T2vT3_3.csv", 'r') as f: #load features obtained from Select From Model with Random Forest
	dat = csv.reader(f, delimiter=',')
	for row in dat:
		X.append(row);
X = np.array(X);
print(X.shape)

X = preproc.normalize(X, norm='l2'); #normalize to increase computation speed
X = preproc.scale(X); #scale to increase computation speed and effectiveness of classifier


Y = [];
with open("SelLabels.csv", 'r') as f: #load labels
	dat = csv.reader(f, delimiter=',')
	for row in dat:
		Y.append(row[0]);
Y = np.array(Y);

ZY = np.array([])
removeSubClasses(Y); #remove subclasses i.e. T2x = T2
for i in range(0,487): #Change class labels
	if Y[i] == 'T2':
		ZY = np.append(ZY,[1]);
	if Y[i] == 'T3':
		ZY = np.append(ZY,[2]);



SVM = SVC(kernel = 'linear' , class_weight='balanced');
parameters = {'C':[0.1, 1, 10, 100]} 


f=open("Scores_linear_T2vT3_gar.txt", "w+"); #open file for saving scores and confusion matrices

#Ns =[40];
Ns = [10,20,30,40,50,60,70,80,90,100]#,
#Ns = [150,160,170,180,190,200];
#Ns = [250,300,350,400,450,500,550,600,650,700,750,800,850,900,950,1000];

for n in Ns: 
	
	print("starting rfe(",n,")")
	rfe = RFE(estimator=SVM, n_features_to_select=n, step=1)#try 10, 15, 20, 30, 40, 50, 60, 70 features, etc.
	print("...")
	rfe.fit(X, ZY)
	print("Done!")
	ranking = rfe.ranking_

	#obtain most important features according to RFE ranking
	rmv = [];
	for i in range(0, ranking.size):
		if ranking[i] != 1:
			rmv.append(i);
	XRFE = np.delete(X, rmv, axis=-1);
	GeneIDsRFE = np.delete(GeneIDs, rmv, axis=0);
	#np.savetxt('Linear_T2vT3.csv', GeneIDsRFE, delimiter=",", fmt='%s'); #save feature ID's

	clf = GridSearchCV(SVM, parameters, cv=3, scoring='f1_weighted', n_jobs=-1); #grid search to find best params according to scoring function
	clf.fit(XRFE, ZY);
	SVM.C = clf.cv_results_['params'][clf.best_index_]['C'];
	
	Y_pred = cross_val_predict(SVM, XRFE, ZY, cv = 3)#obtain prediction results with best params	
	
	#save stats to file
	c = confusion_matrix(ZY, Y_pred);
	f.write("n: "+str(n)+"\tC:"+str(SVM.C)+"\n");
	print(precision_score(ZY, Y_pred, average='weighted'));
	f.write("w:"+str(precision_score(ZY, Y_pred, average='weighted'))+"\n");
	f.write("m:"+str(precision_score(ZY, Y_pred, average='micro'))+"\n");
	f.write(str(c));
	f.write("\n\n");
f.close();

winsound.Beep(2500, 1000);









