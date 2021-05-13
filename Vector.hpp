#pragma once

#include <iostream>
#include <memory>
#include <stdexcept>
#include <algorithm>

template<typename InputIterator>
using RequireInputIterator = typename
    std::enable_if<std::is_convertible<typename
                                       std::iterator_traits<InputIterator>::iterator_category,
                                       std::input_iterator_tag>::value>::type;

template <class T>
class Vector {
    public:
        // Member types
        typedef T value_type;
        typedef std::allocator<T> allocator_type;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef T& reference;
        typedef const T& const_reference;
        typedef typename std::allocator_traits<allocator_type>::pointer pointer;
        typedef typename std::allocator_traits<allocator_type>::const_pointer const_pointer;
        typedef T* iterator;
        typedef const T* const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

        // MEMBER FUNCTIONS
        // Constructors
        Vector() { create(); }

        explicit Vector(size_type n, const T& val = T{}) { create(n, val); }

        template <class InputIt, class = RequireInputIterator<InputIt>>
        Vector( InputIt first, InputIt last) { create(first, last); }

        Vector(const Vector& v) { create(v.begin(), v.end()); } // copy c-tor

        Vector(Vector&& v) { // move c-tor
            create(v.begin(), v.end());
            v._data = v.limit = v.avail = nullptr;
        } 

        Vector( std::initializer_list<T> init) { create(init.begin(), init.end()); }

        // Destructor
        ~Vector() { destroy(); }

        // Operator =
        Vector& operator=(const Vector&);
        Vector& operator=(Vector&&);
        Vector& operator=(std::initializer_list<T>);

        // Assign
        void assign(size_type count, const T& value){
            destroy();
            create(count, value);
        }

        template <class InputIt, class = RequireInputIterator<InputIt>>
        void assign(InputIt first, InputIt last){
            destroy();
            create(first, last);
        }

        void assign(std::initializer_list<T> init){
            destroy();
            create(init.begin(), init.end());
        }
        
        // Allocator
        allocator_type get_allocator() const {
            return alloc;
        }

        // MEMBER FUNCTIONS
        // Element access
        
        // At
        reference at(size_type pos) {
            if (pos >= 0 && pos < size()) 
                return _data[pos];
            else throw std::out_of_range("[Vector::at] Index out of range!");
        }

        const_reference at(size_type pos) const {
            if (pos >= 0 && pos < size()) 
                return _data[pos];
            else throw std::out_of_range("[Vector::at] Index out of range!");
        }

        // Operator []
        reference operator[](size_type i) { return _data[i]; }
        const_reference operator[](size_type i) const { return _data[i]; }

        // Front
        reference front() { return _data[0]; }
        const_reference front() const { return _data[0]; }
        
        // Back
        reference back() { return _data[size()-1]; }
        const_reference back() const { return _data[size()-1]; }

        // Data
        T* data() { return _data; }
        const T* data() const { return _data; }
    
        // MEMBER FUNCTIONS
        // Iterator

        // Begin
        iterator begin() { return _data; }
        const_iterator begin() const { return _data; }
        const_iterator cbegin() const { return begin(); }

        // End
        iterator end() { return avail; }
        const_iterator end() const { return avail; }
        const_iterator cend() const { return end(); }

        // Reverse begin
        reverse_iterator rbegin() {
            return reverse_iterator(end());
        }
        const_reverse_iterator rbegin() const {
            return const_reverse_iterator(end());
        }
        const_reverse_iterator crbegin() const {
            return rbegin();
        }

        // Reverse end
        reverse_iterator rend() {
            return reverse_iterator(begin());
        }
        const_reverse_iterator rend() const {
            return const_reverse_iterator(begin());
        }
        const_reverse_iterator crend() const {
            return rend();
        }

        // MEMBER FUNCTIONS
        // Capacity

        // Empty
        bool empty() const {
            return _data == limit;
        }

        // Size
        size_type size() const { return avail - _data; }

        // Max size
        size_type max_size() const {
            return alloc.max_size();
        }

        // Reserve
        void reserve(size_type new_cap) {
            if (new_cap > capacity()){
                grow(new_cap);
            }    
        }

