
rhoNEPCM = ((1+l)*rhoc*rhos)/(rhos+l*rhoc);
DB=kB*T/(3.0*3.14*muf*ds);
DT=0.26*(kappaf/(2.0*kappaf+kappaNEPCM))*(muf/rhof)*alpha;
rho = alpha*rhoNEPCM+(1-alpha)*rhof;
mu = (muf/pow((1.0-alpha),2.5));

// Time derivative matrix
fvVectorMatrix ddtUEqn(fvm::ddt(U));

// Convection-diffusion matrix
fvVectorMatrix HUEqn
(
    fvm::div(phi, U)
  + turbulence->divDevReff()
);

if (pimple.momentumPredictor())
{
    // Solve momentum predictor
    solve(relax(ddtUEqn + HUEqn) == -fvc::grad(p));
}
else
{
    // Explicit update
    U = (ddtUEqn.H() + HUEqn.H() - fvc::grad(p))/(HUEqn.A() + ddtUEqn.A());
    U.correctBoundaryConditions();
}
