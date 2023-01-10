# Flatbuffers Schema

!!! EXPERIMENTAL !!!

## Motivation

Using a Flatbuffers serialization format is an alternative 
to creating binary language bindings.

Market gateways will eventually support a low latency
Flatbuffers interface.

Note! The C++ API remains the choice for low latency.

## Implementation

The schema will be maintained "by hand" until it has been
properly tested.

The intention is to auto generate `.fbs` files from the
`.json` files which are anyway used to auto generate many
of the headers of the C++ API.

## Usage

You should use the Flatbuffers compiler (`flatc`) to
generate the bindings for your particular language.

You can normally find the schema here:
`$PREFIX/share/roq/fbs`
