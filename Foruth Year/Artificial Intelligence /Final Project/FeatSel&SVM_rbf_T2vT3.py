'''
William Briguglio
The following code is completely my own, except for the mrmr import statement, and the mrmr feature selection code within the for loop that is labeled as Alexandru's code
The following code was used by me to search for the best classification results using SVM with RBF kernel for classifying T2 vs. T3
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
from skfeature.function.information_theoretical_based.MRMR import mrmr

import winsound;

#_________________________FUNCTIONS____________________________________________________
def removeSubClasses(Y):#removes subclasses from 
	for i in range(0, len(Y)):
		if	((Y[i] == 'T2a') | (Y[i] == 'T2b') | (Y[i] == 'T2c')):
			Y[i] = 'T2';
		if	((Y[i] == 'T3a') | (Y[i] == 'T3b')):
			Y[i] = 'T3';
        
#_________________________MAIN_________________________________________________________	
#---------------------------Code to Load Selected Features and Labels------------------

GeneIDs = [];
with open("SFMGeneIDs_T2vT3.csv", 'r') as f: #load labels
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


SVM = SVC(kernel = 'rbf' , class_weight='balanced');
parameters = {'C':[1, 10, 100, 1000], 'gamma':[0.1,0.01,0.001,0.0001]} 


f=open("Scores_rbf_T2vT3_M.txt", "w+");

Ns = [10,20,30,40,50];
#Ns = [60,70,80,90,100];
#Ns = [110,120,130,140,150,160,170,180,190,200];
#Ns = [250,300,350,400,450,500,550,600,650,700,750,800,850,900,950,1000];

for n in Ns: 
	
	print("starting mrmr(",n,")")
	
	#Alexandru's code-----------------------------------
	F, gar, gar2 = mrmr(X, ZY, n_selected_features=n)
	Xmrmr = X[:,F];#obtain most important features according to MRMR ranking
	#End Alexandru's code-------------------------------
	
	GeneIDsRFE = GeneIDs[F];
	#np.savetxt('rbf_T2vT3.csv', GeneIDsRFE, delimiter=",", fmt='%s');
	print(Xmrmr.shape)

	clf = GridSearchCV(SVM, parameters, cv=3, scoring='f1_weighted', n_jobs=-1); #grid search to find best params according to scoring function
	clf.fit(Xmrmr, ZY);
	SVM.C = clf.cv_results_['params'][clf.best_index_]['C'];
	SVM.gamma = clf.cv_results_['params'][clf.best_index_]['gamma'];
	
	Y_pred = cross_val_predict(SVM, Xmrmr, ZY, cv = 3)	
	
	c = confusion_matrix(ZY, Y_pred);
	f.write("n: "+str(n)+"\tC:"+str(SVM.C)+"\n");
	print(precision_score(ZY, Y_pred, average='weighted'));
	f.write("w:"+str(precision_score(ZY, Y_pred, average='weighted'))+"\n");
	f.write("m:"+str(precision_score(ZY, Y_pred, average='micro'))+"\n");
	f.write(str(c));
	f.write("\n\n");
f.close();

winsound.Beep(2500, 1000);









