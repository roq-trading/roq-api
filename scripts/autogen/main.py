#!/usr/bin/env python

"""
Copyright (c) 2017-2022, Hans Erik Thrane

Command-line tool to auto-generate source code files based on JSON spec and templates
"""

import argparse
import json
import re
import os

from jinja2 import Environment, FileSystemLoader


def snake_case(name):
    """convert to snake-case"""
    # https://stackoverflow.com/a/1176023
    name = re.sub("(.)([A-Z][a-z]+)", r"\1_\2", name)
    return re.sub("([a-z0-9])([A-Z])", r"\1_\2", name).lower()


def camel_case(word):
    """convert to camel-case"""
    # https://www.w3resource.com/python-exercises/re/python-re-exercise-37.php
    return "".join(x.capitalize() or "_" for x in word.split("_"))


defaults = {
    "std::string_view": " = {}",
    "std::chrono::nanoseconds": " = {}",
    "std::chrono::milliseconds": " = {}",
    "std::chrono::seconds": " = {}",
    "std::chrono::days": " = {}",
    "roq::chrono::days": " = {}",
    "bool": " = false",
    "char": " = '\\0'",
    "int8_t": " = {}",
    "uint8_t": " = {}",
    "int16_t": " = {}",
    "uint16_t": " = {}",
    "int32_t": " = {}",
    "uint32_t": " = {}",
    "int64_t": " = {}",
    "uint64_t": " = {}",
    "std::int32_t": " = {}",
    "std::uint32_t": " = {}",
    "std::int64_t": " = {}",
    "std::uint64_t": " = {}",
    "double": " = NaN",
}


def is_variable(type_):
    """test if is a variable"""
    return type_ is not None


def is_array(type_):
    """test if is an array"""
    if type_ is None:
        return False
    return "roq::span<" in type_


def sub_type(type_):
    """find sub-type"""
    if type_ is not None:
        if "roq::span<" in type_:
            return type_[10:-1]
    return type_


def is_pod_or_std(type_):
    """test if is plain-old-data or a std type"""
    return type_ is None or type_ in defaults or "std::" in type_


_string_like_types = {"std::string_view", "roq::string_buffer", "UUID"}


def is_string_like(type_):
    """test if is string-like"""
    if type_ is not None:
        for name in _string_like_types:
            if name in type_:
                return True
    return False


replace = {"private": "private_"}


def get_default_from_type(type_):
    """get default value from type"""
    if is_array(type_) or is_string_like(type_):
        return ""
    ret = defaults.get(type_)
    return ret if isinstance(ret, str) else " = {}"


def get_variable_name(name):
    """get the variable name"""
    ret = replace.get(name)
    return ret if ret else name


keywords = {"private": "private_"}


def _safe(name):
    """make name safe (avoid restricted keywords)"""
    tmp = keywords.get(name)
    return tmp if tmp else name


def _safe_enum(name):
    """make safe enum string (can't start with a digit)"""
    first_char = name[0]
    return "_" + name if first_char.isdigit() else name


def _format_helper(char, string, array, safe_name, accessor):
    """format helper"""
    value = ('fmt::join(value.{}{}, ", "sv)' if array else "value.{}{}").format(safe_name, accessor)
    # required until Mask has been implemented
    if safe_name in ("supports", "supported", "available", "unavailable"):
        return ("{:#x}", value)
    if char:
        return ("'{}'", value)
    if array:
        return ("[{}]", value)
    if string:
        return ('"{}"', value)
    return ("{}", value)


def _find_default_comment(name):
    """some variable names are repeated many times"""
    return dict(
        account="Account name",
        commission_currency="Currency",
        conditional_on_version="Auto-reject if this version has positively failed (optional)",
        create_time_utc="Created timestamp (UTC)",
        currency="Currency",
        exchange="Exchange",
        exchange_time_utc="Exchange timestamp (UTC)",
        execution_instruction="Execution instruction",
        external_account="External account name",
        external_order_id="External order identifier",
        external_trade_id="External trade identifier",
        gateway_order_id="Order identifier",
        gateway_trade_id="Trade identifier",
        order_id="Order identifier",
        order_template="Order template",
        order_type="Order type",
        position_effect="Position effect",
        request_id="Request identifier",
        routing_id="Routing identifier",
        settlement_currency="Currency",
        snapshot="Full update (possibly old) if true and otherwise an incremental update",
        stream_id="Stream identifier",
        strike_currency="Currency",
        symbol="Symbol",
        time_in_force="Time in force",
        trade_id="Trade identifier",
        update_time_utc="Updated timestamp (UTC)",
        version="Version number (strictly increasing, optional)",
    ).get(name, "")


