.. _basics:

First steps
###########

This sections demonstrates the basic features of kratos. Before getting
started, make sure that development environment is set up to run the included
tests.

Installing kratos
=================
kratos requires Python 3.6+. Please make sure that you have necessary Python
environment set up.

Linux/macOS/Windows
-------------------
You can simply do

.. code:: bash

  pip install kratos

This will download the pre-compiled binary wheel.

Other OS/ Compiling from source
-------------------------------
You need a C++-17 compatible compiler with ``<filesystem>`` enabled. g++-8
or clang++-8 should be sufficient. Also making sure that you have met the
following dependencies:

  - CMake

If your system doesn't come with a recent ``cmake``, you can install cmake
through your ``pip``:

.. code-block:: bash

    pip install cmake

Then you should be able to install using the following command

.. code-block:: bash

    git clone https://github.com/Kuree/kratos
    cd kratos
    git submodule init && git submodule update
    pip install .

Development setup
-----------------
You can simply do

.. code-block:: bash

    pip install -e .

Create a simple pass-through module
===================================

In kratos, every module is a live generator object. You can create a circuit
definition inside ``__init__()`` constructor, just as normal Python object.
This allows you to fuse configuration with the circuit itself, which can be
used to provide `single-source-of-truth` in hardware design.

.. code:: Python

    from kratos import *

    class PassThroughMod(Generator):
        def __init__(self):
            super().__init__("PassThrough", False)
            self.in_ = self.input("in", 1)
            self.out_ = self.output("out", 1)
            self.wire(self.out_, self.in_)


``super().__init__("PassThrough", False)`` tells the underlying system to
create a verilog module called ``PassThrough`` and we don't want debug
information (thus ``False``).

We now can go ahead and instantiate a path through module:

.. code:: Python

    mod = PassThroughMod()

To produce a system verilog file called "mod.sv" in the current working
directory, we can simply call a helper function

.. code:: Python

  verilog(mod, filename="mod.sv")

Looking at the content of ``mod.sv``, we can see the following system
verilog definition:

.. code-block:: SystemVerilog
    :linenos:

    module PassThrough (
      input logic  in,
      output logic  out
    );

    assign out = in;
    endmodule   // PassThrough

To see how debug works, we can modify the ``super()`` base class constructor
into

.. code:: Python

    super().__init__("PassThrough", True)

Now if we call ``verilog()`` with debug on, such as

.. code:: Python

    verilog(mod, filename="mod.sv", debug=True)

We will have an additional debug information file called ``mod.sv.debug`` in
the same directory as ``mod.sv``, which is a ``JSON`` file indexed by line
number.

.. code:: JSON

    {
      "1": [["/tmp/kratos/example.py", 3]],
      "2": [["/tmp/kratos/example.py", 4]],
      "3": [["/tmp/kratos/example.py", 5]],
      "6": [["/tmp/kratos/example.py", 6]]}

Put everything together
-----------------------

Here is an example that prints out the pass through module

.. code-block:: Python

    import kratos

    class PassThroughMod(kratos.Generator):
        def __init__(self):
            super().__init__("PassThrough", False)
            self.in_ = self.input("in", 1)
            self.out_ = self.output("out", 1)
            self.wire(self.out_, self.in_)


    mod = PassThroughMod()
    mod_src = kratos.verilog(mod)
    print(mod_src["PassThrough"])
