#ifndef __ARRANGEMENT_BUFFER_H__
#define __ARRANGEMENT_BUFFER_H__

#include <boost/circular_buffer.hpp>

#include "utilities/Singleton.hpp"
#include "arrangement/ArrangementTypes.h"

class ArrangementBuffer : public Singleton<ArrangementBuffer>
{
    friend class Singleton<ArrangementBuffer>;

private:
    explicit ArrangementBuffer();
    ~ArrangementBuffer() {}

    boost::circular_buffer<Pol_arr*> _arrangementsBuffer;

public:
    void push_back(Pol_arr* arrangement);
    void set_capacity(const unsigned int size);
    const size_t capacity();
    const size_t size();
    Pol_arr* back();
    void pop_back();
};

#endif // __ARRANGEMENT_BUFFER_H__