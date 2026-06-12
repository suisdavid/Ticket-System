#pragma once
#include <iostream>
namespace sjtu
{
    template <typename T>
    class vector
    {
    private:
        T *data;
        int len, capacity;

    public:
        vector()
        {
            len = 0;
            capacity = 0;
            data = nullptr;
        }
        vector(const vector &other)
        {
            len = other.len;
            capacity = other.capacity;
            if (capacity)
            {
                data = (T *)operator new(sizeof(T) * capacity);
            }
            else
            {
                data = nullptr;
            }
            for (int i = 0; i < len; i++)
            {
                new (&data[i]) T(other.data[i]);
            }
        }
        /**
         * TODO Destructor
         */
        ~vector()
        {
            for (int i = 0; i < len; i++)
            {
                data[i].~T();
            }
            operator delete(data);
        }
        /**
         * TODO Assignment operator
         */
        vector &operator=(const vector &other)
        {
            if (this == &other)
            {
                return *this;
            }
            for (int i = 0; i < len; i++)
            {
                data[i].~T();
            }
            len = other.len;
            capacity = other.capacity;
            operator delete(data);
            data = nullptr;
            if (capacity)
            {
                data = (T *)operator new(sizeof(T) * capacity);
            }
            for (int i = 0; i < len; i++)
            {
                new (&data[i]) T(other.data[i]);
            }
            return *this;
        }
        /**
         * assigns specified element with bounds checking
         * throw index_out_of_bound if pos is not in [0, size)
         */
        T &at(const size_t &pos)
        {
            return data[pos];
        }
        const T &at(const size_t &pos) const
        {
            return data[pos];
        }
        /**
         * assigns specified element with bounds checking
         * throw index_out_of_bound if pos is not in [0, size)
         * !!! Pay attentions
         *   In STL this operator does not check the boundary but I want you to do.
         */
        T &operator[](const size_t &pos)
        {
            return data[pos];
        }
        const T &operator[](const size_t &pos) const
        {
            return data[pos];
        }
        /**
         * access the first element.
         * throw container_is_empty if size == 0
         */
        const T &front() const
        {
            return data[0];
        }
        /**
         * access the last element.
         * throw container_is_empty if size == 0
         */
        T back() const
        {
            return data[len - 1];
        }
        /**
         * returns an iterator to the beginning.
         */

        bool empty() const
        {
            return !len;
        }
        /**
         * returns the number of elements
         */
        size_t size() const
        {
            return len;
        }
        /**
         * clears the contents
         */
        void clear()
        {
            for (int i = 0; i < len; i++)
            {
                data[i].~T();
            }
            operator delete(data);
            data = nullptr;
            len = 0;
            capacity = 0;
        }

        void push_back(const T &value)
        {
            if (len == capacity)
            {
                capacity = (capacity == 0 ? 1 : capacity << 1);
                T *newdata = (T *)operator new(sizeof(T) * capacity);
                for (int i = 0; i < len; i++)
                {
                    new (&newdata[i]) T(data[i]);
                    data[i].~T();
                }
                operator delete(data);
                data = newdata;
            }
            new (&data[len]) T(value);
            len++;
        }

        /**
         * remove the last element from the end.
         * throw container_is_empty if size() == 0
         */
        void pop_back()
        {
            data[--len].~T();
        }
    };
}
