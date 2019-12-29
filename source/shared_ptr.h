#ifndef SHARED_PTR_INCLUDED
#define SHARED_PTR_INCLUDED

template <class ValueT>
struct shared_ptr_storage
{
    int user_count;
    ValueT* ptr;
};


template <class ValueT>
class shared_ptr
{
public:
    shared_ptr(ValueT* ptr)
    {
        m_storage = new shared_ptr_storage<ValueT>;
        m_storage->user_count = 1;
        m_storage->ptr = ptr;
    }

    shared_ptr(const shared_ptr<ValueT>& other)
    {
        m_storage = other.m_storage;
        m_storage->user_count++;
    }

    shared_ptr<ValueT>& operator=(const shared_ptr<ValueT>& other)
    {
        if (this == &other)
            return;
            
        delete_storage();
        m_storage = other.m_storage;
        m_storage->user_count++;
        return *this;
    }

    ~shared_ptr()
    {
        delete_storage();
    }

    ValueT* get()
    {
        return m_storage->ptr;
    }

    int use_count()
    {
        return m_storage->user_count;
    }

    ValueT* operator->()
    {
        return m_storage->ptr;
    }

    ValueT& operator*()
    {
        return *m_storage->ptr;
    }

private:
    void delete_storage()
    {
        if (m_storage->user_count == 1)
        {
            delete m_storage->ptr;
            delete m_storage;
        }
        else
        {
            m_storage->user_count--;
        }
    }

private:
    shared_ptr_storage<ValueT>* m_storage;
};

#endif
