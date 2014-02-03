#include "SimpleModel.hpp"
#include "TraceLine.hpp"

using namespace boost;

SimpleModel::SimpleModel(lockfree::queue<std::string*> *q, atomic<bool> *done, unsigned long buckets) : PowerModel(q, done, buckets), m(){ }

SimpleModel::~SimpleModel() {}

int SimpleModel::calculate(TraceLine tr)
{
    while (output.size() <= (tr.getTick()/bucket_size)) {
        atomic<unsigned long> *ul = new atomic<unsigned long>(0L);
        m.lock();
        output.push_back(ul);
        m.unlock();
    }
    if (!(output.size() % 10000)) printf("Output vector is %lu long.", output.size());
    switch (tr.getInstr().getInstrType())
    {
        case IntAlu:
            *(output[tr.getTick()/bucket_size]) += 28;
            break;
        case IntMult:
            *(output[tr.getTick()/bucket_size]) += 23;
            break;
        case MemRead:
            *(output[tr.getTick()/bucket_size]) += 18;
            break;
        case MemWrite:
            *(output[tr.getTick()/bucket_size]) += 13;
            break;
        case SimdFloatMisc:
            *(output[tr.getTick()/bucket_size]) += 43;
            break;
        case ErrorType:
            break;
    }
    return 0;
}
