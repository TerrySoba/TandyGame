#ifndef _INCLUDE_VECTOR_H
#define _INCLUDE_VECTOR_H

namespace tnd
{

template <class ValueT>
class vector
{
public:
    vector(const vector& other)
    {
        m_capacity = other.m_size;
        m_size = other.m_size;
        m_data = new ValueT[m_size];
        for (int i = 0; i < other.size(); ++i)
        {
            m_data[i] = other.m_data[i];
        }
    }

    vector()
    {
        m_capacity = 0;
        m_size = 0;
        m_data = (ValueT*)0;
    }

    ~vector()
    {
        delete[] m_data;
    }

    void operator=(const vector& other)
    {
        delete[] m_data;
        m_capacity = other.m_size;
        m_size = other.m_size;
        m_data = new ValueT[m_size];
        for (int i = 0; i < other.size(); ++i)
        {
            m_data[i] = other.m_data[i];
        }
    }

    void push_back(const ValueT& val)
    {
        if (m_size + 1 > m_capacity)
        {
            ValueT* tmp = new ValueT[m_size + 1];
            m_capacity = m_size + 1;
            for (int i = 0; i < m_size; ++i)
            {
                tmp[i] = m_data[i];
            }
            delete[] m_data;
            m_data = tmp;
        }
        m_data[m_size] = val;
        ++m_size;
    }

    void clear()
    {
        m_size = 0;
    }

    const ValueT& operator[](int index) const
    {
        return m_data[index];
    }

    ValueT& operator[](int index)
    {
        return m_data[index];
    }

    int size() const
    {
        return m_size;
    }


private:
    ValueT* m_data;
    int m_capacity;
    int m_size;
};

}

#endif