        // Capacity
        size_type capacity() {
            return limit - _data;
        }

        // Shrink to fit
        void shrink_to_fit() {
            grow(1, true);
        }

        // MEMBER FUNCTIONS
        // Modifiers

        // Clear
        void clear() {
            destroy();
        }

        // Insert
        iterator insert(iterator pos, const T& value) {
            size_type count = 1;
            return insert(pos, count, value);
        }
        iterator insert( iterator pos, size_type count, const T& value ) {
            if (pos < begin() || pos > end())
                throw std::out_of_range("[Vector::insert] Index out of range!");

            if (size() + count > capacity()) {
                // Return position pointer to new location
                size_type index = pos - begin();
                grow(size() + count);
                pos = index + begin();
            }

            // Move objects to other memory location
            int i = 0;
            for (iterator it = (avail + count - 1); it != (pos + count - 1); it--, i++) {
                *it = _data[size()- i - 1];
            }

            std::uninitialized_fill(pos, pos+count, value);
            avail += count;

            return pos;
        }
        template <class InputIt, class = RequireInputIterator<InputIt>>
        iterator insert( iterator pos, InputIt first, InputIt last ) {
            if (pos < begin() || pos > end())
                throw std::out_of_range("[Vector::insert] Index out of range!");

            size_type count = last - first;

            if (size() + count > capacity()) {
                // Return position pointer to new location
                size_type index = pos - begin();
                grow(size() + count);
                pos = index + begin();
            }

            // Move objects to other memory location
            int i = 0;
            for (iterator it = (avail + count - 1); it != (pos + count - 1); it--, i++) {
                *it = _data[size()- i - 1];
            }

            std::uninitialized_copy(first, last, pos);
            avail += count;

            return pos;
        }

        // Emplace
        template <class... Args>
        iterator emplace( iterator pos, Args&&... args ) {
            if (pos < begin() || pos > end())
                throw std::out_of_range("[Vector::emplace] Index out of range!");
            
            if (size() + 1 > capacity()) {
                // Return position pointer to new location
                size_type index = pos - begin();
                grow();
                pos = index + begin();
            }

            // Move objects to other memory location
            int i = 0;
            for (iterator it = avail; it != pos; it--, i++) {
                *it = _data[size()- i - 1];
            }

            alloc.construct(pos, std::forward<Args>(args)...);
            avail += 1;

            return pos;
        }

        // Erase
        iterator erase( iterator pos ) {
            if (pos < begin() || pos > end())
                throw std::out_of_range("[Vector::erase] Index out of range!");

            avail = std::uninitialized_copy(pos+1, end(), pos);
            alloc.destroy(avail);

            return pos;
        }
        iterator erase( iterator first, iterator last ){
            iterator new_avail = std::uninitialized_copy(last, end(), first);

            iterator it = avail;
            while (it != new_avail) {
                alloc.destroy(--it);
            }

            avail = new_avail;
            return last;
        }

        // Push back
        void push_back(const value_type& val) {
            if (avail == limit)
                grow();
            unchecked_append(val);
        }
        void push_back(const value_type&& val) {
            if (avail == limit)
                grow();
            unchecked_append(val);
            val;
        }

        // Emplace back
        template <class... Args>
        reference emplace_back( Args&&... args ) {
            if (avail == limit)
                grow();
            alloc.construct(avail++, std::forward<Args>(args)...);
            return back();
        }

        // Pop back
        void pop_back() {
            erase(end() - 1);
        }

        // Resize
        void resize( size_type count, const value_type& value = T{}){
            if (size() < count) {
                insert(end(), count - size(), value);
            } else if (size() > count) {
                erase(begin()+count, end());
            }
        }

        // Swap
        void swap(Vector& other) {
            std::swap(_data, other._data);
            std::swap(avail, other.avail);
            std::swap(limit, other.limit);
        }
    private:
        iterator _data; // Pirmas elementas
        iterator avail;
        iterator limit;

        std::allocator<T> alloc; // objektas atminties valdymui

