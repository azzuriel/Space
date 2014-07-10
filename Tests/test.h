#pragma once
#include <vector>
#include <assert.h>
#include <easylogging++.h>
#define SHOW_PASSED 1
#define NOT_SHOW_PASSED 0
#define BREAK_ON_ERROR 2

#define TEST_ASSERT_EQUAL(a, b, showpassed, fail) \
    if(a != b) \
        { LOG(ERROR) << __FILE__ << " line " << __LINE__ << " :: " << #a << "==" << #b << " (" << a << " == " << b << ") failed"; \
          fail = true; \
          if(showpassed == BREAK_ON_ERROR) throw std::logic_error("Test error."); \
        } \
    else if(showpassed == SHOW_PASSED) \
        LOG(ERROR) << __FILE__ << " line " << __LINE__ << " :: " << #a << "==" << #b << " (" << a << " == " << b << ") passed"; 

#define TEST_ASSERT_FALSE(a, showpassed, fail) \
    if(a != false) \
        { LOG(ERROR) << __FILE__ << " line " << __LINE__ << " :: " << #a << "==" << "false" << " (" << a << " == " << 0 << ") failed"; \
          fail = true; \
          if(showpassed == BREAK_ON_ERROR) throw std::logic_error("Test error."); \
        } \
    else if(showpassed == SHOW_PASSED) \
        LOG(ERROR) << __FILE__ << " line " << __LINE__ << " :: " << #a << "==" << "false" << " (" << a << " == " << 0 << ") passed";

#define TEST_ASSERT_TRUE(a, showpassed, fail) \
    if(a != true) \
        { LOG(ERROR) << __FILE__ << " line " << __LINE__ << " :: " << #a << "==" << "true" << " (" << a << " == " << 1 << ") failed"; \
          fail = true; \
          if(showpassed == BREAK_ON_ERROR) throw std::logic_error("Test error."); \
        } \
    else if(showpassed == SHOW_PASSED) \
    LOG(ERROR) << __FILE__ << " line " << __LINE__ << " :: " << #a << "==" << "true" << " (" << a << " == " << 1 << ") passed";

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

