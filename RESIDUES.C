//------------------------------------------------------------------------------

// RESIDUES.C -- written by Davin M. Potts using Turbo C++ 2.0
//            -- object-oriented code for residue numerical systems
//            -- begun 19 May 1992
//	      -- version 1.0, 13 Nov 1993
//            -- current version 1.1, 18 May 1994

//------------------------------------------------------------------------------


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>

// RESINTi is the macro definition putting an upper limit on
//    the number of radices.  Making it larger allows for
//    more radices, but it also requires more memory for each
//    residue_int class variable.
#define RESINTi 25



//------------------------------------------------------------------------------

// Class definition

class residue_int
{

private:
   unsigned long int value[RESINTi], invradix[RESINTi-1][RESINTi];

// Private constructor
   residue_int::residue_int( int num_rad, unsigned long* radp,
			     unsigned long* invradp );

public:
   unsigned long int radix[RESINTi];
   int number_of_radices;


// Constructors
   residue_int::residue_int( void );
   residue_int::residue_int( int num_rad, unsigned long r0 ... );
   residue_int::residue_int( int num_rad, unsigned long* radp );

// Conversion functions
   void itoresidue( unsigned long x );
   unsigned long int residuetoi( void );
   long double residuetof( void );
   char* residuetoc( int max_length );
   residue_int residuetoresidue( int num_rad, unsigned long newr0 ... );

// Overloaded operators
   residue_int residue_int::operator+ (residue_int &Arg);
   residue_int residue_int::operator- (residue_int &Arg);
   residue_int residue_int::operator* (residue_int &Arg);
   residue_int residue_int::operator+ (unsigned long n);
   residue_int residue_int::operator- (unsigned long n);
   residue_int residue_int::operator* (unsigned long n);
   friend residue_int operator+ (unsigned long n, residue_int &part_residue )
                                { return( part_residue + n ); }
   friend residue_int operator- (unsigned long n, residue_int &part_residue ) 
      {
	 residue_int temp( part_residue.number_of_radices,
			   (unsigned long*)part_residue.radix,
			   (unsigned long*)part_residue.invradix );
         temp.itoresidue( 0L );
         temp = temp - part_residue;
         return( n + temp );
      }
   friend residue_int operator* (unsigned long n, residue_int &part_residue )
                                { return( part_residue * n ); }

// Other Math functions
   void inverse( void );

};



//------------------------------------------------------------------------------

// Constructors


// Constructor with values passed for radices:
residue_int::residue_int( int num_rad, unsigned long r0 ... )
{
   short int mult_inv_of, with_respect_to, i;
   signed long int a, b, u, v, temp;
   va_list ap;

   // Initialize the struct va_list variable for extracting arguments.
   va_start( ap, r0 );

   // The integer array 'radix' contains the values of the radices
   //    (also called moduli).  Extract the radix values using the
   //    macro 'va_arg'.
   for( i = 1; i < num_rad; i++ )
      radix[i] = va_arg( ap, unsigned long );

   // The value of the first radix is from r0.
   radix[0] = r0;

   // Undo any modification to the stack done by 'va_start'.
   va_end( ap );

   // The number of radices used in this class will be equal to 'num_rad'.
   number_of_radices = num_rad;

   // Calculate the values for the array 'invradix' which contains
   //    the multiplicative inverses of each radix with respect to
   //    each larger radix:  (radix[a]*invradix[a][b])%radix[b]=1
   //    These values are needed in residuetoi, residuetof, and
   //    residuetoresidue.  The calculation uses the Euclidean 
   //    Algorithm to calculate the mult. inv.s.

   for( mult_inv_of = 0; mult_inv_of < (number_of_radices-1);
        mult_inv_of++ )
      for( with_respect_to = mult_inv_of+1;
           with_respect_to < number_of_radices; with_respect_to++ )
      {
         for( a = radix[mult_inv_of], b = radix[with_respect_to], 
              u = 0, v = 1; a != (long)0L; )
         {
            temp = v;
            v = u - (signed long)floor(b/a) * v;
            u = temp;

            temp = b;
            b = a;
            a = temp % a;
         }
	 if( u < 0 ) u = u + radix[with_respect_to];
         invradix[mult_inv_of][with_respect_to] = u;
      }
}

