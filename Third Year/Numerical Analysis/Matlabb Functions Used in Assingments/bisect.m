function [c iter err yc] = bisect(f, a, b, delta, maxIt)
%Input - f is the function input as string 'f'
%      - a and b are left and right end points
%      - delta is the tolerance
%      - maxIt is the maximum number of iterations
%Output- c is the zero
%      - iter is the number of iterations performed
%      - err is the error estimate for c
%      - yc = f(c)
ya = feval(f, a); 
yb = feval(f, b); 
if ya*yb>0, break, end

for k=1:maxIt
  c=(a+b)/2;
  yc = feval(f, c); 
  if yc == 0
    a=c;
    b=c;
  elseif yb*yc>0
    b=c;
    yb=yc; 
  else
    a=c;
    ya=yc;
  end
  if b-a < delta, break, end
end
c=(a+b)/2;
err=abs(b-a);
yc=feval(f,c);
iter = k;