#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

struct Bead
{
	double x,y,z;			// x/y/z coordinates of bead
	double x0,y0,z0;		// x/y/z coordinates of bead at previous iteration
	double dVx,dVy,dVz;		// x,y/z gradient of free energy surface at bead location
	double e;				// energy at current bead position

	// bead parameters
	double g;				// intepolation position
	double lxyz;			// arc length
};

// GLOBALS
double J, Q, Q2, T, beta, mu;		// free energy surface parameters
int R;								// number of orientational states
int nBeads;							// number of beads on string
int max_steps;						// maximum number of string integration steps
double timestep, tolerance;			// integration timestep and convergence tolerance
double X_START, X_END;				// start / end point of string in phi direction
double Y_START, Y_END;				// start / end point of string in tau direction
double Z_START, Z_END;				// start / end point of string in omega direction

// FUNCTIONS
void initialise_string(Bead*);
double take_step(Bead*);
void compute_energy(Bead*);
void compute_forces(Bead*);
void simple_integrate(Bead*);
void heun_integrate(Bead*);
void reparametrise_string(Bead*);
void interpolate_positions(Bead*);
double compute_barrier_heights(Bead*);

int main(int argc, char** argv)
{
    int i;
	double tol;
    FILE *pFile, *pFile2;

    // read command line arguments
	if (argc < 10 || sscanf(argv[1],"%lf",&J) != 1 || sscanf(argv[2],"%lf",&Q) != 1 || sscanf(argv[3],"%lf",&Q2) != 1 || sscanf(argv[4], "%lf", &T) != 1 || sscanf(argv[5], "%d", &R) != 1
            ||  sscanf(argv[6], "%d", &nBeads) != 1 || sscanf(argv[7], "%lf", &timestep) != 1 || sscanf(argv[8], "%d", &max_steps) != 1 ||  sscanf(argv[9], "%lf", &tolerance) != 1 || sscanf(argv[10], "%lf", &X_END) != 1 || sscanf(argv[11], "%lf", &Y_END) !=1 || sscanf(argv[12], "%lf", &Z_END) != 1)
	{
		printf("usage: ./mep J Q Q2 T R num_beads dt max_steps tolerance X_END Y_END Z_END\n");
		exit(1);
	}

	X_START = 0;
	if (J > 4) X_START = -1;

	Y_START = Z_START = 0;

    std::ostringstream filename,filename2, filename3;

	filename.setf(ios::fixed, ios::floatfield);
	filename.precision(4);
	filename.str("");

	filename2.setf(ios::fixed, ios::floatfield);
	filename2.precision(4);
	filename2.str("");

	filename3.setf(ios::fixed, ios::floatfield);
	filename3.precision(4);
	filename3.str("");

    // construct filename from input parameters
	filename << "mep_J_" << J << "_Q_" << Q << "_Q2_" << Q2 << "_T_" << T << "_R_" << R << ".txt";
	filename2 << "energy_J_" << J << "_Q_" << Q << "_Q2_" << Q2 << "_T_" << T << "_R_" << R << ".txt";
	filename3 << "direct_energy_J_" << J << "_Q_" << Q << "_Q2_" << Q2 << "_T_" << T << "_R_" << R << ".txt";

	// initialise array of beads
	Bead beads[nBeads];

	// set inverse temperature and chemical potential
	beta = 1.0/T;
	mu = -J/2.0;

	double e_direct, e_mep;

	// initialise the string coordinates
    initialise_string(beads);

	bool converged = false;

	for (i=0;i<max_steps;i++)
	{
		tol = take_step(beads);

		if (tol < tolerance)
		{
			converged = true;
			break;
		}
	}

	compute_energy(beads);
	e_mep = compute_barrier_heights(beads);
	
	cout << "Exited after " << i << " steps." << endl;
	if (converged) cout << "Path converged to desired tolerance." << endl;

    // overwrite existing files
    pFile = fopen(filename.str().c_str(), "w");
    pFile2 = fopen(filename2.str().c_str(), "w");

	for (i=0;i<nBeads;i++)
	{
		fprintf(pFile, "%5.4f %5.4f %5.4f\n", beads[i].x, beads[i].y, beads[i].z);
		fprintf(pFile2, "%5.4f %5.4f\n", beads[i].g, beads[i].e-beads[0].e);
	}

    fclose(pFile);
    fclose(pFile2);

	// re-define string as direct path from initial to final basin
	X_START = beads[0].x;
	X_END = beads[nBeads-1].x;
	Y_START = beads[0].y;
	Y_END = beads[nBeads-1].y;
	Z_START = beads[0].z;
	Z_END = beads[nBeads-1].z;

	initialise_string(beads);

	compute_energy(beads);
	e_direct = compute_barrier_heights(beads);

	// write direct energy path to file
    pFile = fopen(filename3.str().c_str(), "w");

	for (i=0;i<nBeads;i++)
	{
		fprintf(pFile, "%5.4f %5.4f\n", beads[i].g, beads[i].e-beads[0].e);
	}

    fclose(pFile);

    return 0;
}

