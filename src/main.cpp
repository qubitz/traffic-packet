// Program Information ////////////////////////////////////////////////////////
/**
 * @file Main.cpp
 *
 * @brief Driver program
 *
 * @details
 *
 * @version 1.1
 *          Brandon Thai Nguyen (06 September 2016)
 *          Updated for use with simulator class
 *
 *          1.0
 *          Tyler Goffinet (28 September 2016)
 *          Original Code
 *
 * @note
 */
// Precompiler directives /////////////////////////////////////////////////////

// none

// Header Files ///////////////////////////////////////////////////////////////

#include <iostream>
#include "simulator.h"

// Global Constant Definitioans  //////////////////////////////////////////////

// Function Prototypes ////////////////////////////////////////////////////////

// Main Function Implementation ///////////////////////////////////////////////

int main( int argc, char **argv )
{
  // Varible Declaration
  Simulator simulator;

  // Run Simulation
  simulator.run();

  // Return
  return EXIT_SUCCESS;
}

// Function Implementation ////////////////////////////////////////////////////

// None
