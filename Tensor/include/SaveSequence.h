#pragma once
#include "Tensor.h"

namespace TSlib
{
	namespace Tools
	{
		template<typename T>
		class otnsr_sequence
		{
				std::ofstream out_file;
				std::vector<size_t> shape;
			
			public;

				tnsr_sequence(std::string dir, std::vector<size_t> storage_shape);

				template<Mode device>
				tnsr_sequence(std::string dir, const Tensor<T, device>& base);

				begin_sequence();

				template<Mode device>
				append(const Tensor<T, device>& source);

		};

		template<typename T>
		class itnsr_sequence
		{
			std::ofstream in_file;
			std::vector<size_t> shape;
			size_t length;

			public;

			tnsr_sequence(std::string dir, std::vector<size_t> storage_shape);

			template<Mode device>
			tnsr_sequence(std::string dir, const Tensor<T, device>& base);

			template<Mode device>
			void read(Tensor<T, device>& source);

			template<Mode device = default_device>
			Tensor<T, device> read();

		};
	}
}

#include "SaveSequence.ipp"
