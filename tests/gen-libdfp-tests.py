#!/usr/bin/env python

import sys
import re
from optparse import OptionParser

# Decribes the evaluated function
class Function:
  def __init__ (self):
    self.name = None
    self.args = []
    self.ret = None

  def ret_init_type (self):
    return self.ret.argtype

  def ret_type (self):
    return self.ret.type

  def ret_field (self):
    if self.ret.decfield:
      return "." + self.ret.decfield
    return ""

  def ret_printf (self):
    return self.ret.printf

  def arg_type (self, i):
    return self.args[i].argtype;


# Describes an operation to perform
class Operation:
  def __init__(self, line):
    self.args = []
    self.ret = None
    self.line = line

# Describe a type handle in operations and functions definition
class Type (object):
  def __init__(self, name, type, printf):
    self.name = name
    self.type = type
    self.printf = printf
    self.decfield = ""

  def parse_arg(self, arg):
    return arg


class BoolType(Type):
  def __init__(self, name, type, printf):
    super(BoolType, self).__init__(name, type, printf)
    self.argtype = type

  def parse_arg (self, arg):
    if arg == "True":
      return 1
    elif arg == "False":
      return 0
    return arg;


class IntegerType(Type):
  def __init__(self, name, type, printf):
    super(IntegerType, self).__init__(name, type, printf)
    self.argtype = type

  def parse_arg (self, arg):
    if "DEC_MIN_EXP_SUB" in arg:
      return arg.replace ("DEC_MIN_EXP_SUB", DECIMAL.minexpsub)
    if "DEC_MIN_EXP" in arg:
      return arg.replace ("DEC_MIN_EXP", DECIMAL.minexp)
    if "DEC_MAX_EXP" in arg:
      return arg.replace ("DEC_MAX_EXP", DECIMAL.maxexp)
    return arg


class DecimalType(Type):
  # Regular expression to check for integer numbers and DEC[32|64|128]_[MAX|MIN]
  intnum_re = re.compile("([-|+]?[0-9]+$)|(-?DEC.*_(MAX|MIN))")

  def __init__(self, name, type, tname = "", suffix = "", decfield = "",
               maxvalue = "", minvalue = "", subnormal = "", maxexp = "",
               minexp = "", minexpsub = "", printf = "", argtype = ""):
    super(DecimalType, self).__init__(name, type, printf)
    self.tname = tname
    self.suffix = suffix
    self.decfield = decfield
    self.maxvalue = maxvalue
    self.minvalue = minvalue
    self.subnormal = subnormal

    self.minexp = minexp
    self.maxexp = maxexp
    self.minexpsub = minexpsub

    if not argtype:
      self.argtype = type
    else:
      self.argtype = argtype

  def parse_arg (self, arg):
    # NAN values
    if "NaN" in arg:
      return "{ .ieee_nan = { .c = 0x1f, .signaling_nan = %d, .negative = %d } }" \
             % (int("s" in arg), int("-" in arg))
    # Infinity
    if "Inf" in arg:
      prefix = "minus" if ("-" in arg) else "plus"
      return "{ .%s = %s_infty }" % (DECIMAL.decfield, prefix)
    # Replace <number>E[+-]DEC_[MAX|MIN]_EXP by a expected number, i.e,
    # 1E-DEC_MIN_EXP -> 1E-383
    if "DEC_MAX_EXP" in arg:
      arg = arg.replace ("DEC_MAX_EXP", DECIMAL.maxexp)
    if "DEC_MIN_EXP_SUB" in arg:
      arg = arg.replace ("DEC_MIN_EXP_SUB", DECIMAL.minexpsub)
    if "DEC_MIN_EXP" in arg:
      arg = arg.replace ("DEC_MIN_EXP", DECIMAL.minexp)
    # Macro fox max, min, tiny values
    if "DEC_MAX" in arg:
      arg = arg.replace ("DEC_MAX", DECIMAL.maxvalue);
    if "DEC_MIN" in arg:
      arg = arg.replace ("DEC_MIN", DECIMAL.minvalue);
    if "DEC_SUBNORMAL_MIN" in arg:
      arg = arg.replace ("DEC_SUBNORMAL_MIN", DECIMAL.subnormal);
    # Normal value
    if not self.intnum_re.match (arg):
      arg += DECIMAL.suffix
    return "{ .%s = %s }" % (DECIMAL.decfield, arg)


