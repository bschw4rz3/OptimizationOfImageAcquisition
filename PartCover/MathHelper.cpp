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

#include "MathHelper.h"

unsigned long long MathHelper::factorial(int n)
{
    unsigned long long n1 = n;
    unsigned long long ret = 1;

    while (n1 > 1)
    {
        ret *= n1--;
    }
        
    return ret;
}

unsigned long long int MathHelper::ipow(unsigned long long a, unsigned long long n)
{
    unsigned long long int r = 1;

    while (n > 0) 
    {
        if (n & 1)
            r *= a;
        a *= a;
        n >>= 1;
    }
    return r;
}

bool MathHelper::equals(double a, double b, double epsilon)
{
	return a - epsilon < b && a + epsilon > b;
}

double MathHelper::distance(const Point& one, const Point& two)
{
	return sqrt(pow(one.x - two.x, 2) + pow(one.x - two.x, 2) * 1.0);
}

double MathHelper::getAngle(const Point& one, const Point& two, const Point& three)
{
    double dings = ((one.x - two.x) * (three.x - two.x)) + ((one.y - two.y) * (three.y - two.y));
    double bums = sqrt(pow(one.x - two.x, 2) + pow(one.y - two.y, 2)) * sqrt(pow(three.x - two.x, 2) + pow(three.y - two.y, 2));
    
	return this->toDegree(acos(this->roundTo2Deciamls(dings/bums)));
}

double MathHelper::toDegree(double rad)
{
	return rad * 180/this->getPi();
}

double MathHelper::getPi()
{
    return 3.14159265;
}

double MathHelper::roundTo2Deciamls(double value)
{
	return round(value * 100.0) / 100.0;
}