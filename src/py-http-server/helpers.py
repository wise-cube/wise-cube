from flask_table import Table, Col, create_table
from common.database import ScopedSession


def rel2table(relation):
    rel_fields = dir(relation)
    ignored_fields = ['metadata']
    sorted_fields = ['id','name']
    fields = [f for f in sorted_fields if f in rel_fields ] + [x for x in rel_fields if not x.startswith('_') and not (x  in ignored_fields or x in sorted_fields)]
    print(rel_fields)
    table_class = create_table(options={'classes':['table','is-bordered', 'is-striped', 'is-narrow']})
    for f in fields:
        table_class.add_column(f, Col(f.capitalize()))
    entries = ScopedSession.query(relation).all()
    return table_class(entries)