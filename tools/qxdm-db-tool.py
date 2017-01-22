#!/usr/bin/env python
#
# Code to parse the ASCII Database files of QXDM
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
from qxdm_db import QxdmDb

parser = OptionParser()
parser.add_option("-d", "--database-dir", dest="db_dir",
                  help="QXDM Database directory to use")

(options, args) = parser.parse_args()

if not options.db_dir:
    print "You have to specify the database directory"
    exit(2)

db = QxdmDb(options.db_dir)

db.export_valstr_events()
db.export_valstr_event_descs()
db.export_valstr_logitems()
db.export_valstr_msgs()
