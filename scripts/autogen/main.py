#!/usr/bin/env python

import argparse
import json
import re
import os


def snake_case(name):
    # https://stackoverflow.com/a/1176023
    name = re.sub('(.)([A-Z][a-z]+)', r'\1_\2', name)
    return re.sub('([a-z0-9])([A-Z])', r'\1_\2', name).lower()


def camel_case(word):
    # https://www.w3resource.com/python-exercises/re/python-re-exercise-37.php
    return ''.join(x.capitalize() or '_' for x in word.split('_'))


defaults = {
    'std::string_view': '',
    'std::chrono::nanoseconds': ' = {}',
    'bool': ' = false',
    'char': ' = \'\\0\'',
    'int8_t': ' = 0',
    'uint8_t': ' = 0',
    'int16_t': ' = 0',
    'uint16_t': ' = 0',
    'int32_t': ' = 0',
    'uint32_t': ' = 0',
    'int64_t': ' = 0',
    'uint64_t': ' = 0',
    'std::int32_t': ' = 0',
    'std::uint32_t': ' = 0',
    'std::int64_t': ' = 0',
    'std::uint64_t': ' = 0',
    'double': ' = std::numeric_limits<double>::quiet_NaN()'}


def is_variable(type_):
    return type_ is not None


def is_array(type_):
    if type_ is None:
        return False
    return 'roq::span<' in type_


def sub_type(type_):
    if type_ is not None:
        if 'roq::span<' in type_:
            return type_[10:-1]
    return type_


def is_pod_or_std(type_):
    return type_ is None or type_ in defaults or 'std::' in type_

_string_like_types = {
    'std::string_view',
    'roq::string'
}

def is_string_like(type_):
    if type_ is not None:
        for name in _string_like_types:
            if name in type_:
                return True
    return False

replace = {
    'private': 'private_'
}

def get_default_from_type(type_):
    if is_array(type_) or is_string_like(type_):
        return ''
    ret = defaults.get(type_)
    return ret if isinstance(ret, str) else ' = {}::UNDEFINED'.format(type_)

def get_variable_name(name):
    ret = replace.get(name)
    return ret if ret else name


keywords = {
    'private': 'private_'
}


def _safe(name):
    tmp = keywords.get(name)
    return tmp if tmp else name


def _safe_enum(name):
    c = name[0]
    return '_' + name if c.isdigit() else name

def _format_helper(char, string, array, safe_name, accessor):
    value=('fmt::join(value.{}{}, ", ")' if array else 'value.{}{}').format(safe_name, accessor)
    if char:
        return ('\'{}\'', value)
    if string:
        return ('"{}"', value)
    if array:
        return ('[{}]', value)
    return ('{}', value)


def _new_spec_helper(item):
    k = item['name']
    r = item.get('raw', k)
    t = item.get('type')
    comment = item.get('comment', '')
    name = _safe_enum(snake_case(k))
    safe_name = _safe(name)
    char = t == 'char'
    string = is_string_like(t)
    array = is_array(t)
    enum = not is_pod_or_std(t) and not array and not string
    is_float = t == 'double'
    tag = item.get('tag', -1)
    custom = item.get('custom', False)
    default = item.get('default', get_default_from_type(t))
    accessor = item.get('accessor', '')
    format_string, format_value = _format_helper(char, string, array, safe_name, accessor)
    return dict(
        raw_name=r,
        name=safe_name,
        enum_value=name.upper(),
        comment=comment,
        is_variable=is_variable(t),
        tag=tag,
        custom=custom,
        type=t,
        is_required=item.get('required', False),
        is_map=item.get('map', False),
        default=default,
        include=t is not None,
        is_string=string,
        is_array=array,
        is_enum=enum,
        is_float=is_float,
        format_string=format_string,
        format_value=format_value)


def _include_helper(namespaces, variable):
    if variable['is_array']:
        return namespaces + (snake_case(sub_type(variable['type'])),)
    if '::' not in variable['type']:
        return namespaces + (snake_case(sub_type(variable['type'])),)
    assert False


def new_spec(path, namespaces, name, comment, spec, type_):
    filename = os.path.splitext(os.path.basename(path))[0]

    values = [_new_spec_helper(item) for item in spec]
    variables = [value for value in values if value['is_variable']]

    includes = sorted({_include_helper(namespaces, variable) for variable in variables if (
        variable['is_array'] and not is_string_like(sub_type(variable['type']))) or variable['is_enum']})

    return dict(
        namespaces=namespaces,
        name=name,
        comment=comment,
        filename=filename,
        variables=variables,
        values=values,
        includes=includes)


def process(file_type, path, namespaces, templates):
    with open(path) as fd:
        doc = json.load(fd)
        type_ = doc['type']
        name = doc['name']
        comment = doc['comment']
        values = doc['values']
        from jinja2 import Environment, FileSystemLoader
        env = Environment(
                loader=FileSystemLoader(templates),
                trim_blocks=True,
                lstrip_blocks=True)
        template = env.get_template('.'.join((type_, file_type)))
        spec = new_spec(path, namespaces, name, comment, values, type_)
        result = template.render(**spec)
        print(result)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument(
        '--namespace',
        type=str,
        required=True,
        help='namespace')
    parser.add_argument(
        '--file-type',
        type=str,
        required=True,
        help='output file (h/cpp)')
    parser.add_argument('spec', type=str, help='spec file (.json)')
    args = parser.parse_args()

    namespaces = tuple(args.namespace.split('/'))

    dirname = os.path.dirname(os.path.realpath(__file__))
    templates = os.path.join(dirname, 'templates')

    process(args.file_type, args.spec, namespaces, templates)
