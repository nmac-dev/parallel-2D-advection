/// 2D advection example program which advects a Gaussian u(x,y) at a fixed velocity

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <omp.h>

/// Prototypes
float calc_vel_x(float height);

int main()
{
	/**
	 * Grid properties
	 */
	const int NX = 1000;	   // Number of x points
	const int NY = 1000;	   // Number of y points
	const float MIN_X = 0.0f;  // Minimum x value
	const float MAX_X = 30.0f; // Maximum x value
	const float MIN_Y = 0.0f;  // Minimum y value
	const float MAX_Y = 30.0f; // Maximum y value

	/**
	 * Parameters for the Gaussian initial conditions
	 */
	const float CENTER_X = 3.0f;			  // Center(x)
	const float CENTER_Y = 15.0f;			  // Center(y)
	const float SIGMA_X  = 1.0f;			  // Width(x)
	const float SIGMA_Y  = 5.0f;			  // Width(y)
	const float SIGMA_X2 = SIGMA_X * SIGMA_X; // Width(x) squared
	const float SIGMA_Y2 = SIGMA_Y * SIGMA_Y; // Width(y) squared

	/** 
	 * Boundary conditions
	 */
	const float BVAL_LEFT  = 0.0f;	// Left boudnary value
	const float BVAL_RIGHT = 0.0f; 	// Right boundary value
	const float BVAL_LOWER = 0.0f; 	// Lower boundary
	const float BVAL_UPPER = 0.0f; 	// Upper bounary

	/**
	 * Time stepping parameters 
	 */
	const float CFL = 0.9f;	  // CFL number
	const int N_STEPS = 800;  // Number of time steps

	/**
	 * Velocity
	 */
	const float VEL_X = calc_vel_x(MAX_Y); // Velocity in x direction
	const float VEL_Y = 0.0f;			   // Velocity in y direction

	/**
	 * Calculate distance between points
	 */
	const float DIST_X = (MAX_X - MIN_X) / ((float)NX);
	const float DIST_Y = (MAX_Y - MIN_Y) / ((float)NY);

	/**
	 * Calculate time step using the CFL condition 
	 */
	const float TIME_STEP = CFL / ((fabs(VEL_X) / DIST_X) + (fabs(VEL_Y) / DIST_Y));

	/**
	 * Arrays to store variables, NX+2 elements to allow boundary values to be stored at both ends 
	 */
	float x_axis[NX + 2];			// x-axis values
	float y_axis[NX + 2];			// y-axis values
	float u_vals[NX + 2][NY + 2];	// Array of u values
	float  roc_u[NX + 2][NY + 2];	// Rate of change of u
	float *u_vad = (float *)calloc(NX, sizeof(float)); // Stores vertical averages for u_vals[x,y] (calloc: init indexes as 0)

	float sq_x2; // x squared (used to calculate iniital conditions)
	float sq_y2; // y squared (used to calculate iniital conditions)

	/**
	 * Report information about the calculation
	 */
	printf("Grid spacing dx     = %g\n", DIST_X);
	printf("Grid spacing dy     = %g\n", DIST_Y);
	printf("CFL number          = %g\n", CFL);
	printf("Time step           = %g\n", TIME_STEP);
	printf("No. of time steps   = %d\n", N_STEPS);
	printf("End time            = %g\n", TIME_STEP * (float)N_STEPS);
	printf("Distance advected x = %g\n", VEL_X * TIME_STEP * (float)N_STEPS);
	printf("Distance advected y = %g\n", VEL_Y * TIME_STEP * (float)N_STEPS);

	/**
	 * 	WARNING: GCC 9 or older implements the changes made to default(none) in OpenMP 4.0
	 *	default(none) NO LONGER predetermines "const variables with no mutable members" as shared (breaking backwards compatibility)
	 *	To implement a program which compiles for any GCC version, any const variables will be declared as "firstprivate"
	 */
	#pragma omp parallel default(none) shared(x_axis, y_axis, u_vals) \
				firstprivate(NX, NY, DIST_X, DIST_Y, CENTER_X, CENTER_Y, SIGMA_X2, SIGMA_Y2)
	{
		/**
		* Place x points in the middle of the cell
		*/
		/// LOOP 1
		#pragma omp for
		for (int i = 0; i < NX + 2; i++)
		{
			x_axis[i] = ((float)i - 0.5f) * DIST_X;
		}

		/**
		* Place y points in the middle of the cell
		*/
		/// LOOP 2
		#pragma omp for
		for (int j = 0; j < NY + 2; j++)
		{
			y_axis[j] = ((float)j - 0.5f) * DIST_Y;
		}

		/**
		* Set up Gaussian initial conditions
		*/
		/// LOOP 3
		#pragma omp for private(sq_x2, sq_y2)
		for (int i = 0; i < NX + 2; i++)
		{
			for (int j = 0; j < NY + 2; j++)
			{
				sq_x2 = (x_axis[i] - CENTER_X) * (x_axis[i] - CENTER_X);
				sq_y2 = (y_axis[j] - CENTER_Y) * (y_axis[j] - CENTER_Y);
				u_vals[i][j] = exp(-1.0f * ((sq_x2 / (2.0f * SIGMA_X2)) + (sq_y2 / (2.0f * SIGMA_Y2))));
			}
		}
	}

	/** 
	 * Write array of initial u values out to file
	 */
	FILE *initial_file;
	initial_file = fopen("initial.dat", "w");
	/// LOOP 4
	///	  No parallel implementation:
	///		Writing to a singular file using multiple threads breaks output dependency
	///		Each thread overwrites the other threads changes
	for (int i = 0; i < NX + 2; i++)
	{
		for (int j = 0; j < NY + 2; j++)
		{
			fprintf(initial_file, "%g %g %g\n", x_axis[i], y_axis[j], u_vals[i][j]);
		}
	}
	fclose(initial_file);

	/**
	 * Process rows sequentially and columns in parallel
	 *	placing the parallel block in the outer scope of the sequential loop reduces overhead for calling threads
	 */
	#pragma omp parallel default(none) shared(x_axis, y_axis, u_vals, roc_u, u_vad) \
			firstprivate(NX, NY, DIST_X, DIST_Y, VEL_Y, N_STEPS, TIME_STEP, BVAL_LEFT, BVAL_RIGHT, BVAL_LOWER, BVAL_UPPER)
	{
		/**
		* Update solution by looping over time steps
		*/
		/// LOOP 5
		///	  No parallel implementation (outer loop only) . . .
		///		Each iteration of the outer loop depends upon the updates made by the previous
		///		Flow dependency is broken in parallel as the time steps become unordered
		for (int m = 0; m < N_STEPS; m++)
		{
			/**
			* Apply boundary conditions at u_vals[0][:] and u_vals[NX+1][:]
			*/
			/// LOOP 6
			#pragma omp for
			for (int j = 0; j < NY + 2; j++)
			{
				u_vals[0][j] = BVAL_LEFT;
				u_vals[NX + 1][j] = BVAL_RIGHT;
			}

			/**
			* Apply boundary conditions at u_vals[:][0] and u_vals[:][NY+1]
			*/
			/// LOOP 7
			#pragma omp for
			for (int i = 0; i < NX + 2; i++)
			{
				u_vals[i][0] = BVAL_LOWER;
				u_vals[i][NY + 1] = BVAL_UPPER;
			}

			/** 
			* Calculate rate of change of u using leftward difference
			* 	Loop over points in the domain but not boundary values
			*/
			/// LOOP 8
			#pragma omp for
			for (int i = 1; i < NX + 1; i++)
			{
				for (int j = 1; j < NY + 1; j++)
				{
					roc_u[i][j] = -calc_vel_x(y_axis[j]) * (u_vals[i][j] - u_vals[i - 1][j]) / DIST_X - VEL_Y * (u_vals[i][j] - u_vals[i][j - 1]) / DIST_Y;
				}
			}

			/**
			* Update u from t to t+TIME_STEP & Calculate vertically averaged distribution
			* 	Loop over points in the domain but not boundary values
			*/
			/// LOOP 9
			#pragma omp for
			for (int i = 1; i < NX + 1; i++)
			{
				for (int j = 1; j < NY + 1; j++)
				{
					u_vals[i][j] = u_vals[i][j] + roc_u[i][j] * TIME_STEP;
					u_vad[i - 1] += u_vals[i][j];	// Sum vertical u values (y)
				}
				u_vad[i - 1] /= NY; // Divide sum by total points of y to get vertical average
			}
		}
	}

	/**
	 * Write array of final u values out to file
	 */
	FILE *final_file;
	final_file = fopen("final.dat", "w");
	/// LOOP 10
	///   No parallel implementation: same case as LOOP 4
	for (int i = 0; i < NX + 2; i++)
	{
		for (int j = 0; j < NY + 2; j++)
		{
			fprintf(final_file, "%g %g %g\n", x_axis[i], y_axis[j], u_vals[i][j]);
		}
	}
	fclose(final_file);

	/**
	 * Write array for vertically averaged distribution of u_vals[x,y] out to file
	 */
	FILE *v_averaged_file;
	v_averaged_file = fopen("v_averaged.dat", "w");
	/// LOOP 11
	///   No parallel implementation: same case as LOOP 4
	for (int i = 0; i < NX; i++)
	{
		fprintf(v_averaged_file, "%g %g\n", x_axis[i], u_vad[i]);
	}
	fclose(v_averaged_file);

	free(u_vad);

	return 0;
}

/**
 * @brief Calculates the horizontal velocity using a logarithmic profile (adds vertical shear)
 *
 * @param height y axis value
 */ 
float calc_vel_x(float height)
{
	const float F_VEL    = 0.2f;  // Friction velocity (meters per second)
	const float R_LEN    = 1.0f;  // Roughness length 
	const float VK_CONST = 0.41f; // Von Karman's constant

	float vel_x = 0;

	/// Apply log profile if height is greater than roughness length
	if (height > R_LEN)
	{
		vel_x = (F_VEL / VK_CONST) * logf(height / R_LEN);
	}
	return vel_x;
}