DecimalTypes = {
  "bool"      : BoolType ("bool", "_Bool", printf = "%d"),

  "int"       : IntegerType("int",
                            "int",
                            printf = "%i"),

  "longint"   : IntegerType("longint",
                            "long int",
                            printf = "%li"),

  "llongint"  : IntegerType("llongint",
                            "long long int",
                            printf = "%lli"),

  "decimal32" : DecimalType ("decimal32",
                             "_Decimal32",
                             "32",
                             "DF",
                             "sd",
                             "DEC32_MAX",
                             "DEC32_MIN",
                             "DEC32_SUBNORMAL_MIN",
                             "90",
                             "95",
                             "101",
                             "%.7HgDF",
                             "union ieee754r_Decimal32"),

  "decimal64" : DecimalType ("decimal64",
                             "_Decimal64",
                             "64",
                             "DD",
                             "dd",
                             "DEC64_MAX",
                             "DEC64_MIN",
                             "DEC64_SUBNORMAL_MIN",
                             "369",
                             "383",
                             "398",
                             "%.16DgDD",
                             "union ieee754r_Decimal64"),

  "decimal128" : DecimalType ("decimal128",
                              "_Decimal128",
                              "128",
                              "DL",
                              "td",
                              "DEC128_MAX",
                              "DEC128_MIN",
                              "DEC128_SUBNORMAL_MIN",
                              "6111",
                              "6143",
                              "6176",
                              "%.34DDgDL",
                              "union ieee754r_Decimal128")
}

DECIMAL = None

# Parse the testname.input file returning a set of (Function, Operation)
# Each file has headers (line starting with '#') describing the test
# arguments and returns. It follows:
# '#' name   <testname>
# '#' arg<N> <type>
# '#' ret    <type>
#
# Each test operation is described by one line:
# <input> <output> <list of decimal types to apply>
def parse_file (filename):
  try:
    file = open (filename, "r")
    lines = file.readlines()
    i = 0;

    # Parse the header with function description
    func = Function()

    for l in range (i, len(lines)):
      if not lines[l].rstrip().startswith ("#"):
        break

      fields = lines[l].split()
      if fields[1].startswith("name"):
        func.name = fields[2]
      if fields[1].startswith("arg"):
        func.args.append(DecimalTypes[fields[2] + DECIMAL.tname])
      elif fields[1].startswith("ret"):
        rettname = fields[2]
        if rettname == "decimal":
          rettname += DECIMAL.tname
        func.ret = DecimalTypes[rettname]
      i = i + 1

    # Parse the inputs and expected result
    operations = []
    expected_args = len (func.args)
    for l in range(i, len(lines)):
      i = i + 1
      lines[l] = lines[l].strip()
      if not lines[l]:
        continue
      # Ignore commnets
      if lines[l].startswith ("#"):
        continue

      fields = lines[l].split()
      # Check if number of arguments is the expected one
      if len(fields) - 1 < expected_args:
        print ("warning: %s:%i: line %s not follow specified function" % \
          (filename, l, lines[l]))
        continue

      # Check if the test applies to decimal type being tested
      if (len(fields) - 1) > expected_args:
        declist = fields[expected_args+1].split()
	if DECIMAL.name not in declist:
          continue

      op = Operation(i)
      for oparg in range (0, expected_args):
        op.args.append(fields[oparg])
      op.ret = fields[expected_args]

      operations.append(op)

    return (func, operations)

  except (IOError, OSError) as e:
    print ("error: open (%s) failed: %s\n" % (filename, e.strerror))
    exit (1)