void initialise_string(Bead *beads)
{
	double dx = 1.0 / (nBeads-1);

	for (int i=0;i<nBeads;i++)
	{
		beads[i].g = i*dx;
		beads[i].x = X_START + (X_END - X_START)*beads[i].g;
		beads[i].y = Y_START + (beads[i].x - X_START)*(Y_END - Y_START)/(X_END - X_START);
		beads[i].z = Z_START + (beads[i].x - X_START)*(Z_END - Z_START)/(X_END - X_START);
	}

	reparametrise_string(beads);
}

double take_step(Bead *beads)
{
	int i;

	heun_integrate(beads);
	reparametrise_string(beads);

	double tolx,toly,tolz,tol;
	tolx = toly = tolz = 0;

	for (i=0;i<nBeads;i++)
	{
		tolx += (beads[i].x - beads[i].x0)*(beads[i].x - beads[i].x0);
		toly += (beads[i].y - beads[i].y0)*(beads[i].y - beads[i].y0);
		tolz += (beads[i].z - beads[i].z0)*(beads[i].z - beads[i].z0);
	}

	tol = (sqrt(tolx) + sqrt(toly) + sqrt(tolz)) / nBeads;

	return tol;
}

void compute_energy(Bead *beads)
{
    double exponent, cosh, rho;

	for (int i=0;i<nBeads;i++)
	{
		rho = 0.5*(1+beads[i].x);
		exponent = beta*Q*beads[i].y;
		cosh = 0.5*(exp(exponent) + exp(-exponent));
		exponent = beta*Q2*beads[i].z;

		beads[i].e = 0.5*(J*rho*rho + Q*beads[i].y*beads[i].y + Q2*beads[i].z*beads[i].z) - T*log(1 + (1.0/R)*exp(beta*(J*rho + mu))*(R - 4.0 + 2.0*exp(-exponent) + 2.0*exp(exponent)*cosh));
	}
}

void compute_forces(Bead *beads)
{
    double exponent, cosh, sinh;
	double e1, e2, e3, e4, e5, e6, rho;
	double numerator, denominator;

	for (int i=0;i<nBeads;i++)
	{
		rho = 0.5*(1+beads[i].x);

		exponent = beta*Q*beads[i].y;
		cosh = 0.5*(exp(exponent) + exp(-exponent));
		sinh = 0.5*(exp(exponent) - exp(-exponent));

		exponent = beta*(J*rho + mu);
		e1 = exp(exponent);
		e2 = exp(beta*Q2*beads[i].z);
		e3 = exp(2.0*beta*Q2*beads[i].z);
		e4 = exp(beta*(J*rho + Q2*beads[i].z + mu));
		e5 = exp(beta*(J*rho + 2.0*Q2*beads[i].z + mu));
		e6 = exp(-beta*Q2*beads[i].z);

		numerator = 2.0*e1*(e3*cosh - 1.0);
		denominator = 2.0*e1 + (R-4.0)*e4 + e2*R + 2.0*e5*cosh;

		// compute derivative in rho direction
		beads[i].dVx = J*(rho - 1 + ((e2*R)/denominator));

		// compute derivative in omega direction
		beads[i].dVz = Q2*(beads[i].z - (numerator/denominator));

		numerator = 2.0*e4*sinh;
		denominator = R*(1.0 + (1.0/R)*(e1*(R - 4.0 + 2.0*e6 + 2.0*e2*cosh)));

		// compute derivative in tau direction
		beads[i].dVy = Q*(beads[i].y - (numerator/denominator));
	}
}

void simple_integrate(Bead *beads)
{
	compute_forces(beads);

	for (int i=0;i<nBeads;i++)
	{
		beads[i].x0 = beads[i].x;
		beads[i].y0 = beads[i].y;
		beads[i].z0 = beads[i].z;

		beads[i].x -= timestep*beads[i].dVx;
		beads[i].y -= timestep*beads[i].dVy;
		beads[i].z -= timestep*beads[i].dVz;
	}
}

