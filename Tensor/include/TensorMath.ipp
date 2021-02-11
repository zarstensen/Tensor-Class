#pragma once
#include "Tensor.h"
#include "TensorTools.h"

namespace TSlib
{
	namespace Consts
	{
		static constexpr double Euler = 2.71828182845904523536;
		static constexpr double PI = 3.14159265358979323846;
	}

	// takes the sum of the elements
	// elem += current_elem

	template<typename T, Device device>
	template<typename TReturn>
	TReturn TensorSlice<T, device>::sum() const
	{
		TReturn sum_val = 0;
		for (size_t i = 0; i < size(); i++)
		{
			sum_val += At(i);
		}

		return sum_val;
	}

	template<typename T, Device device>
	template<typename TReturn>
	Tensor<TReturn, device> TensorSlice<T, device>::sum(size_t axis, bool keepDims) const
	{
		return Compute([&](T& sum_elem, const T& elem) {sum_elem += elem; }, axis, 0, keepDims);
	}

	template<typename T, Device device>
	template<typename TReturn>
	TReturn Tensor<T, device>::sum() const
	{
		TReturn sum_val = 0;
		for (size_t i = 0; i < size(); i++)
		{
			sum_val += At(i);
		}

		return sum_val;
	}

	template<typename T, Device device>
	template<typename TReturn>
	Tensor<TReturn, device> Tensor<T, device>::sum(size_t axis, bool keepDims) const
	{
		return Compute([&](T& sum_elem, const T& elem) {sum_elem += elem; }, axis, 0, keepDims);
	}

	template<typename T, Tools::enable_if_tensor<T>>
	typename T::Type Tools::sum(const T& source)
	{
		return source.sum();
	}

	template<typename T, Tools::enable_if_tensor<T>>
	T Tools::sum(const T& source, size_t axis, bool keep_dims)
	{
		return source.sum(axis, keep_dims);
	}

	// takes the product of the elements
	// elem *= current_elem

	template<typename T, Device device>
	template<typename TReturn>
	TReturn TensorSlice<T, device>::prod() const
	{
		TReturn prod_val = 1;
		for (size_t i = 0; i < size(); i++)
		{
			prod_val *= At(i);
		}

		return prod_val;
	}

	template<typename T, Device device>
	template<typename TReturn>
	Tensor<TReturn, device> TensorSlice<T, device>::prod(size_t axis, bool keepDims) const
	{
		return Compute([&](T& prod_elem, const T& elem) {prod_elem *= elem; }, axis, 1, keepDims);
	}

	template<typename T, Device device>
	template<typename TReturn>
	TReturn Tensor<T, device>::prod() const
	{
		TReturn prod_val = 1;
		for (size_t i = 0; i < size(); i++)
		{
			prod_val *= At(i);
		}

		return prod_val;
	}

	template<typename T, Device device>
	template<typename TReturn>
	Tensor<TReturn, device> Tensor<T, device>::prod(size_t axis, bool keepDims) const
	{
		return Compute([&](T& prod_elem, const T& elem) {prod_elem *= elem; }, axis, 1, keepDims);
	}

	template<typename T, Tools::enable_if_tensor<T>>
	typename T::Type Tools::prod(const T& source)
	{
		return source.prod();
	}

	template<typename T, Tools::enable_if_tensor<T>>
	T Tools::prod(const T& source, size_t axis, bool keep_dims)
	{
		return source.prod(axis, keep_dims);
	}

	// takes every element of the tensor and sets Euler's number to a power of that element
	// elem = e^elem
	template<typename T, Device device>
	Tensor<T, device>& Tensor<T, device>::exp()
	{
		Compute([](T& elem) {
			elem = T(std::pow(Consts::Euler, double(elem)));
			});

		return *this;
	}

	template<typename T, Device device>
	TensorSlice<T, device>& TensorSlice<T, device>::exp()
	{
		Compute([](T& elem) {elem = std::pow(T(Consts::Euler), double(elem)); });

		return *this;
	}

	template<typename T, Tools::enable_if_tensor<T>>
	T Tools::exp(const T& source)
	{
		T result = source;

		result.exp();

		return result;
	}