// Constructor with values passed for radices within an array:
residue_int::residue_int( int num_rad, unsigned long* radp )
{
   short int mult_inv_of, with_respect_to, i;
   signed long int a, b, u, v, temp;

   // The integer array 'radix' contains the values of the radices
   //    (also called moduli).  Extract the radix values from the array.
   for( i = 1; i < num_rad; i++ )
      radix[i] = *(radp + i);

   // The number of radices used in this class will be equal to 'num_rad'.
   number_of_radices = num_rad;

   // Calculate the values for the array 'invradix' which contains
   //    the multiplicative inverses of each radix with respect to
   //    each larger radix:  (radix[a]*invradix[a][b])%radix[b]=1
   //    These values are needed in residuetoi, residuetof, and
   //    residuetoresidue.  The calculation uses the Euclidean 
   //    Algorithm to calculate the mult. inv.s.

   for( mult_inv_of = 0; mult_inv_of < (number_of_radices-1);
        mult_inv_of++ )
      for( with_respect_to = mult_inv_of+1;
           with_respect_to < number_of_radices; with_respect_to++ )
      {
         for( a = radix[mult_inv_of], b = radix[with_respect_to], 
              u = 0, v = 1; a != (long)0L; )
         {
            temp = v;
            v = u - (signed long)floor(b/a) * v;
            u = temp;

            temp = b;
            b = a;
            a = temp % a;
         }
	 if( u < 0 ) u = u + radix[with_respect_to];
         invradix[mult_inv_of][with_respect_to] = u;
      }
}

// Constructor with values passed for both radices and inverseradices:
residue_int::residue_int( int num_rad, unsigned long* radp,
			  unsigned long* invradp )
{
   int i, j;

   if( (num_rad > RESINTi) || (num_rad < 1) )exit( -2 );

   // The number of radices used in this class will be equal to num_rad.
   number_of_radices = num_rad;

   // The array 'radix' contains the radices.
   for( i = 0; i < num_rad; i++ )radix[i] = *(radp + i);

   // The array 'invradix' contains the mult. inv. values.
   for( i = 0; i < num_rad; i++ )
      for( j = 0; j < (num_rad-1); j++ )
	 invradix[j][i] = *(invradp + j*RESINTi + i);
}
 
// Constructor with default values (no values passed for radices):
residue_int::residue_int( void )
{
   // The number of radices used in this class will be 5.
   number_of_radices = 5;

   // The array 'radix' contains the radices.
   radix[0] = (long)65449L;
   radix[1] = (long)65479L;
   radix[2] = (long)65497L;        // These are the greatest primes
   radix[3] = (long)65519L;        // less than 2^16.  Product is
   radix[4] = (long)65521L;        // 1204964463846332731259513.

   invradix[0][1] = (long)41470L;  // The array 'invradix' contains the
   invradix[0][2] = (long)34113L;  // multiplicative inverses of each
   invradix[0][3] = (long)64583L;  // radix with respect to each 
   invradix[0][4] = (long)910L;    // larger radix:
   invradix[1][2] = (long)25471L;  // radix[a]*invradix[a][b]%radix[b]=1
   invradix[1][3] = (long)63881L;  // These values are needed in 
   invradix[1][4] = (long)1560L;   // residuetoi, residuetof, and 
   invradix[2][3] = (long)44672L;  // residuetoresidue.
   invradix[2][4] = (long)2730L;   
   invradix[3][4] = (long)32760L;            
}



//------------------------------------------------------------------------------

// Overloaded operators:


