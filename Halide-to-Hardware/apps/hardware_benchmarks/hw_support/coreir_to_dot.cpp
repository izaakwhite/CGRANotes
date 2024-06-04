#include "coreir/libs/commonlib.h"
#include "coreir/libs/float.h"
#include "lakelib.h"
#include "coreir/passes/transform/rungenerators.h"
#include "cgralib.h"

using namespace std;
using namespace CoreIR;

void coreir_to_dot(std::string coreir_design_filename,
                   std::string output_dot_filename) {
  
  // New context for coreir test
  Context* context = newContext();
  Namespace* g = context->getGlobal();

  CoreIRLoadLibrary_commonlib(context);
  CoreIRLoadLibrary_lakelib(context);
  CoreIRLoadLibrary_float(context);
  CoreIRLoadLibrary_cgralib(context);

  Module* top;
  if (!loadFromFile(context, coreir_design_filename, &top)) {
    cout << "Could not load " << coreir_design_filename
         << " from json!!" << endl;
    context->die();
  }

  //cout << "design top exists=" << context->hasTop() << endl;
  //Module* design = g->getModule("DesignTop");
  Module* design = top;
  assert(design != nullptr);

  if (!saveToDot(design, output_dot_filename)) {
    cout << "Could not save to dot!!" << endl;
    context->die();
  }
}

int main(int argc, char **argv) {
  if (argc != 3) {
    cout << "Usage:\n"
         << "  coreir_to_dot [design].json [design].txt"
         << endl;
    return 1;
  }

  coreir_to_dot(argv[1], argv[2]);
  return 0;
}