	template<typename T, Tools::enable_if_tensor<T>>
	T Tools::exp(const T& source, size_t axis, bool keepDims)
	{
		std::vector<size_t> return_shape(source.Shape());

		return_shape[axis] = 1;

		T result(return_shape, 0);

		result.Compute([&](typename T::Type& elem, const std::vector<size_t>& coords)
			{
				std::vector<size_t> new_coords = coords;
				new_coords[axis] = 0;

				double new_elem = 0;
				for (size_t i = 0; i < source.Shape()[axis]; i++)
				{
					new_elem += std::pow(Consts::Euler, double(source.Get(new_coords)));
					new_coords[axis]++;
				}
				elem = typename T::Type(new_elem);
			});

		if (!keepDims)
		{
			return_shape.resize(result.Dims() - 1);

			for (size_t i = 0; i < return_shape.size(); i++)
			{
				return_shape[i] = source.Shape()[i] * (i < axis - 1) + source.Shape()[i + 1] * (i >= axis - 1);
			}

			result.Reshape(return_shape);
		}

		return result;
	}

	// takes the sum of the tensor and divides each element with it.
	// elem = elem/sum(tensor)

	template<typename T, Device device>
	Tensor<T, device>& Tensor<T, device>::normalize()
	{
		T tensor_sum = sum();
		Compute([=](T& elem) {elem = elem / tensor_sum; });

		return *this;
	}

	template<typename T, Device device>
	TensorSlice<T, device>& TensorSlice<T, device>::normalize()
	{
		T tensor_sum = sum();
		Compute([=](T& elem) {elem = elem / tensor_sum; });

		return *this;
	}

	template<typename T, Tools::enable_if_tensor<T>>
	T Tools::normalize(const T& source)
	{
		T result = source;
		result.normalize();

		return result;
	}

	// takes the max element of the tensor
	// largest elem = max(largest elem, current elem)

	template<typename T, Device device>
	T Tensor<T, device>::max() const
	{
		T max_elem = At(0);

		for (size_t i = 1; i < size(); i++)
		{
			max_elem = std::max(max_elem, At(i));
		}

		return max_elem;
	}

	template<typename T, Device device>
	T TensorSlice<T, device>::max() const
	{
		T max_elem = At(0);

		for (size_t i = 1; i < size(); i++)
		{
			max_elem = std::max(max_elem, At(i));
		}

		return max_elem;
	}

	template<typename T, Tools::enable_if_tensor<T>>
	T Tools::max(const T& source, size_t axis, bool keepDims)
	{
		T result = source.Compute([](typename T::Type& new_elem, const typename T::Type& elem) {new_elem = std::max(new_elem, elem); }, axis, keepDims);
		return result;
	}

	// takes the min element of the tensor
	// largest elem = mix(largest elem, current elem)

	template<typename T, Device device>
	T Tensor<T, device>::min() const
	{
		T min_elem = At(0);

		for (size_t i = 1; i < size(); i++)
		{
			min_elem = std::min(min_elem, At(i));
		}

		return min_elem;
	}

	template<typename T, Device device>
	T TensorSlice<T, device>::min() const
	{
		T min_elem = At(0);

		for (size_t i = 1; i < size(); i++)
		{
			min_elem = std::min(min_elem, At(i));
		}

		return min_elem;
	}

	template<typename T, Tools::enable_if_tensor<T>>
	T Tools::min(const T& source, size_t axis, bool keepDims)
	{
		T result = source.Compute([](typename T::Type& new_elem, const typename T::Type& elem) {new_elem = std::min(new_elem, elem); }, axis, keepDims);
		return result;
	}

	// sums the elements and divides them by the number of elements, thus taking the average value of the tensor
	// sum_elem += elem
	// avg = sum_elem / elem_count

	template<typename T, Device device>
	template<typename RT>
	RT TensorSlice<T, device>::avg() const
	{
		return sum<RT>() / (RT)size();
	}

	template<typename T, Device device>
	template<typename RT>
	RT Tensor<T, device>::avg() const
	{
		return sum<RT>() / (RT)size();
	}
	
	template<typename T, Tools::enable_if_tensor<T>>
	T Tools::avg(const T& source, size_t axis, bool keepDims)
	{
		T result = source.Compute([](typename T::Type& new_elem, const typename T::Type& elem) {new_elem += elem; }, axis, keepDims);
		result.Compute([axis, &source](typename T::Type& elem) {elem /= (T)source.Shape()[axis]; });
		return result;

	}

	// takes the sine value of the element
	// elem = sin(elem)

	template<typename T, Device device>
	inline Tensor<T, device>& Tensor<T, device>::sin()
	{
		Compute([](T& elem) {elem = std::sin(elem); });

		return *this;
	}

	template<typename T, Device device>
	inline TensorSlice<T, device>& TensorSlice<T, device>::sin()
	{
		Compute([](T& elem) {elem = std::sin(elem); });
		return *this;
	}

	template<typename T, Tools::enable_if_tensor<T>>
	T Tools::sin(const T& source)
	{
		T result = source;
		result.sin();
		return result;
	}