// operator+ -- Adds two residue_int's by overloading the addition operator.  
residue_int residue_int::operator+ (residue_int &Arg)
{
   residue_int Temp( number_of_radices, (unsigned long*)radix,
		     (unsigned long*)invradix );
   short int index;

   // Check to make certain the two residue_ints being added have
   //    the same number of radices.
   if( number_of_radices != Arg.number_of_radices )exit( -1 );

   for( index = 0; index < number_of_radices; index++ )
   {

      // We will check to make sure that the two residue_ints being
      //    added have the same radices at the same time that we add
      //    them.  Check then add.  If any radix differs between the
      //    first (and current) and second (which is being passed to 
      //    this fn) arguments, error results.

      if( radix[index] != Arg.radix[index] )exit( -1 );

      // Add and store the new values in a temporary residue_int.  Then 
      //    check to make certain the new values are still in range.  If 
      //    not, find the residue values modulo the radix (i.e. bring it 
      //    back to its residue equivalent).

      Temp.value[index] = value[index] + Arg.value[index];
      if( Temp.value[index] >= radix[index] )
	 Temp.value[index] -= Temp.radix[index];
   }

   return( Temp );  // Return the temporary residue_int.
}


// operator- -- Subtracts two residue_int's (overloaded operator).
residue_int residue_int::operator- (residue_int &Arg)
{
   residue_int Temp( number_of_radices, (unsigned long*)radix,
                     (unsigned long*)invradix );
   short int index;

   // Check to make certain the two residue_ints being added have
   //    the same number of radices.
   if( number_of_radices != Arg.number_of_radices )exit( -1 );

   for( index = 0; index < number_of_radices; index++ )
   {

      // Subtraction works the same way as overloaded addition.

      if( radix[index] != Arg.radix[index] )exit( -1 );

      // Subtract.  Then check to make certain still in range.

      Temp.value[index] = value[index] - Arg.value[index];
      if( Temp.value[index] > value[index] )
	 Temp.value[index] += Temp.radix[index];
   }

   return( Temp );  // Return the temporary residue_int.
}


// operator* -- Multiplies two residue_int's (overloaded operator).
residue_int residue_int::operator* (residue_int &Arg)
{
   residue_int Temp( number_of_radices, (unsigned long*)radix,
                     (unsigned long*)invradix );
   short int index;

   // Check to make certain the two residue_ints being added have
   //    the same number of radices.
   if( number_of_radices != Arg.number_of_radices )exit( -1 );

   for( index = 0; index < number_of_radices; index++ )
   {
      // Multiplication works the same way as overloaded addition.

      if( radix[index] != Arg.radix[index] )exit( -1 );

      // Multiply.  Then check to make certain still in range.

      Temp.value[index] = (value[index] * Arg.value[index]) % radix[index];
   }

   return( Temp );  // Return the temporary residue_int.
}

// operator+ -- Adds a residue_int and an unsigned long 
//              (overloaded operator).
residue_int residue_int::operator+ (unsigned long n)
{
   residue_int part_residue = *this;
   residue_int part_integer( number_of_radices, (unsigned long*)radix,
                             (unsigned long*)invradix );

   part_integer.itoresidue( n );
 
   return( part_residue + part_integer );
}

// operator- -- Subtracts an unsigned long from a residue_int
//              (overloaded operator).
residue_int residue_int::operator- (unsigned long n)
{
   residue_int part_residue = *this;
   residue_int part_integer( number_of_radices, (unsigned long*)radix,
                             (unsigned long*)invradix );

   part_integer.itoresidue( n );
 
   return( part_residue - part_integer );
}

// operator* -- Multiplies a residue_int by an unsigned long
//              (overloaded operator).
residue_int residue_int::operator* (unsigned long n)
{
   residue_int part_residue = *this;
   residue_int part_integer( number_of_radices, (unsigned long*)radix,
                             (unsigned long*)invradix );

   part_integer.itoresidue( n );
 
   return( part_residue * part_integer );
}

