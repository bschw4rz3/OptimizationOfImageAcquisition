#include "pch.h"

// BSD 3 - Copyright 2023
//
// Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation 
// and/or other materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software 
// without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS” AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS 
// BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
// GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH 
// DAMAGE.
// 
// Parts of this Software are based on Irrlicht Engine, zlib, libpng and IJG-Code

#include "CppUnitTest.h"

#include <math.h>

#include "../PartCover/BananaFacet.h"
#include <wrl/client.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PartCoverTests
{
	TEST_CLASS(BananaFacetTests)
	{
	public:
		
		TEST_METHOD(calculateBoarderCalculatesMiddleWidth)
		{
			// Arrange
			double width = 5;

			vector<double> listY;
			BananaFacet facet(23, width, 10);

			// Act
			double leftY = facet.calculateBoarder(0, true);
			double rightY = facet.calculateBoarder(0, false);

			// Assert
			double result = leftY - rightY;
			Assert::AreEqual(width, result);
		}

		TEST_METHOD(calculateBoarderReturnsInfinityIfXIsInRange)
		{
			// Arrange
			double length = 20;

			vector<double> listY;
			BananaFacet facet(length, 2, 10);

			double invalidLength = (length / 2);

			// Act
			double resultOne = facet.calculateBoarder(invalidLength, true);
			double resultTwo = facet.calculateBoarder(-invalidLength, true);

			// Assert
			Assert::AreEqual(false, isnan(resultOne));
			Assert::AreEqual(false, isnan(resultTwo));
		}

		TEST_METHOD(calculateBoarderReturnsInfinityIfXIsBiggerThanLength)
		{
			// Arrange
			double length = 24;

			vector<double> listY;
			BananaFacet facet(length, 2, 10);

			double invalidLength = (length / 2) + 1;

			// Act
			double result = facet.calculateBoarder(invalidLength, true);

			// Assert
			Assert::AreEqual(true, isnan(result));
		}

		TEST_METHOD(calculateBoarderReturnsInfinityIfXIsSmalerThanLength)
		{
			// Arrange
			double length = 24;

			vector<double> listY;
			BananaFacet facet(length, 2, 10);

			double invalidLength = -((length / 2) + 1);

			// Act
			double result = facet.calculateBoarder(invalidLength, true);

			// Assert
			Assert::AreEqual(true, isnan(result));
		}
	};
}
