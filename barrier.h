#ifndef BARRIER_H
#define BARRIER_H


class Barrier
{
public:
    Barrier();
    Barrier(bool faction);
    int _row;
    int _col;
    int length = 120;
    int width = 10;
    bool _redFaction;
    bool _across = 1;

private:

};

#endif // BARRIER_H
