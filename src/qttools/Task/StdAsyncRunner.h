#pragma once

#include "BaseRunner.h"

#include <future>

namespace Task {

/*! \brief Task runner based on std::async()
 */
class StdAsyncRunner : public BaseRunner
{
public:
    StdAsyncRunner(const Manager* mgr, std::launch policy = std::launch::async);
    // TODO: destroy StdAsyncRunner on task end

    bool isAbortRequested() override;
    void requestAbort() override;

    void launch() override;

private:
    bool m_isAbortRequested;
    std::launch m_policy;
};

} // namespace Task
