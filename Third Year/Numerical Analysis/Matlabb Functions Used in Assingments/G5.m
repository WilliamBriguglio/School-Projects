function Z = G5(X)
x=X(1);
y=X(2);
Z=zeros(1,2);
Z(1)= (1 - 2*y)/3;
Z(2)= ((3 - x.^2)/2).^(1/2);