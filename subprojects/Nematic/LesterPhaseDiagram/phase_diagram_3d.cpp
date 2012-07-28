#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

struct FreeEnergySurfer
{
    double rho;							// density phase space coordinate
    double tau;							// ferromagnetic phase space coordinate
    double omega;						// nematic phase space coordinate
	double f_rho;						// "force" in rho direction
	double f_tau;						// "force" in tau direction
	double f_omega;						// "force" in omega direction
	double energy;						// current free energy
};

enum Phase{
  NONE, FLUID, PHASE_SEP, FERRO, NEMATIC
};




// GLOBALS
double T, beta;							// temperature / inverse temperature
double J, Q, Q2;                        // interaction parameters
double K_max, kappa_max, kappa2_max;	// maximum extent of phase diagram
int R;                                  // number of rotational degrees of freedom
double t_max;							// maximum time
int steps;                              // number of integration steps
int resolution;							// resolution of phase diagram
double mob_rho, mob_tau, mob_omega;     // translational and rotational mobilities
double mu;                              // chemical potential (set to zero for phase coexistence)

// FUNCTIONS
void heun(FreeEnergySurfer&);
void compute_forces(FreeEnergySurfer&);





int main(int argc, char** argv)
{
    unsigned int i,j,k;
    FreeEnergySurfer surfer;

    FILE *pFile;

    // Read command line arguments
	if (argc < 9 || sscanf(argv[1],"%lf",&T) != 1 || sscanf(argv[2],"%d",&R) != 1 || sscanf(argv[3], "%lf", &K_max) != 1 || sscanf(argv[4], "%lf", &kappa_max) != 1
		|| sscanf(argv[5], "%lf", &kappa2_max) != 1 || sscanf(argv[6], "%lf", &t_max) !=1 || sscanf(argv[7], "%d", &steps) != 1 || sscanf(argv[8], "%d", &resolution) != 1)
	{
		printf("usage: ./phase_diagram T R K_max kappa_max kappa2_max t_max steps resolution\n");
		exit(1);
	}

    beta = 1.0/T;
    mob_rho = mob_tau = mob_omega = 1.0;

    std::ostringstream filename;
	filename.setf(ios::fixed, ios::floatfield);
	filename.precision(4);
	filename.str("");

    // construct filename from input parameters
	filename << "phase_diagram_3d_T_" << T << "_R_" << R << ".txt";

	// wipe file
	pFile = fopen(filename.str().c_str(),"w");
	fclose(pFile);

	// phase diagram increments
	double K_step = K_max/resolution;
	double kappa_step = kappa_max/resolution;
	double kappa2_step = kappa2_max/resolution;

	// minimum energy of final state
	double min_energy;
	double ferro_nem_gap; //the gap between the ferro and nematic phases
	bool ferro_exist;
	bool nem_exist;

	// flag for final state (0 = F, 1 = PS, 2 = ferro, 3 = nematic)
        Phase phase;

	cout << "Generating phase diagram, be patient..." << endl;

	for (i=0;i<resolution;i++)
	{
		J = 4*(i+1)*K_step;
		mu = -J/2.0;

		for (j=0;j<resolution;j++)
		{
			Q = 4*(j+1)*kappa_step;

			for (k=0;k<resolution;k++)
			{
				Q2 = 4*(k+1)*kappa2_step;

				// set initial energy
				min_energy = 1e6;
				ferro_exist = 0;
				nem_exist = 0;
				ferro_nem_gap = 0;

				// set initial phase
				phase = NONE;

				// start in disordered fluid phase
				surfer.rho = 0.5 + 1e-3;
				surfer.tau = 1e-3;
				surfer.omega = 1e-3;

				heun(surfer);

				// check whether phase is stable
				if (surfer.rho < 0.5 + 1e-2)
				{
					if (surfer.tau < 1e-2)
					{
						if (surfer.omega < 1e-2)
						{
							min_energy = surfer.energy;
							phase = FLUID;
						}
					}
				}

				// start in phase separated phase
				surfer.rho = 0.9;
				surfer.tau = 1e-3;
				surfer.omega = 1e-3;

				heun(surfer);

				// check whether phase is stable
				if (surfer.rho > 0.5 + 1e-2)
				{
					if (surfer.tau < 1e-2 && surfer.tau > -0.2)
					{
						if (surfer.omega < 1e-2 && surfer.omega > -0.2)
						{
							if (surfer.energy < min_energy)
							{
								phase = PHASE_SEP;
								min_energy = surfer.energy;
							}
						}
					}
				}

				// start in ferromagnetic phase
				surfer.rho = 0.9;
				surfer.tau = 0.9;
				surfer.omega = 1e-3;

				heun(surfer);

				// check whether ferromagnetic phase is stable
				if (surfer.tau > 1e-2)
				{
					if (surfer.omega < surfer.tau)
					{
                                                ferro_exist = 1;
						ferro_nem_gap = surfer.energy;
						if (surfer.energy < min_energy)
						{
							phase = FERRO;
							min_energy = surfer.energy;
							
						}
					}
				}

				// start in nematic phase
				surfer.rho = 0.9;
				surfer.tau = 1e-3;
				surfer.omega = 0.9;

				heun(surfer);

				// check whether nematic phase is stable
				if (surfer.omega > 1e-2)
				{
					if (surfer.tau < surfer.omega)
					{
                                                nem_exist = 1;
						ferro_nem_gap -= surfer.energy;
						if (surfer.energy < min_energy)
						{
							phase = NEMATIC;
							min_energy = surfer.energy;
						}
					}
				}

				pFile = fopen(filename.str().c_str(),"a");
				if (nem_exist && ferro_exist){
				if (phase == FLUID) 
				  fprintf(pFile, "%5.4f %5.4f %5.4f %s %5.24f\n", J/4, Q/4, Q2/4, "H", ferro_nem_gap);
				else if (phase == PHASE_SEP) 
				  fprintf(pFile, "%5.4f %5.4f %5.4f %s %5.24f\n", J/4, Q/4, Q2/4, "PS", ferro_nem_gap);
				else if (phase == FERRO) 
				  fprintf(pFile, "%5.4f %5.4f %5.4f %s %5.24f\n", J/4, Q/4, Q2/4, "F", ferro_nem_gap);
				else if (phase == NEMATIC) 
				  fprintf(pFile, "%5.4f %5.4f %5.4f %s %5.24f\n", J/4, Q/4, Q2/4, "N", ferro_nem_gap);
				}
				else{
				if (phase == FLUID) 
				  fprintf(pFile, "%5.4f %5.4f %5.4f %s NA\n", J/4, Q/4, Q2/4, "H");
				else if (phase == PHASE_SEP) 
				  fprintf(pFile, "%5.4f %5.4f %5.4f %s NA\n", J/4, Q/4, Q2/4, "PS");
				else if (phase == FERRO) 
				  fprintf(pFile, "%5.4f %5.4f %5.4f %s NA\n", J/4, Q/4, Q2/4, "F");
				else if (phase == NEMATIC) 
				  fprintf(pFile, "%5.4f %5.4f %5.4f %s NA\n", J/4, Q/4, Q2/4, "N");
				}
				fclose(pFile);
			}
		}
	}

	cout << "Done!" << endl;

    return 0;
}

