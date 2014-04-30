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
    _arrangementsBuffer.push_back(arrangement);
}