	// takes the cosine value of the element
	// elem = cos(elem)

	template<typename T, Device device>
	inline Tensor<T, device>& Tensor<T, device>::cos()
	{
		Compute([](T& elem) {elem = std::cos(elem); });

		return *this;
	}

	template<typename T, Device device>
	inline TensorSlice<T, device>& TensorSlice<T, device>::cos()
	{
		Compute([](T& elem) {elem = std::cos(elem); });
		return *this;
	}

	template<typename T, Tools::enable_if_tensor<T>>
	T Tools::cos(const T& source)
	{
		T result = source;
		result.cos();
		return result;
	}

	// takes the tangent value of the element
	// elem = tan(elem)

	template<typename T, Device device>
	inline Tensor<T, device>& Tensor<T, device>::tan()
	{
		Compute([](T& elem) {elem = std::tan(elem); });

		return *this;
	}

	template<typename T, Device device>
	inline TensorSlice<T, device>& TensorSlice<T, device>::tan()
	{
		Compute([](T& elem) {elem = std::tan(elem); });
		return *this;
	}

	template<typename T, Tools::enable_if_tensor<T>>
	T Tools::tan(const T& source)
	{
		T result = source;
		result.tan();
		return result;
	}

	// takes the arc sine / inverse sine value of the element
	// elem = arcsin(elem)

	template<typename T, Device device>
	inline Tensor<T, device>& Tensor<T, device>::arcsin()
	{
		Compute([](T& elem) {elem = std::asin(elem); });

		return *this;
	}

	template<typename T, Device device>
	inline TensorSlice<T, device>& TensorSlice<T, device>::arcsin()
	{
		Compute([](T& elem) {elem = std::asin(elem); });
		return *this;
	}

	template<typename T, Tools::enable_if_tensor<T>>
	T Tools::arcsin(const T& source)
	{
		T result = source;
		result.arcsin();
		return result;
	}

	// takes the arc cosine / inverse cosine value of the element
	// elem = cos(elem)

	template<typename T, Device device>
	inline Tensor<T, device>& Tensor<T, device>::arccos()
	{
		Compute([](T& elem) {elem = std::acos(elem); });

		return *this;
	}

	template<typename T, Device device>
	inline TensorSlice<T, device>& TensorSlice<T, device>::arccos()
	{
		Compute([](T& elem) {elem = std::acos(elem); });
		return *this;
	}

	template<typename T, Tools::enable_if_tensor<T>>
	T Tools::arccos(const T& source)
	{
		T result = source;
		result.arccos();
		return result;
	}

	// takes the arc tangent / inverse tangent value of the element
	// elem = tan(elem)

	template<typename T, Device device>
	inline Tensor<T, device>& Tensor<T, device>::arctan()
	{
		Compute([](T& elem) {elem = std::atan(elem); });

		return *this;
	}

	template<typename T, Device device>
	inline TensorSlice<T, device>& TensorSlice<T, device>::arctan()
	{
		Compute([](T& elem) {elem = std::atan(elem); });
		return *this;
	}

	template<typename T, Tools::enable_if_tensor<T>>
	T Tools::arctan(const T& source)
	{
		T result = source;
		result.arctan();
		return result;
	}

	//converts radians to degrees
	// elem = 360/pi * elem

	template<typename T, Device device>
	inline Tensor<T, device>& Tensor<T, device>::convDeg()
	{
		Compute([](T& elem) {elem = T(360.0 / Consts::PI * elem); });
		return *this;
	}

	template<typename T, Device device>
	inline TensorSlice<T, device>& TensorSlice<T, device>::convDeg()
	{
		Compute([](T& elem) {elem = T(360.0 / Consts::PI * elem); });
		return *this;
	}

	template<typename T, Tools::enable_if_tensor<T>>
	T Tools::convDeg(const T& source)
	{
		T result = source;
		return result.convDeg();
	}

	//converts degrees to radians
	// elem = pi/360 * elem

	template<typename T, Device device>
	inline Tensor<T, device>& Tensor<T, device>::convRad()
	{
		Compute([](T& elem) {elem = T(Consts::PI / 360.0 * elem); });
		return *this;
	}

	template<typename T, Device device>
	inline TensorSlice<T, device>& TensorSlice<T, device>::convRad()
	{
		Compute([](T& elem) {elem = T(Consts::PI / 360.0 * elem); });
		return *this;
	}

	template<typename T, Tools::enable_if_tensor<T>>
	T Tools::convRad(const T& source)
	{
		T result = source;
		return result.convRad();
	}
}