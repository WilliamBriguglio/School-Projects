function [X, iter] = seidel(A,B,P,delta,maxI)
%Input - A is an N x N nonsingular matrix
%      - B is an N x 1 matrix
%      - P is and N x 1 matric aka the initial guess
%      - delta is the tolerance for X0
%      - maxI is the maximum number of iterations
%Output- X is an N x 1 matrix: the gaus-seidel approximation to the solution of AX = B
%      - iter is the number of iterations

N = length(B);

for k=1:maxI
  for j=1:N
    if j==1
      X(1) = (B(1)-A(1,2:N)*P(2:N))/A(1,1);
    elseif j == N
      X(N) = (B(N)-A(N,1:N-1)*(X(1:N-1))')/A(N,N);
    else 
      % X contains the kth approximation and P the (k-1)st
      X(j) = (B(j)-A(j,1:j-1)*X(1:j-1)'-A(j,j+1:N)*P(j+1:N))/A(j,j);
    end
  end
  err = abs(norm(X'-P));
  relerr = err/(norm(X)+eps);
  P = X';
  if(err<delta)||(relerr<delta)
    break
  end
end

iter = k; 
X = X';