#pragma once

#include <array>
#include <chrono>
#include <random>

namespace MatrixUtils {
	
	template<typename T, int numDimensions>
	std::array<T, numDimensions * numDimensions> createHouseholderMatrix(const std::array<T, numDimensions>& unitVector) {
		std::array<T, numDimensions * numDimensions> P;
		for (int i = 0; i < numDimensions; ++i) {
			for (int j = 0; j < numDimensions; ++j) {
				float identity = (i == j) ? 1.f : 0.f;
				P[i * numDimensions + j] = identity - 2 * unitVector[i] * unitVector[j];
			}
		}
		return P;
	}

	template<typename T, int numDimensions>
	std::array<T, numDimensions * numDimensions> createRandomHouseholderMatrix() {
		auto seed = static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count());
		std::mt19937 rng(seed);
		std::normal_distribution<float> distribution;

		std::array<T, numDimensions> unitVector;
		float normSquared{ 0.f };
		for (int i = 0; i < numDimensions; ++i) {
			float value = distribution(rng);
			normSquared += value * value;
			unitVector[i] = value;
		}
		float invNorm = 1.f / std::sqrt(normSquared);
		for (int i = 0; i < numDimensions; ++i) {
			unitVector[i] *= invNorm;
		}

		return createHouseholderMatrix<T, numDimensions>(unitVector);
	}
}

// Use like `Hadamard<double, 8>::inPlace(data)` - size must be a power of 2
template<typename Sample, int size>
class Hadamard {
public: 
	static inline void recursiveUnscaled(Sample * data) {
		if (size <= 1) return;
		constexpr int hSize = size/2;

		// Two (unscaled) Hadamards of half the size
		Hadamard<Sample, hSize>::recursiveUnscaled(data);
		Hadamard<Sample, hSize>::recursiveUnscaled(data + hSize);

		// Combine the two halves using sum/difference
		for (int i = 0; i < hSize; ++i) {
			double a = data[i];
			double b = data[i + hSize];
			data[i] = (a + b);
			data[i + hSize] = (a - b);
		}
	}

	static inline void inPlace(Sample * data) {
		recursiveUnscaled(data);
		
		Sample scalingFactor = std::sqrt(1.0/size);
		for (int c = 0; c < size; ++c) {
			data[c] *= scalingFactor;
		}
	}
};