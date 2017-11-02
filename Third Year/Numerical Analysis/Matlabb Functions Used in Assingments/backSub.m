function X=backSub(A,B)

%Input  - A is an n x n upper-trianglar nonsingular matrix
%       - B is an n x 1 matrix
%Output - X is the solution to the linear system AX = B

%Find the dimension of B and initialize X
n=length(B);
X=zeros(n,1);%creates nx1 matrix of 0's

X(n)=B(n)/A(n,n);
for k=n-1:-1:1
  X(k) = (B(k)-A(k,k+1:n)*X(k+1:n))/A(k,k);
end