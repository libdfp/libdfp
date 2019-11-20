#!/usr/bin/env python

import sys
import re
import os
from optparse import OptionParser

# Use CPP flags to guard tests which should not be
# be on certain platforms (e.g sizeof(long int) != x
cppcheckdict = {
    "ONLY_ON_4BYTE_LONG" : "(__SIZEOF_LONG__ == 4)",
    "ONLY_ON_8BYTE_LONG" : "(__SIZEOF_LONG__ == 8)"
    }

# Decribes the evaluated function
class Function:
  def __init__ (self):
    self.name = None
    self.args = []
    self.ret = None
    self.ulp = False

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
    self.extraflags = []
    self.line = line
    self.cppchecks = []

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
      if fields[2].startswith("decimal"):
        func.args.append(DecimalTypes[fields[2] + DECIMAL.tname])
      else:
        func.args.append(DecimalTypes[fields[2]])
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

    # Split all further args in decimal or extraflags (e.g. excpetions)
    declist = []
    extra_args = []
    if (len(fields) - 1) > expected_args:
      for arg in fields[expected_args + 1 : ]:
        if arg.startswith("decimal"):
          declist.append(arg)
        else:
          extra_args.append(arg)

    # Check if the test applies to decimal type being tested
    if (len(declist) > 0):
      if DECIMAL.name not in declist:
        continue

    op = Operation(i)
    for oparg in range (0, expected_args):
      op.args.append(fields[oparg])
    op.ret = fields[expected_args]

    for earg in extra_args:
      if earg in cppcheckdict.keys():
	op.cppchecks.append(cppcheckdict[earg])
      else:
	op.extraflags.append(earg)

    # Finally, if we ignore the results, coerce to a value
    # which won't make the compiler generate warnings:
    if "IGNORE_RESULT" in op.extraflags:
      if op.ret != "0":
        sys.stderr.write ("WARNING: ignored value coerced to 0 on line %d\n" % i)
      op.ret = "0"

    operations.append(op)

  return (func, operations)


def print_header (func):
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
  print ("#include <fenv.h>")
  print ("")
  # Macros required for test build
  print ("#define %s" % DECIMAL.name.upper())
  print ("#define FLOAT %s" % DECIMAL.type)
  print ("#define FUNC(function) function ## %s" % ('d' + DECIMAL.tname))
  print ("")
  print ("#include \"libdfp-test-ulps.h\"")
  print ("#include \"libdfp-test.c\"")


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
  print ("  const char *argname;")
  for i in range(0, len(func.args)):
    print ("  %s arg%i;" % (func.arg_type(i), i))
  print ("  %s e;" % func.ret_init_type())
  print ("  int extraflags;")
  print ("  int line;")
  print ("} operations_t;")
  print ("")
  print ("static const operations_t operations[] = {")
  for op in operations:
    if op.cppchecks:
        print ("#if " + " && ".join (op.cppchecks))
    line = ""
    line += "  {"
    line += "\"%s\", " % ", ".join(op.args)
    for i in range(0, len(op.args)):
      line += "%s," % func.args[i].parse_arg(op.args[i])
    line += " %s, " % func.ret.parse_arg(op.ret)
    if (len(op.extraflags) > 0):
      line += "|".join (op.extraflags)
      line += ", "
    else:
      line += "NO_EXTRA_FLAG, "
    line += " %d }," % op.line
    print (line)
    if op.cppchecks:
      print ("#endif")
  print ("};")
  print ("static const int operations_size = \
    sizeof(operations)/sizeof(operations[0]);");
  print ("")


def print_func_call(func):
  print ("int main (int argn, char *argv[]) {")
  print ("  int i;")
  print ("")
  print ("  scaffold_setup (argn, argv, \"%s\");" % func.name)
  print ("  for (i = 0; i < operations_size; ++i) {")

  # <macro> (func.name, arg1, arg2, ..., ret, expected)
  MACROSUFFIX = {
    "bool"       : "b",
    "int"        : "i",
    "longint"    : "l",
    "llongint"   : "L",
    "decimal32"  : "f",
    "decimal64"  : "f",
    "decimal128" : "f"
  }
  macro = "RUN_TEST_";
  for arg in func.args:
    macro += MACROSUFFIX[arg.name]
  macro += "_" + MACROSUFFIX[func.ret.name]

  line = "    %s (%s, operations[i].argname, " % (macro, func.name, )
  for i in range(0, len(func.args)):
    line += "operations[i].arg%i" % i
    if "decimal" in func.args[i].name:
      line += "." + DECIMAL.decfield
    line += ", "

  line += "operations[i].e%s, operations[i].extraflags);" % func.ret_field()
  print (line)

  print ("  }")
  print ("  scaffold_teardown ();")
  print ("")
  print ("  return noErrors;");
  print ("}")


def print_output (filename):
  (func, operations) = parse_file (filename)

  print_header (func)
  print_special_ctes ()
  print_operations (func, operations)
  print_func_call (func)


if __name__ == "__main__":
  parser = OptionParser ()
  parser.add_option ("-o", "--output", dest="filename",
                     help="white output to FILE")
  parser.add_option ("-t", "--type", dest="dectype",
                     help="DECIMAL type to use")
  (options, args) = parser.parse_args()

  if options.filename:
    sys.stdout = open (options.filename, "w")

  if not args:
    sys.stderr.write ("usage: gen-libdfp-tests.py <options> <input file\n>")
    raise Exception()
  if not options.dectype:
    sys.stderr.write ("error: you must specify a type: decimal[32|64|128]")
    raise Exception()

  DECIMAL = DecimalTypes[options.dectype]

  print_output (args[0])
