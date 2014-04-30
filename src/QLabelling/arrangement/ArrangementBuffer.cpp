#include "ArrangementBuffer.h"

ArrangementBuffer::ArrangementBuffer()
{
    _arrangementsBuffer.set_capacity(50);
}

void ArrangementBuffer::set_capacity( const unsigned int size )
{
    _arrangementsBuffer.set_capacity(size);
}

const size_t ArrangementBuffer::capacity()
{
    return _arrangementsBuffer.capacity();
}

const size_t ArrangementBuffer::size()
{
    return _arrangementsBuffer.size();
}

void ArrangementBuffer::push_back(Pol_arr* arrangement)
{
    // It is required to (deep) copy the arrangement, otherwise, the same pointer is pushed back every time ...
    Pol_arr* arrCopy = new Pol_arr(*arrangement);
    _arrangementsBuffer.push_back(arrCopy);
}

Pol_arr* ArrangementBuffer::back()
{
    return _arrangementsBuffer.back();
}

void ArrangementBuffer::pop_back()
{
    _arrangementsBuffer.pop_back();
}
