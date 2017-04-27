// Program Information ////////////////////////////////////////////////////////
/**
 * @file intersect.h
 *
 * @author Tyler Michael Goffinet
 *
 * @brief Declaration file for intersect class(es)
 *
 * @details Declares all member methods and structures of the intersect
 *          class(es)
 *
 * @version 1.0
 *          Tyler Michael Goffinet (20 October 2016)
 *          Original Code
 *
 * @note None
 */
// Precompiler directives /////////////////////////////////////////////////////

#ifndef CLASS_INTERSECT_H
#define CLASS_INTERSECT_H

// Header files ///////////////////////////////////////////////////////////////

#include <sstream>
#include <string>

#include "vehicle.h"

// Class implementation ///////////////////////////////////////////////////////

class Intersect1
{
 public:
  static const int         NUM_ROWS = 5;
  static const std::string UPPER_SCORE;
  static const std::string VERT_ROADWAY[5];
  static const std::string HORI_ROADWAY[1];

  // Constructors
  Intersect1();
  Intersect1( char id, bool holdsAsterisk, VehicleDir dir );
  ~Intersect1();

  // Modifiers
  void setIntersection( char id, bool holdsAsterisk, VehicleDir dir );

  // Accessors
  std::string operator[]( int row );
  std::string getIntersection( int row );

 private:
  // Object properties
  char       objId;
  bool       hasAsterisk;
  VehicleDir objDir;

  // Intersection string data
  std::string intersection[5];

  // Helper functions
  void constructIntersection();
};

#endif  // #ifndef CLASS_INTERSECT_Hs
