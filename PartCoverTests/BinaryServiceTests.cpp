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
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS �AS IS� AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS 
// BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
// GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH 
// DAMAGE.
// 
// Parts of this Software are based on Irrlicht Engine, zlib, libpng and IJG-Code

#include "CppUnitTest.h"

#include <math.h>

#include "../PartCover/BinaryService.h"
#include <wrl/client.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PartCoverTests
{
	TEST_CLASS(BinaryServiceTests)
	{
	public:

		TEST_METHOD(raiseCountUpBinaryString1)
		{
			// Arrange
			std::string binary = "00000";
			BinaryService sut;

			// Act
			std::string result = sut.raise(binary);

			// Assert
			Assert::AreEqual(result.c_str(), "00001");
		}

		TEST_METHOD(raiseCountUpBinaryString2)
		{
			// Arrange
			std::string binary = "00001";
			BinaryService sut;

			// Act
			std::string result = sut.raise(binary);

			// Assert
			Assert::AreEqual(result.c_str(), "00010");
		}

		TEST_METHOD(raiseCountUpBinaryString3)
		{
			// Arrange
			std::string binary = "00010";
			BinaryService sut;

			// Act
			std::string result = sut.raise(binary);

			// Assert
			Assert::AreEqual(result.c_str(), "00011");
		}

		TEST_METHOD(raiseCountUpBinaryString4)
		{
			// Arrange
			std::string binary = "00011";
			BinaryService sut;

			// Act
			std::string result = sut.raise(binary);

			// Assert
			Assert::AreEqual(result.c_str(), "00100");
		}

		TEST_METHOD(raiseCountUpBinaryString5)
		{
			// Arrange
			std::string binary = "01111111111111111111";
			BinaryService sut;

			// Act
			std::string result = sut.raise(binary);

			// Assert
			Assert::AreEqual(result.c_str(), "10000000000000000000");
		}

		TEST_METHOD(raiseCountUpBinaryString6)
		{
			// Arrange
			std::string binary = "10111111111011";
			BinaryService sut;

			// Act
			std::string result = sut.raise(binary);

			// Assert
			Assert::AreEqual(result.c_str(), "10111111111100");
		}

		TEST_METHOD(raiseCountUpBinaryString7)
		{
			// Arrange
			std::string binary = "1111111111111111";
			BinaryService sut;

			// Act
			std::string result = sut.raise(binary);

			// Assert
			Assert::AreEqual(result.c_str(), "0000000000000000");
		}
	};
}