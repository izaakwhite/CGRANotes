#ifndef HALIDE_INSERT_HW_BUFFERS_H
#define HALIDE_INSERT_HW_BUFFERS_H

/** \file
 *
 * Defines the streaming optimization pass
 */

#include "IR.h"
#include "ExtractHWBuffers.h"

namespace Halide {
namespace Internal {

/** Perform streaming optimization
 */
Stmt insert_hwbuffers(Stmt s, const HWXcel &dag);

}
}

#endif
