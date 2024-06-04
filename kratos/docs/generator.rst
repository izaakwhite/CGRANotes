.. _generator:

Generator of Generators
#######################

Kratos follows the idea of `generators of generators`. Every class
inherits from ``kratos.Generator`` class is a generator which can
generate different circuit based on different parameters. To push
the generator idea even further, you can modify the circuit even
after being instantiated from a generator class. The backend engine
can take of the additional changes.

There are two ways to populate circuit definition to the generator:

1. Free-style code block
2. Procedural code generation

Each approach has its own strengths and drawbacks. In general,
using code block makes the Python code more readable and
procedural code generation is more flexible and universal. If you
look at the source code, free-style code block is just a helper
to construct circuit definition procedurally.

Port and Variables
==================
As in verilog, we need to declare ports and variables before we can
describe the circuit logic. To declare a port, you can call the
``port`` function from the generator:

.. code-block:: Python

    def port(self, name: str, width: int, direction: PortDirection,
             port_type: PortType = PortType.Data,
             is_signed: bool = False, size: int = 1,
             packed: bool = False)

``PortDirection`` and ``PortType`` are enums to specify the types of
the port we're creating. The definitions for these enum are:

.. code-block:: Python

    class PortDirection(enum.Enum):
        In = _kratos.PortDirection.In
        Out = _kratos.PortDirection.Out
        InOut = _kratos.PortDirection.InOut


    class PortType(enum.Enum):
        Data = _kratos.PortType.Data
        Clock = _kratos.PortType.Clock
        AsyncReset = _kratos.PortType.AsyncReset
        ClockEnable = _kratos.PortType.ClockEnable
        Reset = _kratos.PortType.Reset

Notice that ``_kratos`` is a namespace that came from the native C++ binding.
kratos also has some helper functions to help you create commonly used ports:

.. code-block:: Python

    def input(self, name, width, port_type: PortType = PortType.Data,
              is_signed: bool = False, size: int = 1, packed: bool = False)

    def output(self, name, width, port_type: PortType = PortType.Data,
               is_signed: bool = False, size: int = 1, packed: bool = False)

    def clock(self, name, is_input=True)

    def reset(self, name, is_input=True, is_async=True)

To declare a variable, simply call the following function to create one:

.. code-block:: Python

    def var(self, name: str, width: int,
             is_signed: bool = False)

kratos will do type checking for ``width``, ``port_type``, and ``is_signed``
to avoid any implicit conversion that could be difficult to detect.

Variable Proxies
----------------
For simple modules, it is fine to hold a port/variable/parameters as class
attributes. However, as the generator gets more complicated, it may be
difficult to maintain all the variable names. kratos ``Generator`` comes
with handy proxies to access all the variables you need. You can access a
port either through

- ``[gen].ports.port_name``
- ``[gen].ports["port_name"]``

where ``[gen]`` is any generator instance and ``port_name`` is the name you
want to access. Similarly, you can use