//------------------------------------------------------------------------------

// Other Math functions:


// inverse -- Calculates the mult. inv. of a residue_int.
void residue_int::inverse( void )
{
   signed long a, b, u, v, temp;
   short int index;

   for( index = 0; index < number_of_radices; index++ )
   {
      for( a = value[index], b = radix[index], u = 0, v = 1; a != (long)0L; )
      {
         temp = v;
         v = u - (signed long)floor(b/a) * v;
         u = temp;

         temp = b;
         b = a;
         a = temp % a;
      }

      if( u < 0 )u += radix[index];
      value[index] = u;

      if( u == 0 )exit( -1 ); // There is not a mult. inv. for this case.

      // Note that there can be no mult. inv. for any number that is not
      // relatively prime to all radices.

   }
   return;
}

//------------------------------------------------------------------------------

// Conversion functions:


// itoresidue -- Stores an integer value in a residue_int.
void residue_int::itoresidue( unsigned long x )
{
   int index;

   // Calculate the residue form of the integer by finding its modulo
   //   by each of the radices.
   for( index = 0; index < number_of_radices; index++ )
      value[index] = x % radix[index];

   return;
}


// residuetoi -- Returns the integer value stored in a residue_int.
unsigned long int residue_int::residuetoi( void )
{
   unsigned long int Normal_Int_Value;
   long int v[RESINTi];
   int x, n;

   // Calculate the v[number_of_radices] values as a first step in
   //    calculating the integer value.  This algorithm is set up to
   //    accomodate any number of radices (for versatility) according to
   //    Knuth's formulas for the Chinese Remainder Theorem "in reverse".
   for( x = 0; x < number_of_radices; x++ )
   {
      for( v[x] = value[x], n = 0; n < x; n++ )
      {
         if( v[x] >= v[n] )
	 {
            v[x] = ((v[x] - v[n]) * invradix[n][x]) % radix[x];
         }
         else
         {
	    v[x] = radix[x] - ( ((v[n] - v[x]) * invradix[n][x]) % radix[x] );
	 }
      }
   }	   
	   
   // Initialize Normal_Int_Value.
   Normal_Int_Value = (unsigned long int)0;

   // Using Knuth's formula, calculate the integer value from the
   //    values of the integer array 'v' that we just calculated.
   for( x = (number_of_radices-1); x > 0; x-- )
      Normal_Int_Value = (Normal_Int_Value + v[x]) * radix[x-1];
   Normal_Int_Value = Normal_Int_Value + v[0];

   // Return the integer.
   return(Normal_Int_Value);

}

// residuetof -- Returns the long double float value stored in a residue_int.
long double residue_int::residuetof( void )
{
   long double Normal_Int_Value;
   long int v[RESINTi];
   int x, n;

   // Calculate the v[number_of_radices] values as a first step in
   //    calculating the integer value.  This algorithm is set up to
   //    accomodate any number of radices (for versatility) according
   //    to Knuth's formulas.
   for( x = 0; x < number_of_radices; x++ )
   {
      for( v[x] = value[x], n = 0; n < x; n++ )
      {
         if( v[x] >= v[n] )
	 {
            v[x] = ((v[x] - v[n]) * invradix[n][x]) % radix[x];
         }
         else
         {
	    v[x] = radix[x] - ( ((v[n] - v[x]) * invradix[n][x]) % radix[x] );
	 }
      }
   }	   
	   
   // Initialize Normal_Int_Value.
   Normal_Int_Value = (long double)0;

   // Using Knuth's formula, calculate the integer value from the
   //    values of the integer array 'v' that we just calculated.
   for( x = (number_of_radices-1); x > 0; x-- )
      Normal_Int_Value = (Normal_Int_Value + v[x]) * radix[x-1];
   Normal_Int_Value = Normal_Int_Value + v[0];

   // Return the real.
   return(Normal_Int_Value);

}

