function [P,iter]= jacobi2(G,P,delta,maxIt)

%Input - G is the nonlinear system saved in the M-file G.m
%      - P is the initial guess at the solution
%      - delta is the error bound
%      - maxIt is the number of iterations
%Output- P is the seidel approximation to the solution
%      - iter is the number of iterations required



N=length(P);

for k=1:maxIt
   X=P;
   % X is the kth apporximation to the solution
   for j=1:N     
      A=feval(G,X);
   end
   X=A;
   err=abs(norm(X-P));
   relerr=err/(norm(X)+eps);
   P=X;
   iter=k;
   if (err<delta)||(relerr<delta)
     break
   end
end
