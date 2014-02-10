#include "SimpleModel.hpp"
#include "TraceLine.hpp"

#include "SimEvent.hpp"
#include "Instruction.hpp"

using namespace boost;

SimpleModel::SimpleModel(lockfree::queue<std::string*> *q, atomic<bool> *done, unsigned long bucket_size) : PowerModel(q, done, bucket_size), m(){ }

SimpleModel::~SimpleModel() {}

int SimpleModel::calculate(const SimEvent *se) {

    if (se->getType() == EventType::Unknown)
        return 0;

    const Instruction *inst = (Instruction *) se;

    while (output.size() <= (se->getTick() / bucket_size)) {
        output.push_back(0L);
        if (!(output.size() % 10000)) {
            printf("Output vector is %lu long.", output.size());
        }
    }

    if (inst->getType() == EventType::InstEvent) {

        switch (inst->getInstrType()) {
            case IntAlu:
                output[inst->getTick()/bucket_size] += 28;
                break;
            case IntMult:
                output[inst->getTick()/bucket_size] += 23;
                break;
            case MemRead:
                output[inst->getTick()/bucket_size] += 18;
                break;
            case MemWrite:
                output[inst->getTick()/bucket_size] += 13;
                break;
            case SimdFloatMisc:
                output[inst->getTick()/bucket_size] += 43;
                break;
            case ErrorType:
                break;
        }
    } else {
        // For now, don't care about MemEvents and others.
    }
    return 0;
}
