#include "coreir.h"
#include "coreir/simulator/interpreter.h"
#include "coreir/libs/commonlib.h"
#include "coreir/libs/float.h"
#include "lakelib.h"

#include "Halide.h"

#include "halide_image_io.h"
#include <stdio.h>
#include <iostream>
#include <stdexcept>

#include <fstream>
#include "test_utils.h"
#include "coreir_utils.h"

#include "ubuf_coreirsim.h"

using namespace CoreIR;
using namespace Halide;
using namespace Halide::Tools;
using namespace std;

std::string exec(const char* cmd) {
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (fgets(buffer, sizeof buffer, pipe) != NULL) {
            result += buffer;
        }

    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}

void  ubuffer_conv_3_3_reduce_test() {
  //ImageParam input(type_of<uint8_t>(), 2);
  //Func output;

  //Var x("x"), y("y");

  //Func kernel("kernel");
  //Func conv("conv");
  //RDom r(0, 3,
      //0, 3);

  //kernel(x,y) = 0;
  //kernel(0,0) = 11;      kernel(0,1) = 12;      kernel(0,2) = 13;
  //kernel(1,0) = 14;      kernel(1,1) = 0;       kernel(1,2) = 16;
  //kernel(2,0) = 17;      kernel(2,1) = 18;      kernel(2,2) = 19;

  //conv(x, y) = 0;

  //Func hw_input("hw_input");
  //hw_input(x, y) = cast<uint16_t>(input(x, y));
  //conv(x, y)  += kernel(r.x, r.y) * hw_input(x + r.x, y + r.y);

  //Func hw_output("hw_output");
  //hw_output(x, y) = cast<uint8_t>(conv(x, y));
  //output(x, y) = hw_output(x,y);

  //Var xi,yi, xo,yo;

  //hw_input.compute_root();
  //hw_output.compute_root();

  //int inTileSize = 4;
  //int outTileSize = inTileSize - 2;

  //hw_output.bound(x, 0, outTileSize);
  //hw_output.bound(y, 0, outTileSize);

  //output.bound(x, 0, outTileSize);
  //output.bound(y, 0, outTileSize);

  //// Creating input data
  //Halide::Buffer<uint8_t> inputBuf(4, 4);
  //Halide::Runtime::Buffer<uint8_t> hwInputBuf(4, 4, 1);
  //for (int i = 0; i < 4; i++) {
    //for (int j = 0; j < 4; j++) {
      //for (int b = 0; b < 1; b++) {
        //inputBuf(i, j, b) = i + j*2;
        //hwInputBuf(i, j, b) = inputBuf(i, j, b);
      //}
    //}
  //}

  //// Creating CPU reference output
  //Halide::Buffer<uint8_t> cpuOutput(2, 2);
  //ParamMap rParams;
  //rParams.set(input, inputBuf);
  //Target t;
  //hw_output.realize(cpuOutput, t, rParams);

  //Halide::Runtime::Buffer<uint8_t> outputBuf(2, 2, 1);

  //int tileSize = 4;
  //hw_output.tile(x,y, xo,yo, xi,yi, tileSize-2, tileSize-2)
    //.hw_accelerate(xi, xo);

  //kernel.compute_at(hw_output, xo)
    //.unroll(x).unroll(y);
  //conv.linebuffer();

  //hw_input.stream_to_accelerator();

  //// Generate CoreIR
  //auto context = hwContext();
  //vector<Argument> args{input};
  //buildModule(context, "coreir_conv_3_3", args, "conv_3_3", hw_output);

  //// Get unified buffer fie
  //if (!loadFromFile(context, "./ubuffers.json")) {
    //cout << "Error: Could not load json for ubuffer test!" << endl;
    //context->die();
  //}
  //context->runPasses({"rungenerators", "flattentypes", "flatten", "wireclocks-coreir"});
  //CoreIR::Module* m = context->getNamespace("global")->getModule("hw_input_ubuffer");
  //cout << "hw_input_ubuffer..." << endl;
  //m->print();

  //SimulatorState state(m);
  //state.setValue("self.write_port_0", BitVector(16, 0));
  //state.setValue("self.write_port_0_en", BitVector(1, 0));
  //state.setClock("self.clk", 0, 1);
  //state.setValue("self.reset", BitVector(1, 1));
  //state.resetCircuit();
  //state.setValue("self.reset", BitVector(1, 0));

  //assert(state.getBitVec("self.read_port_0_valid") == BitVec(1, 0));

  //int n_valids = 0;
  //vector<int> port_0_values;
  //map<int, vector<int> > port_vals;
  //for (int t = 0; t < inTileSize*inTileSize; t++) {
    //state.setValue("self.write_port_0", BitVector(16, t));
    //state.setValue("self.write_port_0_en", BitVector(1, 1));

    //state.exeCombinational();
    //if (state.getBitVec("self.read_port_0_valid") == BitVec(1, 1)) {
      //port_0_values.push_back(state.getBitVec("self.read_port_0").to_type<int>());
      //for (int i = 0; i < 9; i++) {
        //port_vals[i].push_back(state.getBitVec("self.read_port_" + to_string(i)).to_type<int>());
      //}
      //n_valids++;
    //}
    //state.execute();
  //}

  ////cout << "n_valids = " << n_valids << endl;
  ////assert(n_valids == 2*2);
  ////assert(port_0_values.size() == 2*2);

  ////for (int i = 0; i < port_0_values.size(); i++) {
    ////cout << "\toutput = " << port_0_values.at(i) << endl;
  ////}
  ////vector<int> correct{0, 1, 4, 5};
  ////assert(port_0_values == correct);

  ////cout << "--- Port values" << endl;
  ////for (int p = 0; p < 9; p++) {
    ////cout << "Port: " << p << endl;
    ////for (size_t i = 0; i < port_vals[p].size(); i++) {
      ////cout << "\t\toutput = " << port_vals[p].at(i) << endl;
    ////}
  ////}
  //deleteContext(context);

  //cout << GREEN << "UBuffer to linebuffer for conv rolled 3x3 test passed" << RESET << endl;
  //assert(false);
}

