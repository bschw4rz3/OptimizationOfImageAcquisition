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

#include "HittingSetDescriptionResult.h"


void HittingSetDescriptionResult::add(HittingSetPoint point)
{
	this->points.push_back(point);
}

int HittingSetDescriptionResult::getSize()
{
	return this->points.size();
}

std::vector<std::vector<int>> HittingSetDescriptionResult::toMatrix()
{
	int matrixSize = this->points.size();
	std::vector<std::vector<int>> result = this->initMatrix(matrixSize);

	if(this->indexMap.size() == 0)
	{
		this->indexMap = this->createIndexTable();
	}

	for(int i = 0; i < matrixSize ; i++)
	{
		for (Point neighbor : this->points[i].neighbors)
		{
			int neighborIndex = this->indexMap.at(neighbor);

			result[i][neighborIndex] = 1;
			result[neighborIndex][i] = 1;
		}
	}

	return result;
}

std::vector<std::vector<int>> HittingSetDescriptionResult::initMatrix(int matrixSize)
{
	std::vector<std::vector<int>> result;

	for(int i = 0;i < matrixSize;i++)
	{
		result.push_back(this->initList(matrixSize));
	}

	return result;
}

std::vector<int> HittingSetDescriptionResult::initList(int size)
{
	std::vector<int> result;

	for(int i = 0;i < size;i++)
	{
		result.push_back(0);
	}

	return result;
}

std::map<Point, int> HittingSetDescriptionResult::createIndexTable()
{
	std::map<Point, int> indexMap;

	for(int i = 0; i < this->points.size() ; i++)
	{
		indexMap.insert(std::pair<Point, int>(this->points[i].currentPosition, i));
	}

	return indexMap;
}

Point HittingSetDescriptionResult::fromIndex(int index)
{
	return this->points[index].currentPosition;
}