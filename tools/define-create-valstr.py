#!/usr/bin/env python

from optparse import OptionParser
from pyparsing import *
from value_string import *

# define the structure of a macro definition (the empty term is used 
# to advance to the next non-whitespace character)
macroDef = "#define" + Word(alphas+"_",alphanums+"_").setResultsName("macro") + \
            empty + restOfLine.setResultsName("value")

NL = Suppress( LineEnd() )
restOfLineNL = restOfLine + NL

LineComment = Literal('//') + restOfLineNL
Comment = cStyleComment | LineComment

# doesn't work :/
macroDef.ignore(Comment)

parser = OptionParser()
parser.add_option("-n", "--name", dest="name",
                  help="Name of the value_string symbol to create")
parser.add_option("-f", "--flavor", dest="flavor", default='osmocom',
                  help="Flavor of generated C (osmocom, wireshark)")
parser.add_option("-w", "--weak-symbol", dest="weak", default=True,
                  help="Generate weak symbols")
(options, args) = parser.parse_args()
filename = args[0]

wr = ValueStringWriter(flavor=options.flavor, weak=options.weak, includes=[filename])

with open(filename, 'r') as f:
    res = macroDef.scanString(f.read())
    vdict = {}
    wr.export_header()
    for tokens, startPos, EndPos in res:
        vdict[tokens.value] = tokens.macro
    wr.export_value_str(options.name, vdict, sort_key=None)