void heun_integrate(Bead *beads)
{
	int i;
	double dVx[nBeads];
	double dVy[nBeads];
	double dVz[nBeads];

	// first pass
	compute_forces(beads);

	// store forces
	for (i=0;i<nBeads;i++)
	{
		dVx[i] = beads[i].dVx;
		dVy[i] = beads[i].dVy;
		dVz[i] = beads[i].dVz;
	}

	// store initial positions
	for (i=0;i<nBeads;i++)
	{
		beads[i].x0 = beads[i].x;
		beads[i].y0 = beads[i].y;
		beads[i].z0 = beads[i].z;
	}

	// update positions
	for (i=0;i<nBeads;i++)
	{
		beads[i].x -= timestep*beads[i].dVx;
		beads[i].y -= timestep*beads[i].dVy;
		beads[i].z -= timestep*beads[i].dVz;
	}

	// second pass
	compute_forces(beads);

	// update forces
	for (i=0;i<nBeads;i++)
	{
		dVx[i] += beads[i].dVx;
		dVy[i] += beads[i].dVy;
		dVz[i] += beads[i].dVz;
	}

	// update positions
	for (i=0;i<nBeads;i++)
	{
		beads[i].x = beads[i].x0 - 0.5*timestep*dVx[i];
		beads[i].y = beads[i].y0 - 0.5*timestep*dVy[i];
		beads[i].z = beads[i].z0 - 0.5*timestep*dVz[i];
	}
}

void reparametrise_string(Bead *beads)
{
	int i;
	double dx,dy,dz;

	beads[0].lxyz = 0;

	// work out arc length
	for (i=1;i<nBeads;i++)
	{
		dx = beads[i].x - beads[i-1].x;
		dy = beads[i].y - beads[i-1].y;
		dz = beads[i].z - beads[i-1].z;
		beads[i].lxyz = beads[i-1].lxyz + sqrt(dx*dx + dy*dy + dz*dz);
	}

	// normalise arc length
	for (i=1;i<nBeads;i++)
	{
		beads[i].lxyz /= beads[nBeads-1].lxyz;
	}

	interpolate_positions(beads);
}

void interpolate_positions(Bead *beads)
{
	int i,j;

	double x[nBeads];
	double y[nBeads];
	double z[nBeads];

	// store old x/y coordinates
	for (i=0;i<nBeads;i++)
	{
		x[i] = beads[i].x;
		y[i] = beads[i].y;
		z[i] = beads[i].z;
	}

	for (i=0;i<nBeads;i++)
	{
		// assign a boundary value if out of range
		if (beads[i].g <= beads[0].lxyz)
		{
			beads[i].x = x[0];
			beads[i].y = y[0];
			beads[i].z = z[0];
		}
		else
		{
			if (beads[i].g >= beads[nBeads-1].lxyz)
			{
				beads[i].x = x[nBeads-1];
				beads[i].y = y[nBeads-1];
				beads[i].z = z[nBeads-1];
			}
			else
			{
				j = 0;

				// work out interval in which the point lies
				while (j <= nBeads-1)
				{
					if (beads[j].lxyz >= beads[i].g) break;
					j++;
				}

				// we don't assume linearly spaced points, hence the slightly more convoluted interpolation method
				beads[i].x = x[j-1] + (x[j] - x[j-1])*(beads[i].g - beads[j-1].lxyz)/(beads[j].lxyz - beads[j-1].lxyz);
				beads[i].y = y[j-1] + (y[j] - y[j-1])*(beads[i].g - beads[j-1].lxyz)/(beads[j].lxyz - beads[j-1].lxyz);
				beads[i].z = z[j-1] + (z[j] - z[j-1])*(beads[i].g - beads[j-1].lxyz)/(beads[j].lxyz - beads[j-1].lxyz);
			}
		}
	}
}

double compute_barrier_heights(Bead *beads)
{
	int i;
	double e,e1,e2,etot;

	e1 = e2 = etot = 0;
	
	for (i=0;i<nBeads;i++)
	{
		e = beads[i].e - beads[0].e;

		if (e < e2)
		{
			if (e2 > e1)
			{
				etot += e;
			}
		}

		e1 = e2;
		e2 = e;
	}

	return etot;
}
