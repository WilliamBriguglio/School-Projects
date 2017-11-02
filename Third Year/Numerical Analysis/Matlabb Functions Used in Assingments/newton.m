function [p0,err,k,y]=newton(f,df,p0,delta,epsilon,maxIt)
%Input - f is the object function input as a string 'f'
%      - df is the derivative of f imput as a string 'df'
%      - p0 is the initial approximation
%	     - delta is the tolerance for p0
%	     - epsilon is the tolerance for the function values y
%	     - maxIt is the maximum number of iterations
%Output- p0 is the Newton-Raphson approx to the zero
%	     - err is the error estimate for p0
%	     - k is the number of iterations
%	     - y is the function value f(p0)

for k=1:maxIt
	p1=p0-feval(f,p0)/feval(df,p0);	
	err=abs(p1-p0);
	relerr=2*err/(abs(p1)+delta);
	p0=p1;
	y=feval(f,p0);
	if (err<delta)||(relerr<delta)||(abs(y)<epsilon),break,end
end