void heun(FreeEnergySurfer &surfer)
{
	double dt = t_max/steps;
	double rho, tau, omega;
	double f_rho, f_tau, f_omega;

	for (unsigned int i=0;i<steps;i++)
	{
		compute_forces(surfer);

		// store forces
		f_rho = surfer.f_rho;
		f_tau = surfer.f_tau;
		f_omega = surfer.f_omega;

		// store initial positions
		rho = surfer.rho;
		tau = surfer.tau;
		omega = surfer.omega;

		// update positions
		surfer.rho += dt*surfer.f_rho;
		surfer.tau += dt*surfer.f_tau;
		surfer.omega += dt*surfer.f_omega;

		// second pass
		compute_forces(surfer);

		// update forces
		f_rho += surfer.f_rho;
		f_tau += surfer.f_tau;
		f_omega += surfer.f_omega;

		// update positions
		surfer.rho = rho + 0.5*dt*f_rho;
		surfer.tau = tau + 0.5*dt*f_tau;
		surfer.omega = omega + 0.5*dt*f_omega;
	}

	// compute final energy
	double exponent = beta*Q*surfer.tau;
	double exponent2 = beta*Q2*surfer.omega;
    double cosh = 0.5*(exp(exponent) + exp(-exponent));
	surfer.energy = 0.5*(J*surfer.rho*surfer.rho + Q*surfer.tau*surfer.tau + Q2*surfer.omega*surfer.omega) - T*log(1 + (1.0/R)*exp(beta*(J*surfer.rho + mu))*(R - 4.0 + 2.0*exp(-exponent2) + 2.0*exp(exponent2)*cosh));
}

void compute_forces(FreeEnergySurfer &surfer)
{
    double exponent = beta*Q*surfer.tau;
    double cosh = 0.5*(exp(exponent) + exp(-exponent));
    double sinh = 0.5*(exp(exponent) - exp(-exponent));

    double e1 = exp(beta*(J*surfer.rho + mu));
	double e2 = exp(beta*Q2*surfer.omega);
	double e3 = exp(2.0*beta*Q2*surfer.omega);
	double e4 = exp(beta*(J*surfer.rho + Q2*surfer.omega + mu));
	double e5 = exp(beta*(J*surfer.rho + 2.0*Q2*surfer.omega + mu));
	double e6 = exp(-beta*Q2*surfer.omega);

    double numerator = 2.0*e1*(e3*cosh - 1.0);
    double denominator = 2.0*e1 + (R-4.0)*e4 + e2*R + 2.0*e5*cosh;

    // compute force component in rho direction
    surfer.f_rho = -mob_rho*J*(surfer.rho - 1 + ((e2*R)/denominator));

    // compute force component in omega direction
    surfer.f_omega = -mob_omega*Q2*(surfer.omega - (numerator/denominator));

	numerator = 2.0*e4*sinh;
	denominator = R*(1.0 + (1.0/R)*(e1*(R - 4.0 + 2.0*e6 + 2.0*e2*cosh)));

    // compute force component in tau direction
    surfer.f_tau = -mob_tau*Q*(surfer.tau - (numerator/denominator));
}
