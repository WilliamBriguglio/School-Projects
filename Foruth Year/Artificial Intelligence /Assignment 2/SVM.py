#William Briguglio
#Nov. 1st, 2018
#_________________________IMPORTS_____________________________________________________________________________________________________________

import numpy as np;
from sklearn import svm;
from sklearn.model_selection import cross_val_predict, KFold;
from sklearn.metrics import confusion_matrix, roc_curve, auc;
from scipy.spatial import ConvexHull
import matplotlib.pyplot as plt;

#_________________________FUNCTIONS___________________________________________________________________________________________________________

def printMetrics(cm):
        TP = cm[0,0]; FN = cm[0,1]; FP=cm[1,0]; TN=cm[1,1]; n = TP+FN+FP+TN; 
        PPV = TP/(TP+FP);
        NPV = TN/(TN+FN);
        Spe = TN/(TN+FP);
        Sen = TP/(TP+FN);
        Acc = (TP+TN)/n;
        print("\t\tPPV: %.5f  NPV: %.5f  Specificity: %.5f\n\t\tSensitivity: %.5f  Accuracy: %.5f \n\n" % (PPV, NPV, Spe, Sen, Acc));
        return

def plotData(X, Y, d):
        plt.subplot;
        plt.title(d);
        for i in range(0,len(Y)):
                if(Y[i] == 1):
                        pass
                        plt.plot(X[i,0],X[i,1], c='r', marker='+');#positive class
                else:
                        pass
                        plt.plot(X[i,0],X[i,1], c='b', marker='.');#negative class
        plt.show();
        return
        
#_________________________MAIN________________________________________________________________________________________________________________       
        
Data = ['twospirals.dat',
                'twogaussians.dat',
                'clusterincluster.dat',
                'halfkernel.dat'];


Kernels = ['linear',
                'poly',
                'rbf'];

#initialize SVM with degree 2 for polynomial kernel, ignored by all other kernels               
SVM = svm.SVC(degree=2, gamma=1);

#set to true to do ROC curve analysis for RBF
ROC = False;

for d in Data:
        data = np.loadtxt("data/"+d);#Importing dataset
        X = data[:,1:3];#features
        Y = data[:,0];#labels
        
        #plotData(X, Y, d);
        
        print("Data Set: "+d);
        
        for i in Kernels:
                SVM.kernel=i;
                if ROC == True:
                        #-------------------------------------ROC CURVE ANALYSIS FOR RBF------------------------------------------------------
                        k = 0; #tracks iterations
                        tpr = []
                        fpr = []
                        minf = 1.0;     #tracks mminimum false positive rate
                        maxt = 0.0;     #tarcks maximum true positive rate
                        Cmax = 0;       #saves C value for max TRP
                        Gmax = 0;       #saves gamma for max TRP
                        Cmax2 = 0;      #saves C value for min FRP
                        Gmax2 = 0;      #saves gamma value for min FRP
                        plt.figure();
                        plt.xlim([-0.025,1.025]);
                        plt.ylim([-0.025,1.025]);
                        for i in range(-21,21):
                                C = 2**i;
                                SVM.C=C;
                                for j in range(-9,9):
                                        G = 2**j;
                                        SVM.gamma=G;
                                        Y_pred = cross_val_predict(SVM, X, Y, cv=10); #obtain prediction from 10-fold cros val
                                        cm = confusion_matrix(Y, Y_pred); #obtain confusion_matrix
                                        if cm[0,0]/(cm[0,0]+cm[1,0]) > maxt: #new max TPR found
                                                maxt = cm[0,0]/(cm[0,0]+cm[1,0]);
                                                Cmax = C;
                                                Gmax = G;
                                        if cm[1,0]/(cm[0,0]+cm[1,0]) < minf: #new min FPR found
                                                minf = cm[1,0]/(cm[0,0]+cm[1,0]);
                                                Cmax2 = C;
                                                Gmax2 = G;
                                        tpr = np.append(tpr,[cm[0,0]/(cm[0,0]+cm[1,0])] ); fpr = np.append(fpr, [cm[1,0]/(cm[0,0]+cm[1,0])] );#save tpr and fpr
                                        k=k+1;
                                        print("\nIteration: ",k);
                        print("\n\t| max tpr:",maxt,"\t| min fpr:",minf,"\t| gamma:",Gmax,"\t| gamma2:",Gmax2,"\t| C:",Cmax,"\t| C2:",Cmax2); #prints maxes for dataset d
                        plt.plot(fpr, tpr, '.', markersize=0.75);
                        plt.show();
                        pts = np.transpose(np.stack((fpr, tpr))); #combin 1x(21*9) arrays into one 2x(21*9) array
                        pts = np.concatenate((pts, [[min(fpr)-0.00001,0.0],[0.99999, max(tpr)],[1,0]])); #add points for obtaining correct convex hull
                        hull = ConvexHull(pts);
                        ch = [];
                        chX = [];
                        chY = [];
                        for vertex in hull.vertices:
                                ch.append(pts[vertex]);#obtain the points in the convex hull
                        ch = sorted(ch, key=lambda x: x[0])
                        for vertex in ch:
                                chX.append(vertex[0]);#obtain X coordinates
                                chY.append(vertex[1]);#and Y coordinates of points in convex hull
                        plt.plot(chX, chY, "ro-", color='k', linewidth=0.8); #plot the convex hull
                        area = -np.trapz(chX, chY); #obtain AUC using composite trapezoidal rule 
                        plt.title('ROC %.4f'%(area));
                        plt.show();
                        exit();
                else:
                        print("\n");
                        print("\tSupport Vector Machine(Kernel = "+i+"):\n")
                        Y_pred = cross_val_predict(SVM, X, Y, cv=10); #obtain prediction from 10-fold cros val
                        cm = confusion_matrix(Y, Y_pred); #obtain confusion_matrix
                        printMetrics(cm);
                        plotData(X, Y_pred, d+" Kernel="+i);