// residuetoc -- Returns a string of chars representing the integer
//                  value stored in a residue_int.
char* residue_int::residuetoc( int max_length )
{
   static char* Normal_Char_Value;
   int* Normal_Int2_Value;
   int* Normal_Int_Value;
   int* big_v;
   int* big_radix;
   long int v[RESINTi], temp_v, temp_radix, decimal;
   int x, n, i, j, max_magnitude, carry;
   ldiv_t llx;
   div_t lx;

   // Proceed just as before for the first part.
   for( x = 0; x < number_of_radices; x++ )
   {
      for( v[x] = value[x], n = 0; n < x; n++ )
      {
         if( v[x] >= v[n] )
	 {
            v[x] = ((v[x] - v[n]) * invradix[n][x]) % radix[x];
         }
         else
         {
	    v[x] = radix[x] - ( ((v[n] - v[x]) * invradix[n][x]) % radix[x] );
	 }
      }
   }	   
	   
   // Allocate memory for the strings.
   if( !( Normal_Char_Value = new char[max_length] ) )exit( -3 );
   if( !( Normal_Int_Value = new int[max_length] ) )exit( -3 );
   if( !( Normal_Int2_Value = new int[max_length] ) )exit( -3 );
   if( !( big_v = new int[max_length] ) )exit( -3 );
   if( !( big_radix = new int[max_length] ) )exit( -3 );

   // Initialize the strings.
   for( i = 0; i < max_length; i++ )
   {
      *(Normal_Char_Value + i) = 0;
      *(Normal_Int_Value + i) = 0;
      *(Normal_Int2_Value + i) = 0;
      *(big_v + i) = 0;
      *(big_radix + i) = 0;
   }


   // Find the order of magnitude of the largest radix.
   for( i = 0; (long int)pow( 10, i ) <= radix[number_of_radices-1]; i++ );

   max_magnitude = i;

   // Using Knuth's formula, calculate the integer value from the
   //    values of the integer array 'v' that we just calculated.
   for( x = (number_of_radices-1); x > 0; x-- )
   {
      // Put v[x] and radix[x-1] into the strings big_v and big_radix.
      for( i = (max_magnitude-1), temp_v = v[x], temp_radix = radix[x-1];
	   i > 0; i-- )
      {
	 decimal = (long int)pow( 10, i );

	 llx = ldiv( temp_v, decimal );
	 *(big_v + i) = (int)llx.quot;
	 temp_v = llx.rem;

	 llx = ldiv( temp_radix, decimal );
	 *(big_radix + i) = (int)llx.quot;
	 temp_radix = llx.rem;
      }
      *(big_v + 0) = (int)temp_v;
      *(big_radix + 0) = (int)temp_radix;

      // Clear the rest of the strings.
      for( i = max_magnitude; i < max_length; i++ )
      {
	 *(big_v + i) = 0;
	 *(big_radix + i) = 0;
      }

      // Clear the string.
      for( i = 0; i < max_length; i++ )
      {
	 *(Normal_Int_Value + i) = 0;
      }

      // Perform the addition part here.
      for( i = 0; i < max_length; i++ )
      {
	 *(Normal_Int_Value+i) += (*(Normal_Int2_Value+i)) + (*(big_v + i));
	 if( *(Normal_Int_Value + i) > 9 )
	 {
	    *(Normal_Int_Value + i) -= 10;
	    *(Normal_Int_Value + i + 1) += 1;
	 }
      }

      // Clear the string.
      for( i = 0; i < max_length; i++ )
      {
	 *(Normal_Int2_Value + i) = 0;
      }

      // Perform the multiplication part here.
      for( i = 0, carry = 0; i < max_length; i++ )
      {
	 for( j = 0; j <= i; j++ )
	    *(Normal_Int2_Value+i) += (*(Normal_Int_Value+j)) *
				     (*(big_radix+i-j));

	 *(Normal_Int2_Value+i) += carry;

	 lx = div( *(Normal_Int2_Value + i), 10 );
	 *(Normal_Int2_Value + i) = lx.rem;
	 carry = lx.quot;
      }
   }

   // Now add in the v[0] part.
   for( i = (max_magnitude-1), temp_v = v[0]; i > 0; i-- )
   {
      decimal = (long int)pow( 10, i );

      llx = ldiv( temp_v, decimal );
      *(big_v + i) = (int)llx.quot;
      temp_v = llx.rem;
   }
   *(big_v + 0) = (int)temp_v;

   // Clear the rest of the string.
   for( i = max_magnitude; i < max_length; i++ )
      *(big_v + i) = 0;

   for( i = 0; i < max_length; i++ )
   {
      *(Normal_Int2_Value + i) += *(big_v + i);
      if( *(Normal_Int2_Value + i) > 9 )
      {
	 *(Normal_Int2_Value + i) -= 10;
	 *(Normal_Int2_Value + i + 1) += 1;
      }
   }

   // Make the int values -> char values corresponding to
   //    chars that look like digits 0 through 9.
   for( i = 0; i < max_length; i++ )
      *(Normal_Char_Value + i) = *(Normal_Int2_Value+max_length-1-i) + 48;

   // Add null character to end of string.
   *(Normal_Char_Value+max_length) = '\0';

   // Return the pointer to the string representing the integer.
   return(Normal_Char_Value);

}

