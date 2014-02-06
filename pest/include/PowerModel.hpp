#pragma once

#include <string>
#include <vector>

#include <boost/lockfree/queue.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/thread.hpp>

#include "SimEvent.hpp"

typedef std::vector<unsigned long> OutputVector;

class PowerModel {
    public:
        PowerModel(boost::lockfree::queue<std::string*> *q, boost::atomic<bool> *done, unsigned long bucket_size = 10000);
        virtual ~PowerModel();
        virtual int calculate(const SimEvent *se) = 0;
        OutputVector getOutput() const;
        int run();
    protected:
        boost::lockfree::queue<std::string*> *q;
        boost::atomic<bool> *done;
        OutputVector output;
        unsigned long bucket_size;
};
