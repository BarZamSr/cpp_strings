#ifndef LIST
#define LIST

#include <cstdlib>
#include <iostream>
#include <cassert>

#include "utils.cpp"

template <class T>
class List {
public:
	List(): List(utils::Fibonacci::bigger_than(0)) {
		// hmmm
	}
	List(std::initializer_list<T> A): List(A.size()) {
		len = A.size();
		utils::copy(A.begin(), array, A.size());
	}
	List(const T * A, int n): List(n) {
		assert(A != NULL);

		utils::copy(A, array, n);
		len = n;
	}
	List(const List & other): List(other.cap) {
		assert(other.len >= 0);

		len = other.len;
		if (len != 0) {
			utils::copy(other.array, array, len);
		}
	}
	List(const List && other): List(other.cap) {
		len = other.len;

		if (len != 0) {
			assert(len > 0);
			utils::copy(other.array, array, len);
		}
	}
	~List() {
		std::free(array);
	}

	int get_len() const {
		return len;
	}
	int get_cap() const {
		return cap;
	}
	int is_empty() const {
		return len == 0;
	}

	friend void swap(List<T> & A, List<T> & B) {
		using std::swap; // enable ADL

		swap(A.array, B.array);
		swap(A.len, B.len);
		swap(A.cap, B.cap);
	}

	// now featuring the copy-and-swap idiom!
	List<T> & operator= (List<T> other) {
		swap(*this, other);
		return *this;
	}

	// element access
	T & operator[] (int index) {
		if(index < 0) {
			assert(0-index <= len);
			return array[len + index];
		}
		else {
			assert(index < len);
			return array[index];
		}
	}
	const T & operator[] (int index) const {
		if(index < 0) {
			assert(0-index <= len);
			return array[len + index];
		}
		else {
			assert(index < len);
			return array[index];
		}
	}
	const T * begin() const {
		return array;
	}
	const T * end() const {
		return array + len;
	}

	int find(T object) {
		return find(object, 0, len);
	}
	int find(T object, int start) {
		return find(object, start, len);
	}
	int find(T object, int start, int end) {
		assert(end <= len);
		for(int i = start; i < end; i++) {
			if (array[i] == object) return i;
		}
		return ERR_VAL;
	}

	void push(T object) {
		if (len == cap) {
			expand();
		}
		array[len++] = object;
	}
	T pop() {
		if (len < utils::Fibonacci::smaller_than(cap)) {
			shrink();
		}
		return array[--len];
	}

	void operator+= (const List<T> & other) {
		if (cap < len + other.len) {
			expand_above(len + other.len);
		}
		utils::copy(other.array, array+len, other.len);
		len += other.len;
	}

	List<T> operator+ (const List<T> & other) {
		List<T> sum(*this);
		sum += other;
		return sum;
	}

	List<T> operator* (int n) {
		assert(n > 0);

		List<T> product(len * n);
		product.len = len * n;

		for(int i=0; i<n; i++) {
			utils::copy(array,
				product.array + (len * i), len);
		}
		return product;
	}

	friend std::ostream & operator<< (std::ostream & stream,
						const List<T> & list) {
		int i;
		stream << '[';
		for(i=0; i<list.get_len()-1; i++) {
			stream << list[i] << ", ";
		}
		stream << list[i] << ']';
		return stream;
	}
protected:
	T* array;
	int len, cap;

	List(int n) {
		assert(n > 0);
		n = utils::Fibonacci::bigger_than(n);

		len = 0;
		cap = n;
		array = static_cast<T *> (
			std::malloc(n * sizeof(T))
		);
	}

	void expand_above(int n) {
		assert(n >= cap);
		n = utils::Fibonacci::bigger_than(n);

		cap = n;
		array = static_cast<T *> (
			std::realloc(array, n * sizeof(T))
		);
	}
	void expand() {
		expand_above(cap);
	}

	void shrink_below(int n) {
		assert(n <= cap);
		n = utils::Fibonacci::smaller_than(n);
		assert(n >= len);

		cap = n;
		array = static_cast<T *> (
			std::realloc(array, n * sizeof(T))
		);
	}
	void shrink() {
		shrink_below(cap);
	}
};


#endif