- ``[gen].vars.port_name``
- ``[gen].vars["port_name]``

and ``[gen].params`` for parameters.

Expressions
-----------

Whenever we perform some arithmetic or logic operator on port/variables, we
implicitly create an expression. An expression can be assigned to a port or
a variable. It can also be composed together to form more complex expressions.

.. code-block:: pycon

    >>> from kratos import *
    >>> g = Generator("mod")
    >>> a = g.var("a", 1)
    >>> b = g.var("b", 1)
    >>> c = a + b
    >>> c
    a + b
    >>> d = c + c
    >>> d
    (a + b) + (a + b)

To avoid conflicts with python built-in functions, some verilog operators
are not directly implemented as operator overloads in Python:

1. ``eq()`` for logical comparison
2. ``ashr()`` for signed arithmetic shift right.

Constants
---------

Kratos's Python front-end will try it's best to do conversation, when it
deeds type-safe. For instance, you can directly adding python integers to
a Kratos variable, or simply ``wire(a, 1)``, where ``a`` is a Kratos variable.
However, if due to either type checking or some special
cases where the type conversation doesn't work, you can call ``const``
function to specify the constant value. The rule of thumb is to use
explicit constant call as much as possible and only use the implicit
conversion when you know it's going to be safe. Here is the ``const``
function definition

.. code-block:: Python

  def const(value: int, width: int, signed: bool = False)

.. note::

  Say we have ``var + num``, where ``var`` is either a port or variable
  and ``num`` is a python integer. The rules of implicit conversation is
  1. The converted constant will have the same width as the ``var``
  2. The converted constant will have the same sign as the ``var``.

  If the value is out of range, an exception will thrown and you have to
  use either concatenate or slice the left hand side.

Arrays
------

ND Array is supported in kratos. You can create an array though the
``Generator.var()``
function call and set the ``size`` to the array size in a similar way as
``numpy`` array. For instance, if ``a.size = (3, 4, 5)``, then ``a[0]`` will
have size ``(4, 5)`` and ``a[0][0]`` will have size ``5``.

.. code-block:: Python

      def var(self, name: str, width: Union[int, _kratos.Param, _kratos.Enum],
            is_signed: bool = False, size: Union[int, Union[List, Tuple]] = 1,
            packed: bool = False, explicit_array: bool = False)

.. note::

  By default, kratos creates unpacked array to allow better error/warning
  checking in downstream tools. However, users can override this by setting
  ``packed=True`` during the variable or port construction.

Enums
-----

Enums are type-checked and compiled directly into SystemVerilog ``enum``
directly. Currently are limited to generate scope and cannot be shared
across different generators. This many change in the future.

To use enum, first you need to define an enum definition, then you create
enum variables based on the enum definition. You can only assign enum
values to these enum variables:

.. code-block:: Python

     mod = Generator("mod")
     state = mod.enum("STATE", {"IDLE": 0, "READY": 1})
     v = mod.enum_var("v", state)
     mod.add_stmt(v.assign(state.IDLE))

This gives us:

.. code-block:: SystemVerilog

     module mod (
     );

     typedef enum logic {
       IDLE = 1'h0,
       READY = 1'h1
     } STATE;
     STATE   v;
     assign v = IDLE;
     endmodule   // mod

If you want Kratos automatically assign enum values for you, you
can pass in a list of enum names:

.. code-block:: Python

    mod.enum("STATE", ["IDLE", "READY])

Kratos will assign values and compute the minimum width.

Notice that you can also have global enum as well, which will be
put into a SystemVerilog package when generated using ``output_dir``.
To create a global enum, simply do

.. code-block:: SystemVerilog

   state = enum("STATE", ["IDLE", "READY"])

You can global enum as your port types as well. For instance, you can
do the following:

.. code-block:: Python

  inst_def = enum("INST", ["Add","Sub", "Mult", "Div"])
  inst = mod.input("inst", inst_def)

This will be generated as:

.. code-block:: SystemVerilog

  typedef enum logic[1:0] {
    Add = 2'h0,
    Sub = 2'h1,
    Mult = 2'h2,
    Div = 2'h3
  } INST;

  ...
    input INST inst,
  ...

.. warning::
  If you use local enum (defined from the generator class) as port type,
  you will get an exception during compilation.

Child generators
================

You can use ``add_child(inst_name, child)`` or ``add_child_generator``
to add a child generator. The ``inst_name`` is the instance name for that
child generator and has to be unique within the parent scope. After adding
the child generator to the parent scope, you can access the child
generator through `self[inst_name]` method. ``__getitem__()``
has been overloaded to get the child.

This is a required step to properly instantiate the sub modules.

In addition to use ``wire()`` function to obtain the desired the connection
between the parent and child, you can also pass the connection when adding
the child generator, similar to how you instantiate a child instance in
RTL:

.. code-block:: Python

    parent = Generator("parent")
    child = Generator("child")
    a = parent.var("a", 1)
    parent.var("b", 1)
    parent.output("c", 1)
    child_in = child.input("in_port", 1)
    child_out1 = child.output("out_port1", 1)
    child_out2 = child.output("out_port2", 1)
    # we use the port name to instantiate the connection
    # due to Python's limitation, we can use keyword such as "in"
    # the syntax is child_port_name=parent_port
    # where parent port can be either port name or port variable
    parent.add_child("child_inst", child,
                     in_port=a, out_port1="b", out_port2="c")


As stated in comment section, it uses Python's ``kwargs`` to pass in child
port names and parent port. Since child port names are used as keywords, they
cannot be parameterized like format string or list array, which is the
limitation. If that is the case, we can always use ``wire`` function.

.. note::

  You can pass comment to the ``add_child`` as additional argument. The
  comment will show up in the generated SystemVerilog to help you
  debug.

External Modules
================
kratos allows you to create either an external module or an stub.

External module
---------------
External modules are created from verilog source. You can call
``Generator.from_verilog`` to import verilog files. You need to
provide the port type mapping to alow the type checking to work
properly.

.. code-block:: Python

    def from_verilog(top_name: str, src_file: str, lib_files: List[str],
                        port_mapping: Dict[str, PortType]):

``lib_files`` lets you import related verilog files at once so
you don't have to copy these files over.

Stub module
-----------
Sometimes you're dealing with IPs while working on an open-source
project, you can create a stub that mimics the IP interface but
produce junk output. kratos provides helper methods to do that.
All you need to do is to set the module as a stub after declaring
the interface. ``self.is_stub = True``. The backend engine will
zero out the outputs for you.

Free-Style Code Block
=====================
kratos allows to write Genesis2 style verilog code inside Python (to
some extent). The basic principle is that if a Python expression can
be evaluated as integer or boolean, the compiler will be happy to do
so. If the Python code results in a kratos expression, the compiler
will leave it as is in the verilog.

Allowed python control flows that will be statically evaluated:

1. ``for``
2. ``if``
3. class function calls that returns a single statement

.. note::

  In some cases, kratos will convert your ``for`` loop into standard
  SystemVerilog ``for`` loop to improve readability. This conversion
  only happens when all the following conditions match at the same time:

  1. The current generator is not in debug mode
  2. All the variables that the iterator indexed into are kratos variable

Keywords like ``while`` may or may not work depends on how it is nested
side other statements.

Please also notice that kratos don't allow ``generate`` statement in
verilog, so the for loop range has to be statically determined,
otherwise a ``SyntaxError`` will be thrown.

To add a code block to the generator definition, you need to wrap the
code block into a class method with only `self` as argument, then call
``[gen].add_always([func])`` to add the code block, where ``func`` is the
function wrapper.

Combinational and Sequential Code Block
---------------------------------------

If you need to add a sequential code block that depends on some signals,
you need to decorate the function wrapper with ``always_ff`` and sensitivity
list. The list format is ``List[Tuple[EdgeType, str]]``, where the
``EdgeType`` can be either ``BlockEdgeType.Posedge`` or
``BlockEdgeType.Negedge``. The ``str`` has be either a port or variable
name. For instance, the code below will produce a code block that listens
to ``clk`` and ``rst`` signal. Notice that if you do ``from kratos import
*``, you can use ``posedge`` or ``negedge`` directly.

.. code-block:: Python

    @always_ff((posedge, "clk"), (posedge, "rst"))
    def seq_code_block(self):
        # code here

For combinational block, you need to decorate the function with
``always_comb``.

.. code-block:: Python

    @always_comb
    def comb_code(self):
        # code here

..warning::

   You are not allowed to call these functions directly since they are
   not treated as function calls. As a result, you will get a syntax
   error.

Examples
--------
Here are some examples the free-style code block in kratos that uses both
combinational and sequential block. Of course you can write it in a more
concise way: this is just an example of how to add code blocks.

.. code-block:: Python

    class AsyncReg(Generator):
        def __init__(self, width):
            super().__init__("register")

            # define inputs and outputs
            self._in = self.input("in", width)
            self._out = self.output("out", width)
            self._clk = self.clock("clk")
            self._rst = self.reset("rst", 1)
            self._val = self.var("val", width)

            # add combination and sequential blocks
            self.add_always(self.seq_code_block)

            self.add_always(self.comb_code_block)

        @always_ff((posedge, "clk"), (posedge, "rst"))
        def seq_code_block(self):
            if self._rst:
                self._val = 0
            else:
                self._val = self._in

        @always_comb
        def comb_code_block(self):
            self._out = self._val

Here is the verilog produced:

.. code-block:: pycon

  >>> reg = AsyncReg(16)
  >>> mod_src = verilog(reg)
  >>> print(mod_src["register"]

.. code-block:: SystemVerilog

  module register (
    input logic  clk,
    input logic [15:0] in,
    output logic [15:0] out,
    input logic  rst
  );

  logic  [15:0] val;

  always @(posedge rst, posedge clk) begin
    if (rst) begin
      val <= 16'h0;
    end
    else begin
      val <= in;
    end
  end
  always_comb begin
    out = val;
  end
  endmodule   // register

If you found the example above verbose, you can put everything inside
``__init__`` to avoid typing ``self`` over and over again. To produce
the identical verilog, you can use scoped functions to reuse the
variables created before:

.. code-block:: Python

  class AsyncReg(Generator):
      def __init__(self, width):
          super().__init__("register")

          # define inputs and outputs
          _in = self.input("in", width)
          _out = self.output("out", width)
          _clk = self.clock("clk")
          _rst = self.reset("rst", 1)
          _val = self.var("val", width)

          @always_ff((posedge, "clk"), (posedge, "rst"))
          def seq_code_block():
              if _rst:
                  _val = 0
              else:
                  _val = _in

          @always_comb
          def comb_code_block():
              _out = _val

          # add combination and sequential blocks
          self.add_always(seq_code_block)
          self.add_always(comb_code_block)

Here is another example on `for` static evaluation when the debug
mode is ``True``:

.. code-block:: Python

    class PassThrough(Generator):
        def __init__(self, num_loop):
            super().__init__("PassThrough", True)
            self.in_ = self.input("in", 1)
            self.out_ = self.output("out", num_loop)
            self.num_loop = num_loop

            self.add_always(self.code)

        @always_comb
        def code(self):
            if self.in_ == 1:
                for i in range(self.num_loop):
                    self.out_[i] = 1
            else:
                for i in range(self.num_loop):
                    self.out_[i] = 0

Here is the generated verilog

.. code-block:: pycon

    >>> a = PassThrough(4)
    >>> mod_src = verilog(a)
    >>> print(mod_src["PassThrough"])

.. code-block:: SystemVerilog

  module PassThrough (
    input logic  in,
    output logic [3:0] out
  );

  always_comb begin
    if (in == 1'h1) begin
      out[0:0] = 1'h1;
      out[1:1] = 1'h1;
      out[2:2] = 1'h1;
      out[3:3] = 1'h1;
    end
    else begin
      out[0:0] = 1'h0;
      out[1:1] = 1'h0;
      out[2:2] = 1'h0;
      out[3:3] = 1'h0;
    end
  end
  endmodule   // PassThrough

Here is the SystemVerilog when the debug mode is off:

.. code-block:: SystemVerilog

    module PassThrough (
      input logic in,
      output logic [3:0] out
    );

    always_comb begin
      if (in == 1'h1) begin
        for (int unsigned i = 0; i < 4; i += 1) begin
            out[2'(i)] = 1'h1;
          end
      end
      else begin
        for (int unsigned i = 0; i < 4; i += 1) begin
            out[2'(i)] = 1'h0;
          end
      end
    end
    endmodule   // PassThrough


Procedural code generation
==========================

Sometimes it is very difficult to generate a desired circuit definition through
limited free-style code block. If that is the case, you can use the procedural
code generation.

The main idea here is to construct verilog statement in a hierarchical way. The
hierarchy is defined by verilog's ``begin ... end`` closure. Here are a list
of statements you can construct:

- ``SequentialCodeBlock``
- ``CombinationalCodeBlock``
- ``SwitchStmt``
- ``IfStmt``
- ``AssignStmt``
- ``ForStmt``


.. note::
    kratos provides a helper function called `wire(var1, var2)` that wires
    things together in the top level. In most cases the ordering does matter:
    it's the same as ``assign var1 = var2;``. The only exception is when one
    of them is a port (not port slice though).

Syntax sugars
-------------

Kratos' Python front-end provides a concise front-end to create these blocks.
``SequentialCodeBlock`` can be constructed through ``[gen].sequential()`` and
``CombinationalCodeBlock`` can be constructed through
``[gen].combinational()``.
You can create a ``SwitchStmt`` through either ``[comb].switch_`` or
``[seq].switch_``. Similarly, you can get a ``IfStmt`` through either
``[comb].if_`` or ``[seq].if_``. For more details, please check out this
`link`_

.. _link: https://github.com/Kuree/kratos/blob/master/kratos/stmts.py

To create an assignment, you can just use a normal function call to the
variable/port, such as ``[gen].var.var_name(value)``, where the ``value``
can be either a variable/port/const or integer values (with implicit
conversation).

Examples
--------

Here is an example on how to build a ``case`` based N-input mux.

.. code-block:: Python

    class Mux(Generator):
        def __init__(self, height: int, width: int):
            name = "Mux_{0}_{1}".format(width, height)
            super().__init__(name)

            # pass through wires
            if height == 1:
                self.in_ = self.input("I", width)
                self.out_ = self.output("O", width)
                self.wire(self.out_, self.in_)
                return

            self.sel_size = clog2(height)
            input_ = self.input(I, width, size=height)
            self.out_ = self.output("O", width)
            self.input("S", self.sel_size)

            # add a combinational block
            comb = self.combinational()

            # add a case statement
            switch_ = comb.switch_(self.ports.S)
            for i in range(height):
                switch_.case_(i, self.out_(input_[i]))
            # add default
            switch_.case_(None, self.out_(0))

Here is the generated verilog

.. code-block:: SystemVerilog

  module Mux_16_3 (
    input logic [15:0] I [2:0],
    output logic [15:0] O,
    input logic [1:0] S
  );

  always_comb begin
    case (S)
      default: O = 16'h0;
      2'h0: O = I[0];
      2'h1: O = I[1];
      2'h2: O = I[2];
    endcase
  end
  endmodule   // Mux_16_3
