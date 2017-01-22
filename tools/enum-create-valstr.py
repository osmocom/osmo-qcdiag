#!/usr/bin/env python
#
# Code to generate a 'struct value string' from enums
# (C) 2017 by Harald Welte <laforge@gnumonks.org>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

from optparse import OptionParser
import pprint
from pycparser import c_parser, c_ast, parse_file
from value_string import export_value_str

class EnumValStrVisitor(c_ast.NodeVisitor):
    "Generate a 'struct value_string' from an enum"
    def visit_Enum(self, node):
        name = "%s_vals" % node.name
        elist = node.values
        vdict = {}
        for val in elist.enumerators:
            vdict[val.name] = val.name
        export_value_str(name, vdict, sort_key=None)

class EnumValuePrinter(c_ast.NodeVisitor):
    def visit_Enum(self, node):
        print("enum %s = {" % node.name)
        elist = node.values
        next_val = 0
        for val in elist.enumerators:
            if val.value:
                numeric = int(val.value.value, 0)
                next_val = numeric
            else:
                numeric = next_val
            print("\t%s = %d" % (val.name, numeric))
            next_val += 1
        print("};")


parser = OptionParser()
(options, args) = parser.parse_args()
filename = args[0]

ast = parse_file(filename, use_cpp=True)

v = EnumValStrVisitor()
#v = EnumValuePrinter()
v.visit(ast)

