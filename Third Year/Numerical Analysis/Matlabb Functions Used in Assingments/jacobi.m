function [X, iter] = jacobi( A, B, P, delta, maxI)
  
% Input - A is an N x N nonsingular matrix
%       - B is an N x 1 matrix
%       - P is an N x 1 matrix; initial guess
%       - dealta is the tolerance for P
%       - maxI is the maximum number of iteration
% Output- X ius an N x 1 matrix: the jacobi approximation to the solution of AX = B
%       - iter is the number of iterations

N = length(B);

for k=1:maxI
    for j=1:N
      X(j)=(B(j)-A(j,[1:j-1,j+1:N])*P([1:j-1,j+1:N]))/A(j,j);
    end
    
    err=abs(norm(X'-P));
    relerr=err/(norm(X)+eps);
    
    P=X';
    
    if (err<delta||relerr<delta)
      break
    end
end

iter = k;
X = X';
      