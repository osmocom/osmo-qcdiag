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

import csv
from value_string import export_value_str

class QxdmDb(object):
    file_fields = {
        'Enum': ['id', 'name', 'entries_id', 'unknown'],
        'EnumEntry': ['enum_id', 'value', 'name', 'unknown'],
        'EventID': ['value', 'name', 'category_id', 'description_id'],
        'EventCategory': ['value', 'name', 'unknown', 'unknown2'],
        'Struct': ['id', 'index', 'unknown', 'field_id', 'unknown2', 'unknown3', 'unknown4'],
        'Field': ['id', 'name', 'bits', 'unknown1', 'unknown2',
            'unknown3', 'unknown4', 'unknown5'],
        'Description': ['id', 'text'],
        'LogItem': ['id', 'name', 'category_id', 'unknown'],
        'LogCategory': ['id', 'name', 'unknown', 'parent_id'],
        'MessageLevel': ['id', 'level', 'name', 'category_id'],
        'MessageCategory': ['id', 'name', 'unknown', 'parent_id'],
    }
    _csvdicts= {}

    descs = {}
    enums = {}
    events = {}
    event_cats = {}
    logitems = {}
    log_cats = {}
    msgs = {}
    msg_cats = {}

    @staticmethod
    def read_into_dict(fname, fieldnames=None):
        csvfile = open(fname, 'rb')
        r = csv.DictReader(csvfile, fieldnames=fieldnames, delimiter='^')
        return r

    def gen_descs(self):
        for d in self._csvdicts['Description']:
            self.descs[int(d['id'], base=0)] = d['text']

    def gen_enums(self):
        for e in self._csvdicts['Enum']:
            self.enums[int(e['id'], base=0)] = {'name': e['name'], 'entries': {}}
        for ee in self._csvdicts['EnumEntry']:
            self.enums[int(ee['enum_id'], base=0)]['entries'][int(ee['value'],base=0)] = ee['name']

    def gen_events(self):
        for c in self._csvdicts['EventCategory']:
            self.event_cats[int(c['value'],base=0)] = {'name': c['name'], 'events': []}

        for e in self._csvdicts['EventID']:
            val_int = int(e['value'], base=0)
            self.events[val_int] = {'name': e['name']}
            if e['description_id'] != '-1':
                self.events[val_int]['description'] = self.descs[int(e['description_id'], base=0)]
            cats = e['category_id'].split(',')
            for c in cats:
                cat_int = int(c, base=0)
                if cat_int in self.event_cats:
                    self.event_cats[cat_int]['events'].append(self.events[val_int])

    def gen_logitems(self):
        for c in self._csvdicts['LogCategory']:
            self.log_cats[int(c['id'],base=0)] = {'name': c['name'], 'items': []}

        for l in self._csvdicts['LogItem']:
            val_int = int(l['id'], base=0)
            self.logitems[val_int] = {'name': l['name']}

            cats = l['category_id'].split(',')
            for c in cats:
                cat_int = int(c, base=0)
                if cat_int in self.log_cats:
                    self.log_cats[cat_int]['items'].append(self.logitems[val_int])

    def gen_msglevels(self):
        def strip_suffix(s):
            slash = s.rfind('/')
            if slash == -1:
                return s
            else:
                return s[:slash]
        for c in self._csvdicts['MessageCategory']:
            self.msg_cats[int(c['id'],base=0)] = {'name': c['name'], 'msgs': []}

        for l in self._csvdicts['MessageLevel']:
            val_int = int(l['id'], base=0)
            level_int = int(l['level'], base=0)
            if not val_int in self.msgs:
                self.msgs[val_int] = {'levels':{}, 'name': strip_suffix(l['name'])}
            self.msgs[val_int]['levels'][level_int] = {'name': l['name']}

            cats = l['category_id'].split(',')
            for c in cats:
                cat_int = int(c, base=0)
                if cat_int in self.msg_cats:
                    self.msg_cats[cat_int]['msgs'].append(self.msgs[val_int]['levels'][level_int])


    def __init__(self, dir_to_db):
        for f in self.file_fields:
            fname = '%s/%s.txt' % (dir_to_db, f)
            self._csvdicts[f] = self.read_into_dict(fname, self.file_fields[f])
        self.gen_descs()
        self.gen_enums()
        self.gen_events()
        self.gen_logitems()
        self.gen_msglevels()

    def export_valstr_events(self):
        def f(v):
            return v['name']
        vals = dict(zip(self.events, map(f, self.events.values())))
        export_value_str('diag_event_vals', vals)

    def export_valstr_event_descs(self):
        vals = {}
        for e in self.events:
            if 'description' in self.events[e]:
                vals[e] = self.events[e]['description']
        export_value_str('diag_event_descs', vals)

    def export_valstr_logitems(self):
        def f(v):
            return v['name']
        vals = dict(zip(self.logitems, map(f, self.logitems.values())))
        export_value_str('diag_log_item_vals', vals)

    def export_valstr_msgs(self):
        def f(v):
            return v['name']
        vals = dict(zip(self.msgs, map(f, self.msgs.values())))
        export_value_str('diag_msg_vals', vals)
