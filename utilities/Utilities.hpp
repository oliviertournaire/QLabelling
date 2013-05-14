#ifndef UTILITIES_HPP
#define UTILITIES_HPP

int randInt(int low, int high)
{
    // Random number between low and high
    return qrand() % ((high + 1) - low) + low;
}

#endif // UTILITIES_HPP
