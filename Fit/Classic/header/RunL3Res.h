#pragma once

#include <string>
#include "GlobalFlag.h"

/**
 * @brief Manages the global fitting process, including loading data, shapes, systematics,
 *        and running the fitter.
 *
 * Pre-requisites and post-processing details are documented in the source.
 */
class RunL3Res {
public:
    explicit RunL3Res(GlobalFlag& globalFlags);
    ~RunL3Res() = default;;

    void Run();

    // Delete copy and assignment operators.
    RunL3Res(const RunL3Res&) = delete;
    RunL3Res& operator=(const RunL3Res&) = delete;

private:
    // Reference to GlobalFlag instance
    GlobalFlag& globalFlags_;
};