        // išskirti atmintį (array) ir ją inicializuoti
        void create();
        void create(size_type, const T&);
        void create(const_iterator, const_iterator);
        template <class InputIt>
        void create(InputIt, InputIt);

        // sunaikinti elementus array ir atlaisvinti atmintį
        void destroy();

        // pagalbinės funkcijos push_back realizacijai
        void grow(size_type new_cap = 1, bool shrink = false);
        void unchecked_append(const T&);
};

// Member functions
// Operator =
template <class T>
Vector<T>& Vector<T>::operator=(const Vector& rhs) {
    if (&rhs != this) {
        destroy();
        create(rhs.begin(), rhs.end());
    }
    return *this;
}

template <class T>
Vector<T>& Vector<T>::operator=(Vector&& rhs) {
    if (&rhs != this) {
        destroy();
        create(rhs.begin(), rhs.end());
        rhs._data = rhs.limit = rhs.avail = nullptr;
    }
    return *this;
}

template <class T>
Vector<T>& Vector<T>::operator=(std::initializer_list<T> ilist) {
    destroy();
    create(ilist.begin(), ilist.end());
    return *this;
}

// NON-MEMBER FUNCTIONS
// Operators
template <class T>
bool operator==(const Vector<T>& lhs, const Vector<T>& rhs) {
    return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <class T>
bool operator!=(const Vector<T>& lhs, const Vector<T>& rhs) {
    return !(lhs == rhs);
}

template <class T>
bool operator<(const Vector<T>& lhs, const Vector<T>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <class T>
bool operator<=(const Vector<T>& lhs, const Vector<T>& rhs) {
    return !(rhs < lhs);
}

template <class T>
bool operator>(const Vector<T>& lhs, const Vector<T>& rhs) {
    return rhs < lhs;
}

template <class T>
bool operator>=(const Vector<T>& lhs, const Vector<T>& rhs) {
    return !(lhs < rhs);
}

// NON-MEMBER FUNCTIONS
// Swap
template <class T>
void swap(const Vector<T>& lhs, const Vector<T>& rhs) {
    std::swap(lhs, rhs);
}

// PRIVATE FUNCTIONS

template <class T>
void Vector<T>::create() {
    _data = avail = limit = nullptr;
}

template <class T>
void Vector<T>::create(size_type n, const T& val) {
    _data = alloc.allocate(n); // grąžina ptr į pirmą elementą
    limit = avail = _data + n; // sustato rodykles
    std::uninitialized_fill(_data, limit, val); // inicializuoja elementus
}

template <class T>
void Vector<T>::create(const_iterator i, const_iterator j) {
    _data = alloc.allocate(j-i); // iskiriama vieta
    limit = avail = std::uninitialized_copy(i, j, _data); // nukopijuoja elemetus
}

template <class T>
template <class InputIt>
void Vector<T>::create(InputIt i, InputIt j) {
    _data = alloc.allocate(j-i); // iskiriama vieta
    limit = avail = std::uninitialized_copy(i, j, _data); // nukopijuoja elemetus
}

template <class T>
void Vector<T>::destroy() {
    if (_data) {
        // sunaikinti sukonstruotus elementus
        iterator it = avail;
        while (it != _data)
            alloc.destroy(--it);
        alloc.deallocate(_data, limit - _data);
    }
    // reset pointers
    _data = limit = avail = nullptr;
}

template <class T>
void Vector<T>::grow(size_type new_cap, bool shrink) {
    // dvigubai vietos
    size_type new_size;
    if(shrink) {
        new_size = avail - _data;
    } else {
        new_size = std::max(2 * capacity(), new_cap);
    }

    // naujos vietos išskyrimas ir kopijavimas
    iterator new__data = alloc.allocate(new_size);
    iterator new_avail = std::uninitialized_copy(_data, avail, new__data);

    // vietos atlaisvinimas
    destroy();

    // rodyklės rodo į naują vietą
    _data = new__data;
    avail = new_avail;
    limit = _data + new_size;
}

// avail rodo į išskirtą, bet neinicilizuotą vietą
template <class T>
void Vector<T>::unchecked_append(const T& val) {
    alloc.construct(avail++, val);
}