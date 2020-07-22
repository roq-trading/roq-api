# Copyright (c) 2017-2020, Hans Erik Thrane

""" test module """

# import pytest

def test_simple():
    """ simple test """
    try:
        import roq
        print(dir(roq))
    except ImportError:
        assert False
    try:
        import roq.fbs
        print(dir(roq.fbs))
    except ImportError:
        assert False
    try:
        import roq.fbs.ConnectionStatus
        print(dir(roq.fbs.ConnectionStatus))
    except ImportError:
        assert False
    try:
        import roq.client.Trading
        print(dir(roq.client.Trading))
    except ImportError:
        assert False
    assert False
