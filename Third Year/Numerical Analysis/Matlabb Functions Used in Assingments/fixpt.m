function [k, p, err, P] = fixpt(g, p0, tol, maxI)
%Input - g is the interation function as a string 'g;' 
%      - p0 is the initial guess for fixed point
%      - tol is the tolerance 
%      - maxI is the maximum number of iterations
%Output- k is the number of iterations that were carried out
%      - p is the approximation for the fixed point
%      - err is the error in the approximation 
%      - P contains the sequence {pn}

P(1) = p0; 
for k=2:maxI
    P(k) = feval(g, P(k-1));
    err = abs(P(k) - P(k-1));
    relerr = err/(abs(P(k))+eps);
    p = P(k);
    if(err<tol) || (relerr<tol), break;end
end

if k == maxI
  disp('max number of iterations exceeded')
end
P = P';