void ubuffer_small_conv_3_3_test() {
  /*ImageParam input(type_of<uint8_t>(), 2);
  ImageParam output(type_of<uint8_t>(), 2);

  Var x("x"), y("y");

  Func kernel("kernel");
  Func conv("conv");
  RDom r(0, 3,
      0, 3);

  kernel(x,y) = 0;
  kernel(0,0) = 11;      kernel(0,1) = 12;      kernel(0,2) = 13;
  kernel(1,0) = 14;      kernel(1,1) = 0;       kernel(1,2) = 16;
  kernel(2,0) = 17;      kernel(2,1) = 18;      kernel(2,2) = 19;

  conv(x, y) = 0;

  Func hw_input("hw_input");
  hw_input(x, y) = cast<uint16_t>(input(x, y));
  conv(x, y)  += kernel(r.x, r.y) * hw_input(x + r.x, y + r.y);

  Func hw_output("hw_output");
  hw_output(x, y) = cast<uint8_t>(conv(x, y));
  output(x, y) = hw_output(x,y);

  Var xi,yi, xo,yo;

  hw_input.compute_root();
  hw_output.compute_root();


  hw_output.bound(x, 0, outTileSize);
  hw_output.bound(y, 0, outTileSize);

  //output.bound(x, 0, outTileSize);
  //output.bound(y, 0, outTileSize);

  // Creating input data
  Halide::Buffer<uint8_t> inputBuf(4, 4);
  Halide::Runtime::Buffer<uint8_t> hwInputBuf(4, 4, 1);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      for (int b = 0; b < 1; b++) {
        inputBuf(i, j, b) = i + j*2;
        hwInputBuf(i, j, b) = inputBuf(i, j, b);
      }
    }
  }

  // Creating CPU reference output
  Halide::Buffer<uint8_t> cpuOutput(2, 2);
  ParamMap rParams;
  rParams.set(input, inputBuf);
  Target t;
  hw_output.realize(cpuOutput, t, rParams);

  Halide::Runtime::Buffer<uint8_t> outputBuf(2, 2, 1);

  int tileSize = 4;
  hw_output.tile(x,y, xo,yo, xi,yi, tileSize-2, tileSize-2)
    .hw_accelerate(xi, xo);

  kernel.compute_at(hw_output, xo)
    .unroll(x).unroll(y);

  conv.update()
    .unroll(r.x, 3)
    .unroll(r.y, 3);
  conv.linebuffer();

  hw_input.stream_to_accelerator();
*/
  int inTileSize = 64;
  int outTileSize = inTileSize - 2;
  int tileSize = 64;
  // Generate CoreIR
  auto context = hwContext();
  //vector<Argument> args{input};
  //buildModule(context, "coreir_conv_3_3", args, "conv_3_3", hw_output);

  //A hack using JEFF's generator
  system("(cd ../conv_3_3/ && make)");
  string run_generator_cmd = "(cd ../conv_3_3/ && make design-coreir > generator.log)";
  const char* cmd = run_generator_cmd.c_str();
  auto info = exec(cmd);
  std::cout << info << endl;
  system("cd ../merged_unit_tests/");
  system("cp ../conv_3_3/ubuffers.json .");

  // Get unified buffer fie
  if (!loadFromFile(context, "./ubuffers.json")) {
    cout << "Error: Could not load json for ubuffer test!" << endl;
    context->die();
  }
  context->runPasses({"rungenerators", "flattentypes", "flatten", "wireclocks-coreir"});
  CoreIR::Module* m = context->getNamespace("global")->getModule("hw_input_ubuffer");
  cout << "hw_input_ubuffer..." << endl;
  m->print();

  auto ubufBuilder = [](WireNode& wd) {
    //UnifiedBuffer* ubufModel = std::make_shared<UnifiedBuffer>(UnifiedBuffer()).get();
    UnifiedBuffer_new* ubufModel = new UnifiedBuffer_new();
    return ubufModel;
  };


  cout << "----------------simulator start-----------------"<<endl;
  map<std::string, SimModelBuilder> qualifiedNamesToSimPlugins{{string("lakelib.unified_buffer"), ubufBuilder}};

  SimulatorState state(m, qualifiedNamesToSimPlugins);

  state.setValue("self.write_port_0", BitVector(16, 0));
  state.setValue("self.write_port_0_en", BitVector(1, 0));
  state.setClock("self.clk", 0, 1);
  state.setValue("self.reset", BitVector(1, 1));
  cout << "begin the reset\n";
  state.resetCircuit();
  state.setValue("self.reset", BitVector(1, 0));


  assert(state.getBitVec("self.read_port_0_valid") == BitVec(1, 0));

  int n_valids = 0;
  vector<vector<int> > port_values(9, vector<int>(outTileSize*outTileSize, 0));
  for (int t = 0; t < inTileSize*inTileSize; t++) {
    //cout << "At position: " << t << endl;
    state.setValue("self.write_port_0", BitVector(16, t));
    state.setValue("self.write_port_0_en", BitVector(1, 1));

    //state.exeCombinational();
    //FIXME: discuss with Dillon about the timing
    state.exeCombinational();
    for (size_t port_num = 0; port_num <9; port_num++ ){
        if (state.getBitVec("self.read_port_"+to_string(port_num)+"_valid") == BitVec(1, 1)) {
            port_values[port_num][n_valids] = state.getBitVec("self.read_port_"+to_string(port_num)).to_type<int>();
            if (port_num == 8)
                n_valids ++;
        }
    }
    state.exeSequential();
    //cout << "n_valids = " << n_valids << endl;
  }

  cout << "n_valids = " << n_valids << endl;
  assert(n_valids == outTileSize*outTileSize && "Valid count is not matched");

  //for (int i = 0; i < port_0_values.size(); i++) {
    //cout << "\toutput = " << port_0_values.at(i) << endl;
  //}
  //vector<int> correct{0, 1, 4, 5};
  //assert(port_0_values == correct);

  cout << "--- Check Port values ---" << endl;
  for (int row = 0; row < 62; row++) {
  for (int col = 0; col < 62; col++) {
      for (size_t p = 0; p < 9; p++) {
          int px = p % 3;
          int py = p / 3;
          int out_pos = row*62 + col;
          int target = (col + px) + (row + py) * 64;
          //cout << "Port: " << p << ", val: " << port_values[p][out_pos] << endl;
          if (port_values[p][out_pos] != target) {
              cout << "Conv result not match at coordination [" << col << ", " << row <<"], port [" << px << ", "<< py <<  "]\n expect val=" << target <<", but get val =" << port_values[p][out_pos] <<endl;
              assert(false);
          }
      }
  }
  }
  deleteContext(context);

  cout << GREEN << "UBuffer to linebuffer for conv 3x3 test passed" << RESET << endl;
  //assert(false);
}

