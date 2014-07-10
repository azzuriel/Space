#pragma once
#include <vector>
#include <xmemory>
template <class _Ty, class _Alloc = std::allocator<_Ty> > class sparse_vector : public std::vector<_Ty, _Alloc>
{
private:
    std::vector<size_type> empty;
public:
    iterator erase(iterator _Where)
    {	
        empty.push_back(_Where - vector::begin());
        (*_Where) = 0;
        return _Where + 1;
    }
    void pop_back(){
        empty.push_back(vector::size() - 1);
        *(vector::end() - 1) = 0;
    }
    void push_back(const value_type& _Val){
        if(empty.empty()){
            vector::push_back(_Val);
            return;
        }
        size_type pos = *(empty.end() - 1);
        empty.pop_back();
        *(vector::begin() + pos) = _Val;
    }
};