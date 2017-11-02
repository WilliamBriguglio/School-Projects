function [lambda, V, iter] = invpow(A, X, alpha, epsilon, maxI)
%Input - A is an N x N  matrix
%      - X is the N x 1 starting vector
%      - alpha is the given shift
%      - epsilon is the tolerance
%      - maxI is the maximum number of iterations
%Output- lambda is the dominant eigenvalue
%      - V is the dominant eigenvector
%      - iter is the number of iterations

%Initialize the matrix A-alphaI and parameteres
[n n] = size(A);
A=A-alpha*eye(n);
lambda = 0; 
cnt = 0; 
err =1; 
state=1;

while((cnt<maxI)&&(state==1))
  %Solve system AY=X
  Y=A\X;
  %Normalize Y
  [m j]=max(abs(Y));
  c1=Y(j);
  dc = abs(lambda-c1);
  Y=(1/c1)*Y;
  %Update X and lambda and check for convergence
  dv = norm(X-Y);
  err= max(dc, dv);
  X=Y;
  lambda = c1; 
  state = 0; 
  if(err>epsilon)
    state =1; 
  end
  cnt = cnt+1;
end
lambda = alpha+1/c1;
V = X;
iter = cnt;
  