def print_header ():
  print ("#ifndef __STDC_WANT_DEC_FP__")
  print ("# define __STDC_WANT_DEC_FP__")
  print ("#endif")
  print ("")
  print ("#define _GNU_SOURCE")
  print ("#include <stdio.h>")
  print ("#include <stdlib.h>")
  print ("#include <string.h>")
  print ("#include <math.h>")
  print ("#include <float.h>")
  print ("#include <endian.h>")
  print ("#include <limits.h>")
  print ("#include <errno.h>")
  print ("#include \"test-common.h\"")
  print ("")
  print ("#define _WANT_VC 1")
  print ("#include \"scaffold.c\"")

def print_special_ctes ():
  print ("#ifndef DEC_INFINITY")
  print ("# define DEC_INFINITY __builtin_infd32()")
  print ("#endif");
  print ("#ifndef DEC_NAN")
  print ("# define DEC_NAN      __builtin_nand32(\"\")")
  print ("#endif");
  print ("#define   plus_infty     DEC_INFINITY")
  print ("#define   minus_infty    -DEC_INFINITY")
  print ("#define   qnan_value     DEC_NAN")
  print ("#define   snan_value     DEC_NAN")
  print ("")

def print_operations (func, operations):
  print ("typedef struct {")
  for i in range(0, len(func.args)):
    print ("  %s arg%i;" % (func.arg_type(i), i))
  print ("  %s e;" % func.ret_init_type())
  print ("  int line;")
  print ("} operations_t;")
  print ("")
  print ("static const operations_t operations[] = {")
  for op in operations:
    print ("  {"),
    for i in range(0, len(op.args)):
      print ("%s," % func.args[i].parse_arg(op.args[i])),
    print (" %s, " % func.ret.parse_arg(op.ret)),
    print (" %d }," % op.line)
  print ("};")
  print ("static const int operations_size = \
    sizeof(operations)/sizeof(operations[0]);");
  print ("")


def print_func_call(func):
  print ("int main (void) {")
  print ("  int i;")
  print ("  for (i = 0; i < operations_size; ++i) {")

  # ret = function (arg1, ...)
  print ("    %s ret = %sd%s (" % (func.ret_type(), func.name, DECIMAL.tname)),
  for i in range(0, len(func.args)):
    print ("operations[i].arg%i.%s" % (i, DECIMAL.decfield)),
    if i is not len(func.args)-1:
      print (","),
  print (");")

  # printf ("function (arg1, ...)")
  print ("    printf (\"%sd%s (" % (func.name, DECIMAL.tname)),
  for i in range(0, len(func.args)):
    print ("%s" % (DECIMAL.printf)),
    if i is not len(func.args)-1:
      print (","),
  print (") = %s\\n\"," % (func.ret_printf())),
  for i in range(0, len(func.args)):
    print ("operations[i].arg%i.%s" % (i, DECIMAL.decfield)),
    if i is not len(func.args)-1:
      print (","),
  print (", ret);")

  # _VC_P(f,l,x,y,fmt)
  print ("    _VC_P (__FILE__, operations[i].line, "
         "operations[i].e%s, ret, \"%s\");" % \
         (func.ret_field(), func.ret_printf()));

  print ("  }")
  print ("")
  print ("  _REPORT ();")
  print ("")
  print ("  return fail;")
  print ("}")


def print_output (filename):
  (func, operations) = parse_file (filename)

  print_header ()
  print_special_ctes ()
  print_operations (func, operations)
  print_func_call (func)


if __name__ == "__main__":
  parser = OptionParser ()
  parser.add_option ("-f", "--file", dest="filename",
                     help="white output to FILE")
  parser.add_option ("-t", "--type", dest="dectype",
                     help="DECIMAL type to use")
  (options, args) = parser.parse_args()

  if options.filename:
    sys.stdout = open (options.filename, "w")

  if not args:
    print ("usage: %s <options> <input file>" % argv[0])
    exit (0)
  if not options.dectype:
    print ("error: you must specify a DECIMAL type: decimal[32|64|128]")
    exit (0)

  DECIMAL = DecimalTypes[options.dectype]

  print_output (args[0])
