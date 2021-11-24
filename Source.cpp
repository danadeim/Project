#include <iostream>
#include <stdexcept> // out_of_range
#include <type_traits> // is_trivially_copyable
#include <cstring> // memcpy, memmove

template <typename T>
class MyVector {
	size_t size;
	size_t cap;
	T* arr;
public:
	bool copyable() {
		return std::is_trivially_copyable<T>::value;
	}
	MyVector();
	MyVector(const MyVector<T>&);
	MyVector<T> operator=(const MyVector<T>&);
	MyVector<T> operator=(MyVector<T>&&);
	MyVector<T>(MyVector<T>&&);
	~MyVector<T>();
	T operator[](size_t i) const;
	T& operator[](size_t i);
	size_t getSize();
	void insert(size_t ind, const T e[], size_t sz);
	void insert(size_t ind, const T& e) { this->insert(ind, new T[1]{ e }, 1); };
	void append(const T& e) { this->append(new T[1]{ e }, 1); };
	void append(const T e[], size_t sz) { this->insert(this->getSize(), e, sz); };
	size_t remove(const T& e, size_t from = 0);
	bool removeAll(const T& e, size_t from = 0);
	friend std::ostream& operator<<(std::ostream& s, const MyVector<T>& v) {
		s << "(" << v.size << "/" << v.cap << ") ";
		if (v.size != 0) {
			s << "[ ";
			for (int i = 0; i < v.size; i++) {
				s << v.arr[i] << " ";
			}
			s << "]";
		}
		return s;
	}
};

template <typename T>
MyVector<T>::MyVector() {
	this->cap = 2;
	this->size = 0;
	this->arr = new T[2];
	for (int i = 0; i < this->cap; i++) {
		this->arr[i] = 0;
	}
}

template <typename T>
MyVector<T>::MyVector(const MyVector<T>& vec) {
	this->cap = vec.cap;
	this->size = vec.size;
	this->arr = new T[vec.cap];
	for (int i = 0; i < this->cap; i++) {
		this->arr[i] = vec.arr[i];
	}
}

template<typename T>
MyVector<T> MyVector<T>::operator=(const MyVector<T>& other)
{
	this->cap = other.cap;
	this->size = other.size;
	this->arr = new T[other.cap];
	for (int i = 0; i < this->cap; i++) {
		this->arr[i] = 0;
	}
	for (int i = 0; i < this->size; i++) {
		this->arr[i] = other[i];
	}
	MyVector<T>* tmp = this;
	return *tmp;
}

template<typename T>
MyVector<T>::MyVector(MyVector<T>&& mv) : arr(mv.arr) {
	mv.arr = nullptr;
}

template<typename T>
MyVector<T>::~MyVector() {
	delete[] arr;
	std::cout << "1";
}

template<typename T>
T MyVector<T>::operator[](size_t i) const {
	if (i >= 0 && i < this->size) {
		return this->arr[i];
	}
	else {
		throw std::out_of_range("operator[]");
	}
}

template<typename T>
T& MyVector<T>::operator[](size_t i) {
	if (i >= 0 && i < this->size) {
		return this->arr[i];
	}
	else {
		throw std::out_of_range("operator[]");
	}
}

template<typename T>
size_t MyVector<T>::getSize() {
	return this->size;
}

template<typename T>
void MyVector<T>::insert(size_t ind, const T e[], size_t sz) {
	if (ind > this->size) {
		throw std::out_of_range("insert 3par");
	}
	else {
		if (this->cap >= this->size + sz) {
			if (copyable()) {
				memcpy(&this->arr[0] + ind + sz, &this->arr[0] + ind, (this->size - ind) * sizeof(e[0]));
				memcpy(&this->arr[0] + ind, e, sz * sizeof(e[0]));
				this->size += sz;
			}
			else {
				for (int i = ind; i < this->size - ind; i++) {
					this->arr[i] = this->arr[i + sz];
				}
				for (int i = ind; i < sz; i++) {
					this->arr[i] = e[i];
				}
				this->size += sz;
			}
		}
		else {
			if (copyable()) {
				size_t newSize = 2 * (this->size + sz);
				T* newArr = new T[newSize];
				for (int i = 0; i < newSize; i++) {
					newArr[i] = 0;
				}
				memcpy(newArr, this->arr, ind * sizeof(e[0]));
				memcpy(newArr + ind, e, sz * sizeof(e[0]));
				memcpy(newArr + sz, this->arr + ind, (this->size - ind) * sizeof(e[0]));

				this->arr = newArr;
				this->cap = newSize;
				this->size += sz;
			}
			else {
				T* newArr = new T[2 * (this->size + sz)];
				for (int i = 0; i < ind; i++) {
					newArr[i] = this->arr[i];
				}
				for (int i = ind, j = 0; i < sz; i++) {
					newArr[i] = e[j++];
				}
				for (int i = ind + sz, j = ind; j < this->size; i++, j++) {
					newArr[i] = this->arr[j];
				}

				delete this->arr;
				this->arr = newArr;
				this->size += sz;
			}
		}
	}
}

template<typename T>
size_t MyVector<T>::remove(const T& e, size_t from) {
	if (from >= this->size) {
		throw std::out_of_range("remove");
	}
	else {
		for (int i = from; i < this->size; i++) {
			if (this->arr[i] == e) {
				for (int j = i; j < this->size - 1; j++) {
					this->arr[j] = this->arr[j + 1];
				}
				this->size--;
				return i;
			}
		}
		return -1;
	}
}

template<typename T>
bool MyVector<T>::removeAll(const T& e, size_t from) {
	bool wasDeleted = false;
	for (int i = from; i < this->size; i++) {
		if (this->remove(e, from) != -1) {
			wasDeleted = true;
		}
	}
	return wasDeleted;
}

int main() {
	int a[]{ 1,2 };
	MyVector<int> vi = MyVector<int>();
	std::cout << "*0* " << vi << std::endl;
	vi.insert(0, a, 2);
	std::cout << "*1* " << vi << std::endl;
	vi.insert(2, a, 2);
	std::cout << "*2* " << vi << std::endl;
	vi.insert(2, &a[1], 1);
	std::cout << "*3* " << vi << std::endl;
	vi.append(a, 2);
	vi.append(3);
	vi.append(4);
	std::cout << "*4* " << vi << std::endl;
	vi.removeAll(2);

	std::cout << "*5* " << vi << std::endl;
	size_t res = vi.remove(1);
	std::cout << "*6* " << vi << " "
		<< "res=" << res << std::endl;
	res = vi.remove(4);
	std::cout << "*7* " << vi << " "
		<< "res=" << res << std::endl;
}