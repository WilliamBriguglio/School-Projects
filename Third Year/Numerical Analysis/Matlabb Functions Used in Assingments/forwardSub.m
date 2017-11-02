function X=forwardSub(A,B)

%Input  - A is an n x n lower-trianglar nonsingular matrix
%       - B is an n x 1 matrix
%Output - X is the solution to the linear system AX = B

%Find the dimension of B and initialize X
n=length(B);
X=zeros(n,1);%creates nx1 matrix of 0's

X(1)=B(1)/A(1,1);
for k=2:+1:n
  X(k) = (B(k)-A(k,1:k-1)*X(1:k-1))/A(k,k);
end