#!/usr/bin/env python

import argparse
import json
import re
import os


def snake_case(name):
    # https://stackoverflow.com/a/1176023
    name = re.sub("(.)([A-Z][a-z]+)", r"\1_\2", name)
    return re.sub("([a-z0-9])([A-Z])", r"\1_\2", name).lower()


def camel_case(word):
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
    return type_ is not None


def is_array(type_):
    if type_ is None:
        return False
    return "roq::span<" in type_


def sub_type(type_):
    if type_ is not None:
        if "roq::span<" in type_:
            return type_[10:-1]
    return type_


def is_pod_or_std(type_):
    return type_ is None or type_ in defaults or "std::" in type_


_string_like_types = {"std::string_view", "roq::string_buffer", "UUID"}


def is_string_like(type_):
    if type_ is not None:
        for name in _string_like_types:
            if name in type_:
                return True
    return False


replace = {"private": "private_"}


def get_default_from_type(type_):
    if is_array(type_) or is_string_like(type_):
        return ""
    ret = defaults.get(type_)
    return ret if isinstance(ret, str) else " = {}"


def get_variable_name(name):
    ret = replace.get(name)
    return ret if ret else name


keywords = {"private": "private_"}


def _safe(name):
    tmp = keywords.get(name)
    return tmp if tmp else name


def _safe_enum(name):
    c = name[0]
    return "_" + name if c.isdigit() else name


def _format_helper(char, string, array, safe_name, accessor):
    value = ('roq::join(value.{}{}, ", "_sv)' if array else "value.{}{}").format(
        safe_name, accessor
    )
    # required until Mask has been implemented
    if safe_name in ('supports', 'supported', 'available', 'unavailable'):
        return ("'{:#x}'", value)
    if char:
        return ("'{}'", value)
    if string:
        return ('"{}"', value)
    if array:
        return ("[{}]", value)
    return ("{}", value)


def _find_default_comment(name):
    return dict(
        account="Account name (as known to the gateway)",
        commission_currency="Currency name",
        create_time_utc="Created timestamp (UTC)",
        currency="Currency name",
        exchange="Exchange name",
        exchange_time_utc="Exchange timestamp (UTC)",
        execution_instruction="Execution instruction",
        external_account="External account name (as known to broker or exchange)",
        external_order_id="External order identifier (as known to broker or exchange)",
        external_trade_id="External trade identifier (as known to broker or exchange)",
        gateway_order_id="Order identifier (as known to gateway)",
        gateway_trade_id="Trade identifier (as known to gateway)",
        order_id="Order identifier (as known to client)",
        order_template="Order template (as known to the gateway)",
        order_type="Order type",
        position_effect="Position effect",
        request_id="Request identifier (as sent to broker or exchange)",
        settlement_currency="Currency name",
        snapshot="Full update (possibly old) if true and otherwise an incremental update",
        strike_currency="Currency name",
        symbol="Symbol",
        time_in_force="Time in force",
        trade_id="Trade identifier (as known to client)",
        update_time_utc="Updated timestamp (UTC)",
        stream_id="Stream identifier",
    ).get(name, "")


def _new_spec_helper(item):
    k = item["name"]
    r = item.get("raw", k)
    t = item.get("type")
    comment = item.get("comment") or _find_default_comment(k)
    name = _safe_enum(snake_case(k))
    safe_name = _safe(name)
    char = t == "char"
    string = is_string_like(t)
    array = is_array(t)
    enum = not is_pod_or_std(t) and not array and not string
    is_float = t == "double"
    tag = item.get("tag", -1)
    custom = item.get("custom", False)
    default = item.get("default", get_default_from_type(t))
    accessor = item.get("accessor", "")
    format_string, format_value = _format_helper(char, string, array, safe_name, accessor)
    position = item.get("position", 0)
    return dict(
        raw_name=r,
        name=safe_name,
        enum_value=name.upper(),
        comment=comment,
        is_variable=is_variable(t),
        tag=tag,
        custom=custom,
        type=t,
        is_required=item.get("required", False),
        is_map=item.get("map", False),
        default=default,
        include=t is not None,
        is_string=string,
        is_array=array,
        is_enum=enum,
        is_float=is_float,
        format_string=format_string,
        format_value=format_value,
        position=position,
    )


def _include_helper(namespaces, variable):
    if variable["is_array"]:
        return namespaces + (snake_case(sub_type(variable["type"])),)
    if "::" not in variable["type"]:
        return namespaces + (snake_case(sub_type(variable["type"])),)
    assert False


def new_spec(path, namespaces, name, comment, spec, type_):
    filename = os.path.splitext(os.path.basename(path))[0]

    values = [_new_spec_helper(item) for item in spec]
    variables = [value for value in values if value["is_variable"]]

    if type_ != "enum":
        if len(comment) == 0:
            raise RuntimeError("{} has no commens".format(name))
        if any([len(value["comment"]) == 0 for value in values]):
            raise RuntimeError(
                "{} requires comments for following fields {}".format(
                    name, [value["name"] for value in values if len(value["comment"]) == 0]
                )
            )

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
    with open(path) as fd:
        doc = json.load(fd)
        type_ = doc["type"]
        name = doc["name"]
        comment = doc["comment"]
        values = doc["values"]
        from jinja2 import Environment, FileSystemLoader

        env = Environment(loader=FileSystemLoader(templates), trim_blocks=True, lstrip_blocks=True)
        template = env.get_template(".".join((type_, file_type)))
        spec = new_spec(path, namespaces, name, comment, values, type_)
        result = template.render(**spec)
        print(result)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--namespace", type=str, required=True, help="namespace")
    parser.add_argument("--file-type", type=str, required=True, help="output file (h/cpp)")
    parser.add_argument("spec", type=str, help="spec file (.json)")
    args = parser.parse_args()

    namespaces = tuple(args.namespace.split("/"))

    dirname = os.path.dirname(os.path.realpath(__file__))
    templates = os.path.join(dirname, "templates")

    process(args.file_type, args.spec, namespaces, templates)
