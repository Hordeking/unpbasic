/******************************************************************************
  Substantial portions of this code are based on or directly adapted
  from code which is Copyright (c) 2014 Ryan Juckett
  http://www.ryanjuckett.com/

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.

  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.

  3. This notice may not be removed or altered from any source
     distribution.
******************************************************************************/

#include <ostream>
#include <string>
#include "Standard.h"

class float_msft {

	public:

		enum class Precision { f_single = 4, f_double = 8, f_quadruple = 16, f_octuple = 32 };

		enum tPrintFloatFormat
		{
			PrintFloatFormat_Positional,    // [-]ddddd.dddd
			PrintFloatFormat_Scientific,    // [-]d.dddde[sign]ddd
			PrintFloatFormat_Unspecified,	// Let the system decide
		};

		/*
		value,          // value to print
		format,         // format to print with
		precision       // If negative, the minimum number of digits to represent a
                        // unique 32-bit floating point value is output. Otherwise,
                        // this is the number of digits to print past the decimal point.
		*/

		float_msft(tU32 value, tPrintFloatFormat format = PrintFloatFormat_Unspecified, tS32 precision = -1);
		float_msft(tU64 value, tPrintFloatFormat format = PrintFloatFormat_Unspecified, tS32 precision = -1);

		tU32 PrintFloat32_MSFT
		(
			tC8 *               pOutBuffer,     // buffer to output into
			tU32                bufferSize,     // size of pOutBuffer
			tU64                floatMantissa,  /// mantissa to print
			tU32                floatExponent,  /// exponent to print
			tB					floatNegative,	/// sign to print
			tPrintFloatFormat   format,         // format to print with
			tS32                precision       // If negative, the minimum number of digits to represent a
												// unique 32-bit floating point value is output. Otherwise,
												// this is the number of digits to print past the decimal point.
		) const;

		tU32 PrintFloat64_MSFT
		(
			tC8 *               pOutBuffer,     // buffer to output into
			tU32                bufferSize,     // size of pOutBuffer
			tU64                floatMantissa,  /// mantissa to print
			tU32                floatExponent,  /// exponent to print
			tB					floatNegative,	/// sign to print
			tPrintFloatFormat   format,         // format to print with
			tS32                precision       // If negative, the minimum number of digits to represent a
												// unique 32-bit floating point value is output. Otherwise,
												// this is the number of digits to print past the decimal point.
		) const;

		// ostream operator << overloading
		friend std::ostream& operator<<(std::ostream &os, const float_msft &myFloat)
		{
			tC8 buffer[256] = {0};	// Should be way more than enough for a standard MSFTfloat.

			if (myFloat.msft_prec == Precision::f_double) {
				myFloat.PrintFloat64_MSFT(buffer, 256, myFloat.mantissa, myFloat.exponent, myFloat.sign, tPrintFloatFormat::PrintFloatFormat_Unspecified,-1);
			}
			else {
				myFloat.PrintFloat32_MSFT(buffer, 256, myFloat.mantissa, myFloat.exponent, myFloat.sign, tPrintFloatFormat::PrintFloatFormat_Unspecified,-1);
			}

			os << std::string(buffer);
			return os;
		}

	protected:

		tU32 exponent = 0;
		tB sign = false;
		tU64 mantissa = 0;

		Precision msft_prec = Precision::f_single;

};

enum tPrintFloatFormat
{
	PrintFloatFormat_Positional,    // [-]ddddd.dddd
	PrintFloatFormat_Scientific,    // [-]d.dddde[sign]ddd
};
