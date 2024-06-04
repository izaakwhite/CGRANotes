#ifndef HALIDE_COREIR_COMPUTE_H
#define HALIDE_COREIR_COMPUTE_H

/** \file
 * Defines the lowering pass that unrolls loops marked as such
 */

#include "IR.h"

#include "coreir.h"

namespace Halide {
namespace Internal {

struct CoreIR_Port {
  std::string name;
  uint bitwidth;
  bool is_dynamic;
};

struct CoreIR_PortBundle {
  std::string name;
  uint total_bitwidth;
  std::vector<CoreIR_Port> ports;
};

struct CoreIR_Interface {
  std::string name;
  CoreIR_Port output;
  std::vector<CoreIR_PortBundle> inputs;
  std::vector<CoreIR_Port> indices; // x, y, or c used in computation
};

/** Take a statement with for loops marked for unrolling, and convert
 * each into several copies of the innermost statement. I.e. unroll
 * the loop. */
//CoreIR::ModuleDef* convert_compute_to_coreir(Stmt, CoreIR_Interface);
//CoreIR::Type* interface_to_type(CoreIR_Interface);
//std::map<std::string, std::string> coreir_generators(CoreIR::Context);
void convert_compute_to_coreir(Expr, CoreIR_Interface, CoreIR::Context*);


struct CoreIR_Inst_Args {
  std::string ref_name = "";
  std::string name;
  std::string gen;
  CoreIR::Values args;
  CoreIR::Values genargs;
  std::string wirename;
  std::string selname;

  CoreIR_Inst_Args() {}
  CoreIR_Inst_Args(std::string name, std::string wirename, std::string selname,
                   std::string gen, CoreIR::Values args, CoreIR::Values genargs) :
    name(name), gen(gen), args(args), genargs(genargs), wirename(wirename), selname(selname) {}
};

CoreIR_Inst_Args rom_to_coreir(std::string, std::vector<int>, Stmt, CoreIR::Context*);

void convert_compute_to_coreir(Expr, CoreIR_Interface, std::vector<CoreIR_Inst_Args>, CoreIR::Context*);


}  // namespace Internal
}  // namespace Halide

#endif