def _new_spec_helper(item):
    """aggregate the specification as a dict"""
    name_ = item["name"]
    raw_name = item.get("raw", name_)
    type_ = item.get("type")
    comment = item.get("comment") or _find_default_comment(name_)
    name = _safe_enum(snake_case(name_))
    safe_name = _safe(name)
    char = type_ == "char"
    string = is_string_like(type_)
    array = is_array(type_)
    enum = not is_pod_or_std(type_) and not array and not string
    is_float = type_ == "double"
    tag = item.get("tag", -1)
    custom = item.get("custom", False)
    default = item.get("default", get_default_from_type(type_))
    accessor = item.get("accessor", "")
    format_string, format_value = _format_helper(char, string, array, safe_name, accessor)
    position = item.get("position", 0)
    return dict(
        raw_name=raw_name,
        name=safe_name,
        enum_value=name.upper(),
        comment=comment,
        is_variable=is_variable(type_),
        tag=tag,
        custom=custom,
        type=type_,
        is_required=item.get("required", False),
        is_map=item.get("map", False),
        default=default,
        include=type_ is not None,
        is_string=string,
        is_array=array,
        is_enum=enum,
        is_float=is_float,
        format_string=format_string,
        format_value=format_value,
        position=position,
    )


def _include_helper(namespaces, variable):
    """sometimes we need a custom include"""
    if variable["is_array"]:
        return namespaces + (snake_case(sub_type(variable["type"])),)
    if "::" not in variable["type"]:
        return namespaces + (snake_case(sub_type(variable["type"])),)
    assert False


def new_spec(path, namespaces, name, comment, spec, type_):
    """aggregate the spec as a dict"""
    filename = os.path.splitext(os.path.basename(path))[0]

    values = [_new_spec_helper(item) for item in spec]
    variables = [value for value in values if value["is_variable"]]

    if type_ != "enum":
        if len(comment) == 0:
            raise RuntimeError(f"{name} has no comments")
        if any([len(value["comment"]) == 0 for value in values]):
            fields = [value["name"] for value in values if len(value["comment"]) == 0]
            raise RuntimeError(f"{name} requires comments for following fields {fields}")

    includes = sorted(
        {
            _include_helper(namespaces, variable)
            for variable in variables
            if (variable["is_array"] and not is_string_like(sub_type(variable["type"])))
            or variable["is_enum"]
        }
    )

    return dict(
        namespaces=namespaces,
        name=name,
        comment=comment,
        filename=filename,
        variables=variables,
        values=values,
        includes=includes,
    )


def process(file_type, path, namespaces, templates):
    """generate the output file based on a template"""
    with open(path) as file:
        doc = json.load(file)
        type_ = doc["type"]
        name = doc["name"]
        comment = doc["comment"]
        values = doc["values"]

        env = Environment(loader=FileSystemLoader(templates), trim_blocks=True, lstrip_blocks=True)
        template = env.get_template(".".join((type_, file_type)))
        spec = new_spec(path, namespaces, name, comment, values, type_)
        result = template.render(**spec)
        print(result)


def main():
    """main function"""
    parser = argparse.ArgumentParser()
    parser.add_argument("--namespace", type=str, required=True, help="namespace")
    parser.add_argument("--file-type", type=str, required=True, help="output file (h/cpp)")
    parser.add_argument("spec", type=str, help="spec file (.json)")
    args = parser.parse_args()

    namespaces = tuple(args.namespace.split("/"))

    dirname = os.path.dirname(os.path.realpath(__file__))
    templates = os.path.join(dirname, "templates")

    process(args.file_type, args.spec, namespaces, templates)


if __name__ == "__main__":
    main()
