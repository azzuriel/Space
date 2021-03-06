#pragma once
#include <vector>
#include <xmemory>

template<typename _Ty> 
class nullable
{
public:
    nullable() : m_value(), m_isSet(false) {}
    nullable(_Ty value) : m_value(value), m_isSet(true) {} 
    nullable(const nullable& other) : m_value(other.m_value), m_isSet(other.m_isSet) {}

    friend void swap(nullable& a, nullable& b)
    {
        std::swap(a.m_isSet, b.m_isSet);
        std::swap(a.m_value, b.m_value);
    }

    nullable& operator=(nullable other) 
    {
        swap(*this, other);
        return *this;
    }

    _Ty operator=(_Ty value) { set(value); return m_value; }
    bool operator==(_Ty value) { return m_isSet && value == m_value; }
    operator _Ty() const { return get(); }

    _Ty get() const 
    {
        if (m_isSet) return m_value;
        else throw std::logic_error("Value of Nullable is not set.");
    }

    bool has_value() const { return m_isSet; }

    void reset() { m_isSet = false; m_value = _Ty(); }

private:
    void set(_Ty value) { m_value = value; m_isSet = true; }

private:
    _Ty m_value;
    bool m_isSet;
};

template <class _Ty, class _Alloc = std::allocator<_Ty> > class sparse_vector : public std::vector<nullable<_Ty>, _Alloc>
{
private:
    std::vector<size_type> empty;
    size_type last_full;
public:
    sparse_vector(){
        last_full = -1;
    }

    iterator erase(iterator _Where)
    {	
        size_type pos = _Where - vector::begin();
        empty.push_back(pos);
        if(pos == last_full){
            last_full--;
        }
        (*_Where).reset();
        return (_Make_iter(_Where));
    }

    iterator erase(iterator _First_arg,
        iterator _Last_arg)
    {	
        iterator i = _First_arg;
        while(i != _Last_arg) {
            empty.push_back(i - vector::begin());
            (*i).reset();
            i++;
        }
        if(last_full > _First_arg - vector::begin()){
            last_full == _First_arg - vector::begin() - 1;
        }
        
        return (_Make_iter(_First_arg));
    }

    void pop_back(){
        empty.push_back(last_full);
        vector::at(last_full).reset();
        last_full--;
    }

    void push_back(const value_type& _Val){
        if(empty.empty()){
            vector::push_back(_Val);
            last_full = vector::size() - 1;
            return;
        }
        size_type pos = *(empty.end() - 1);
        if(pos > last_full) {
            last_full = pos;
        }
        empty.pop_back();
        *(vector::begin() + pos) = _Val;
    }
};