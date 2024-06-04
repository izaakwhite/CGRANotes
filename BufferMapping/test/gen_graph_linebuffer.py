import sys
import os
sys.path.insert(0,'..')
import json

from buffer_mapping.config import IR2Interface
from buffer_mapping.virtualbuffer import VirtualBuffer
from buffer_mapping.linebuffer import VirtualLineBuffer
from buffer_mapping.hardware import InputNode, OutputNode
from buffer_mapping.rewrite import regOptmization, banking, flattenValidBuffer
from buffer_mapping.mapping import CreateHWConfig

def test_linebuffer():
    dir_path = os.path.dirname(os.path.realpath(__file__))
    with open(dir_path+'/../config/test_linebuffer.json') as json_file:
        setup= json.load(json_file)
    HW_setup = setup["hw config"]
    BUFFERsetup = setup["buffer config"]
    mem_config = CreateHWConfig(setup["hw config"])
    #v_buf = CreateVirtualBuffer(setup["virtual buffer"])

    for key, IR_setup in BUFFERsetup.items():
        if key != "2D line buffer":
            continue
        v_setup = IR2Interface(IR_setup)
        v_buf = VirtualBuffer(v_setup._input_port,
                              v_setup._output_port,
                              v_setup._capacity,
                              v_setup._range,
                              v_setup._stride,
                              v_setup._start)

        #define what underline hw like
        hw_input_port = 1
        hw_output_port = 1
        linebuffer = VirtualLineBuffer(v_buf, hw_input_port, hw_output_port, IR_setup['capacity'], IR_setup['access_pattern']['stride_in_dim'])

        with open(dir_path + '/../config/conv_3_3_handcraft.json') as hand_craft_json:
            hand_craft = json.load(hand_craft_json)

        #TODO: make this a method, parse the input coreIR get the buffer module, get the input port and output port, and put them in a dictionary
        core = hand_craft["namespaces"]["global"]["modules"]["DesignTop"]
        instance = core["instances"]
        connection = core["connections"]
        def preprocessHandCraftJson():
            mul_list = []
            #remove reg and general buffer
            for key, value in list(instance.items()):
                if value.get("genref"):
                    if value["genref"] == "coreir.reg" or value["genref"] == "commonlib.unified_buffer":
                        core["connections"] = [wire for wire in core["connections"] if key not in wire[0] and key not in wire[1]]
                        del instance[key]

                #get multiplier list
                if "mul" in key:
                    mul_list.append([OutputNode(key, "in")])
            return mul_list
        mul_list = preprocessHandCraftJson()

        output_dict = {0:mul_list}
        #new_buffer_dict = linebuffer.dump_json("linebuffer", "self.in_arg_0_0_0", "self.in_en", reversed_mul_list)
        #instance.update(new_buffer_dict["instances"])
        #core["connections"].extend(new_buffer_dict["connections"])


        #data_in = HardwarePort("self.datain", 0)
        #valid = HardwarePort("self.inen", True)
        input_node = InputNode("self")
        node_dict, connection_dict = linebuffer.GenGraph("linebuffer", input_node, output_dict)

        #set of compiler pass optimize the graph
        node_dict, connection_dict = flattenValidBuffer(node_dict, connection_dict)
        node_dict, connection_dict = regOptmization(node_dict, connection_dict)
        #node_dict, connection_dict = banking(node_dict, connection_dict, mem_config)

        connection_list = [[key[0], key[1]] for key, _ in connection_dict.items()]

        node_list_dict = {}
        for key, node in node_dict.items():
            instance.update({node.name: node.dump_json()})
            element = {}
            if node.pred:
                element["pred"] = node.pred.name
            element["succ"] = [succ.name for succ in node.succ]
            node_list_dict.update({key: element})
        print (node_list_dict)
        print (connection_list)
        #print (instance)
        core["connections"].extend(connection_list)
        '''
        dump the generated coreIR file
        '''
        with open(dir_path + '/../config/lb_coreir.json' , 'w') as json_out_file:
            data = json.dumps(hand_craft, indent=4)
            json_out_file.write(data)

        for blockid in range(3):
            for i in range(v_setup._capacity // v_setup._input_port):
                tmp = [i*v_setup._input_port + j for j in range(v_setup._input_port)]
                #print (tmp)
                v_buf.write(tmp)
                valid, data_out = linebuffer.read_write(tmp)
                if valid:
                    data_out_ref = v_buf.read()
                    assert data_out_ref== data_out,\
                    "Data read is not matched, \nLine buffer read data ="+ str(data_out) + "\n, virtual buffer read data = " + str(data_out_ref)
                    #print (data_out_ref)
            #print("Finish read all data from line buffer, move to the next tile.")
        print("Finish test for", key)

if __name__ == '__main__':
    test_linebuffer()