// residuetoresidue -- Converts a residue_int from one set of radices
//                     to another set of radices.  Returns this new
//                     residue_int with its new radices.
residue_int residue_int::residuetoresidue( int num_rad,
					   unsigned long newr0 ... )
{
   long int v[RESINTi];
   unsigned long newr[RESINTi];
   int x, n, i;

   va_list ap;

   va_start( ap, newr0 );
   for( i = 1; i < num_rad; i++ )
      newr[i] = va_arg( ap, unsigned long );
   newr[0] = newr0;
   va_end( ap );

   residue_int New_Residue( num_rad, (unsigned long*)newr );
   residue_int Convert( num_rad, (unsigned long*)newr );

   // Calculate the v[number_of_radices] values as a first step to calculate
   //    the integer value.  This algorithm is set up to accomodate 
   //    any number of radices (for versatility) according to Knuth's 
   //    formulas.  (Same as in residuetoi so far.)
   for( x = 0; x < number_of_radices; x++ )
   {
      for( v[x] = value[x], n = 0; n < x; n++ )
      {
         if( v[x] >= v[n] )
	 {
            v[x] = ((v[x] - v[n]) * invradix[n][x]) % radix[x];
         }
         else
         {
	    v[x] = radix[x] - ( ((v[n] - v[x]) * invradix[n][x]) % radix[x] );
	 }
      }
   }
	   
   // Initialize the residue_int to be returned.
   New_Residue.itoresidue( 0 );

   // Use the same algorithm as in residuetoi, except here a residue_int
   //    is being calculated instead of an integer.
   for( x = (num_rad-1); x > 0; x-- )
   {
      Convert.itoresidue( v[x] );      // Convert is used to change integer
				       // values into residue_int's for the
				       // residue_int arithmetic.
      New_Residue = New_Residue + Convert;
      Convert.itoresidue( radix[x-1] );
      New_Residue = New_Residue * Convert;
   }
   Convert.itoresidue( v[0] );
   New_Residue = New_Residue + Convert;

   // Return the residue_int with its new radices.  The integer values of
   //    the residue_ints with the old and new radices are the same.
   return(New_Residue);

}


/*
   10JUN93
   Next steps:

   X  1.  Make number of radices variable.
      2.  Sign determination / versatility.
   X  3.  Cache invradix.
   X  4.  Division algorithm.
*/

