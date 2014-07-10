#pragma once
#include <vector>
#include <assert.h>
#include <easylogging++.h>
#define SHOW_PASSED 1
#define NOT_SHOW_PASSED 0
#define BREAK_ON_ERROR 2

#define TEST_ASSERT_EQUAL(a, b, showpassed, fail) \
    if(showpassed == BREAK_ON_ERROR) \
        assert(a == b); \
    if(a != b) \
        LOG(ERROR) << __FILE__ << " line " << __LINE__ << " :: " << #a << "==" << #b << " (" << a << " == " << b << ") failed"; \
    else if(showpassed == SHOW_PASSED) \
        LOG(ERROR) << __FILE__ << " line " << __LINE__ << " :: " << #a << "==" << #b << " (" << a << " == " << b << ") passed"; \
    if(fail == false) fail = a == b;

class test{
public:
    virtual bool make(int showpassed) = 0;
};

class tester{
private:
    int m_passed, m_failed;
    std::vector<test*> t_list;
public:
    void make_all(int showpassed) {
        m_passed = m_failed = 0;
            for (int i =0; i<t_list.size();i++)
            {
                if(t_list[i]->make(showpassed)){
                    m_passed++;
                } else {
                    m_failed++;
                }
            }
    }

    void add(test* t){
        t_list.push_back(t);
    }

    int total(){
        return m_passed + m_failed;
    }
    int passed(){
        return m_passed;
    }
    int failed(){
        return m_failed;
    }
};

