// Program Information ////////////////////////////////////////////////////////
/**
 * @file intersect.cpp
 *
 * @author Tyler Michael Goffinet
 *
 * @brief Implementation file for intersect class(es)
 *
 * @details Implements all member methods of the intersect class(es)
 *
 * @version
 *          1.0
 *          Tyler Goffinet (03 October 2016)
 *          Original Code
 *
 * @note Requires intersect.h
 */

// Precompiler directives /////////////////////////////////////////////////////

#ifndef CLASS_INTERSECT_CPP
#define CLASS_INTERSECT_CPP

// Header files ///////////////////////////////////////////////////////////////

#include "intersect.h"

using std::string;

// Class implementation ///////////////////////////////////////////////////////

const string Intersect1::UPPER_SCORE = "\u23ba";

const string Intersect1::VERT_ROADWAY[] = { "_", " ", " ", " ", UPPER_SCORE };

const string Intersect1::HORI_ROADWAY[] = { "" };

Intersect1::Intersect1()
{
  setIntersection( ' ', false, NaN );
}

Intersect1::Intersect1( char id, bool holdsAsterisk, VehicleDir dir )
{
  setIntersection( id, holdsAsterisk, dir );
}

Intersect1::~Intersect1()
{
}

void Intersect1::setIntersection( char id, bool holdsAsterisk, VehicleDir dir )
{
  objId       = id;
  hasAsterisk = holdsAsterisk;
  objDir      = dir;

  constructIntersection();
}

string Intersect1::operator[]( int row )
{
  return getIntersection( row );
}

string Intersect1::getIntersection( int row )
{
  // Check for precondition (0 <= row <= 4)
  if( row < 0 || row > 4 )
  {
    return ""; /* return empty string */
  }

  return intersection[row];
}

void Intersect1::constructIntersection()
{
  std::stringstream rowStream[5];

  rowStream[0] << "_|     |_";

  // Calculate rows based on direction
  switch( objDir )
  {
    case NaN:
      rowStream[1] << "         ";

      if( hasAsterisk )
      {
        rowStream[2] << "    " << objId << "*   ";
      }
      else
      {
        rowStream[2] << "    " << objId << "    ";
      }

      rowStream[3] << "         ";
      break;

    case UP:
      rowStream[1] << "    ^    ";

      if( hasAsterisk )
      {
        rowStream[2] << "    *    ";
        rowStream[3] << "    " << objId << "    ";
      }
      else
      {
        rowStream[2] << "    " << objId << "    ";
        rowStream[3] << "         ";
      }
      break;

    case DOWN:
      if( hasAsterisk )
      {
        rowStream[1] << "    " << objId << "    ";
        rowStream[2] << "    *    ";
      }
      else
      {
        rowStream[1] << "         ";
        rowStream[2] << "    " << objId << "    ";
      }

      rowStream[3] << "    v    ";
      break;

    case LEFT:
      rowStream[1] << "         ";

      if( hasAsterisk )
      {
        rowStream[2] << "   <*" << objId << "   ";
      }
      else
      {
        rowStream[2] << "   <" << objId << "    ";
      }

      rowStream[3] << "         ";
      break;

    case RIGHT:
      rowStream[1] << "         ";

      if( hasAsterisk )
      {
        rowStream[2] << "   " << objId << "*>   ";
      }
      else
      {
        rowStream[2] << "    " << objId << ">   ";
      }

      rowStream[3] << "         ";
      break;
  }

  rowStream[4] << UPPER_SCORE << "|     |" << UPPER_SCORE;

  // Assign intersection rows
  for( int i = 0; i < 5; i++ )
  {
    intersection[i] = rowStream[i].str();
  }
}

// Terminating precompiler directives  ////////////////////////////////////////

#endif  // #ifndef CLASS_INTERSECT_CPP
