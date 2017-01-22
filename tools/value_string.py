#!/usr/bin/env python
#
# Helpers for 'struct value_string' as found in libosmocore and wireshark
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


def export_value_str(name, vals, flavor='osmocom', sort_key=int):
    if flavor == 'osmocom':
        print("const struct value_string %s[] = {" % name);
    elif flavor == 'wireshark':
        print("const value_string %s[] = {" % name);
    for v in sorted(vals.iterkeys(), key=sort_key):
        print("\t{ %s, \"%s\" }," % (v, vals[v]));
    print("\t{ 0, NULL }")
    print("};");
