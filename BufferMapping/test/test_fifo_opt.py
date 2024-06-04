import sys
import os
sys.path.insert(0,'..')
import json

from buffer_mapping.config import IR2Interface
from buffer_mapping.virtualbuffer import VirtualBuffer
from buffer_mapping.linebuffer import VirtualLineBuffer

def test_linebuffer():
    dir_path = os.path.dirname(os.path.realpath(__file__))
    with open(dir_path+'/../config/testIR.json') as json_file:
        setup= json.load(json_file)
    #v_setup = setup["virtual buffer"]
    #v_buf = CreateVirtualBuffer(setup["virtual buffer"])

    IR_setup = setup['strided line buffer']
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

    for blockid in range(3):
        for i in range(v_setup._capacity // v_setup._input_port):
            tmp = [i*v_setup._input_port + j for j in range(v_setup._input_port)]
            #print (tmp)
            v_buf.write(tmp)
            valid, data_out = linebuffer.read_write(tmp)
            #print (valid, data_out)
            if valid:
                data_out_ref = v_buf.read()
                assert data_out_ref == data_out,\
                "Data read is not matched, \nLine buffer read data ="+ str(data_out) + "\n, virtual buffer read data = " + str(data_out_ref)
        print("Finish read all data from line buffer, move to the next tile.")
        linebuffer.reset()


if __name__ == '__main__':
    test_linebuffer()
