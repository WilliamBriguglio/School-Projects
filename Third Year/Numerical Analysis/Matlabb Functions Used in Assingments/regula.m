function [c iter err Yc] = regula(f, a, b, delta, epsilon, maxIt)
%Input - f is the function input as a string 'f'
%      - a and be are the left and right end points
%      - delta is the tolerance
%      - epsilon is the tolerance for the value of f at the zero
%      - maxIt is the maximum number of iterations
%Output- c is the zero
%      - iter is the number of iterations performed
%      - Yc = f(c)
%      - err is the error estimate for c
ya = feval(f, a); 
yb = feval(f, b); 
if ya*yb > 0
  disp('Note: f(a)*f(b)>0'),
  break, 
end
for k=1:maxIt
  dx = yb*(b-a)/(yb-ya);
  c=b-dx;
  ac=c-a;
  yc= feval(f,c);
  if yc == 0, break;
  elseif yb*yc>0
    b=c;
    yb = yc; 
  else 
    a= c;
    ya= yc;
  end
  dx=min(abs(dx),ac);
  if abs(dx)<delta,break,end
  if abs(yc)<epsilon,break,end
end
c;
err=abs(b-a)/2;
Yc=feval(f,c)
iter = k;